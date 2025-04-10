#include <cstddef>
#include <deque>
#include <iterator>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <utility>

template<typename T, typename Container = std::deque<T> >
class queue {
public :
    //explicit修饰，禁止隐式调用构造函数
    //默认构造函数
    explicit queue(const Container& c) : container(c) {}

    //移动构造函数
    explicit queue(Container&& c = Container()) : container(std::move(c)) {}

    template<class Alloc> queue(const Container&, const Alloc&);//Alloc表示分配器类型
    template<class Alloc> queue(Container&&, const Alloc&);
    template<class Alloc> queue(queue&&, const Alloc&);

    //const修饰，该成员函数不会修改对象的任何变量
    //判断是否为空
    bool empty() const {
        std::lock_guard<std::mutex> lock(mtx);
        return container.empty();
    }

    //返回队列大小
    size_t size() const {
        std::lock_guard<std::mutex> lock(mtx);
        return container.size();
    }

    //获取队首元素
    T& top() {
        std::lock_guard<std::mutex> lock(mtx);
        return container.front();
    }

    //获取队首元素，const版本
    T const& top() const {
        std::lock_guard<std::mutex> lock(mtx);
        return container.front();
    }

    //拷贝插入元素
    void push(const T& value) {
        std::lock_guard<std::mutex> lock(mtx);
        container.push_back(value);
    }

    //移动插入元素
    void push(T&& value) {
        std::lock_guard<std::mutex> lock(mtx);
        container.push_back(std::move(value));
        cv.notify_one();
    }

    //弹出队首元素
    void pop() {
        std::lock_guard<std::mutex> lock(mtx);
        container.pop_front();
    }

    //交换两个队列
    void swap(queue&& other) {
        if(this == other) return ;
        std::scoped_lock lock(mtx,other.mtx);
        container.swap(other.container);
    }

    //线程接口安全的尝试弹出元素
    std::shared_ptr<T> try_pop() {
        std::lock_guard<std::mutex> lock(mtx);
        if(container.empty()) {
            return nullptr;
        }
        auto back = std::make_shared<T>(container.front());
        container.pop_front();
        return back;
    }

    //线程接口安全的阻塞等待直到有数据弹出
    std::shared_ptr<T> wait_pop() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] {return !container.empty();});
        auto back = std::make_shared<T>(container.front());
        container.pop_front();
        return back;
    }

    //实现类内构造
    template<class... Args>
    void emplace(Args&&...args) {
        std::lock_guard<std::mutex> lock(mtx);
        container.emplace_back(std::forward<Args>(args)...);
    }

private:
    mutable std::mutex mtx;
    std::condition_variable cv;
    Container container;
};

