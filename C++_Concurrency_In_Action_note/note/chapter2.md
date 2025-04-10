### 线程管理
创建线程时传递的函数对象会被复制到新线程的存储空间
>注意，当传递的是临时变量时，可能会导致*C++’s most vexing parse*，C++会将其解析为函数声明，而不是类型对象的定义，而解决办法就是多加一组括号，或使用初始化

    class background_task
    {
    public:
        void operator()() const
        {
            do_something();
            do_something_else();
        }
    };

    background_task f;
    std::thread my_thread(f);

    //编译器认为的
    std::thread my_thread(background_task());

    //解决
    std::thread my_thread((background_task()));  
    std::thread my_thread{background_task()};    

注意在进行线程函数的传参时，尽量不要传递临时变量的引用或指针，因为不能确保新线程运行期间该变量是否还存在，就算使用临时变量复制到线程中，仍是个糟糕的主意

线程的返回
通常是调用join或datach，如果是使用datach，那么线程就会分离，结束自动回收，无需担心太多，但如果是join就要考虑join的位置，如果在线程运行期间报错，而主线程还未运行到join,这样就会跳过join

线程的传参
所有的线程默认是将参数值传递给新线程的，也就是拷贝复制，尽管参数是引用，但也不会传引用，而是复制一份对象
>注意
不要将局部变量的指针进行传参，可能造成悬空指针
如果参数要求是引用，请使用ref绑定
当成员函数作为函数对象传参，第一个参数应该传递对象地址

转移所有权
thread支持移动，但不可复制，可以使用move转移所有权
注意：不能用一个已有线程关联的对象接受新线程

线程的数量
使用std::thread::hardware_concurrency()可以获得并发线程的数量

获取线程id
1. 可以通过线程对象get_id()获取
2. 可以通过当前线程获取，std::this_thread::get_id();

通过获取线程id,可以判断当前线程，进而执行不同的任务，也可以在调试输出中使用