#include <algorithm>
#include <cmath>
#include <future>
#include <iostream>
#include <memory>
#include <pthread.h>
#include <unistd.h>
#include <vector>
#include <queue>
#include <functional>
using namespace std;
const int MAX_NUM=12;


class thread_pool{
    public:
    queue<function<void()>> workqueue;
    bool stop;
    vector<pthread_t> pthread_num;
    pthread_mutex_t mutex ;
    pthread_cond_t  cond_work;
    public:
    thread_pool(){
        pthread_mutex_init(&mutex,NULL);
        pthread_cond_init(&cond_work, NULL);
        pthread_num.resize(MAX_NUM);
        stop=false;
        for(int i=0;i<MAX_NUM;i++){
            if(pthread_create(&pthread_num[i],NULL,
            [](void*arg)->void*{static_cast<thread_pool*>(arg)->work();
            return NULL;},this)!=0){
                cout<<"thread create failed"<<endl;
                exit(-1);
            }
        }
    }
    public:
    ~thread_pool(){
        stop=true;
        pthread_cond_broadcast(&cond_work);
        for(int i=0;i<MAX_NUM;i++){
            pthread_join(pthread_num[i],NULL);
        }
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&cond_work);
    }
    public:
    void work(){
        while(true){
            pthread_mutex_lock(&mutex);
            function<void()> task;
            while(workqueue.empty()&&!stop){
                pthread_cond_wait(&cond_work,&mutex);
            }
            if(stop){
                    pthread_mutex_unlock(&mutex);
                    break;
            }
            task=std::move(workqueue.front());
            workqueue.pop();
            pthread_mutex_unlock(&mutex);
            task();
        }
        return ;
    }
    public:
    template<typename Func>
    auto push_task(Func func)->future<decltype(func())>{

        using ReturnType=decltype(func());

        auto task=make_shared<packaged_task<ReturnType()>>(std::forward<Func>(func));
        future<ReturnType> result=task->get_future();

        pthread_mutex_lock(&mutex);

        workqueue.push([task](){(*task)();});

        pthread_cond_signal(&cond_work);
        pthread_mutex_unlock(&mutex);
        return result;
    }
};

vector<int> cal_num(int n){
	vector<int> result(1,1);
	for(int i=2;i<=n;i++){
		int carry=0;
		for(int j=0;j<result.size();j++){
			int temp=result[j]*i+carry;
			result[j]=temp%10;
			carry=temp/10;
		}
		while(carry){
			result.push_back(carry%10);
			carry/=10;
		}
	}
	return result;
}

int main(){
    thread_pool hahaha;
    vector<vector<int>> result;
    future<vector<int>>result1 = hahaha.push_task([](){return cal_num(11100);});

    future<vector<int>>result2 = hahaha.push_task([](){return cal_num(11190);});

    future<vector<int>>result3 = hahaha.push_task([](){return cal_num(11188);});

    future<vector<int>>result4 = hahaha.push_task([](){return cal_num(11167);});

    future<vector<int>>result5 = hahaha.push_task([](){return cal_num(11187);});

    future<vector<int>>result6 = hahaha.push_task([](){return cal_num(11195);});

    future<vector<int>>result7 = hahaha.push_task([](){return cal_num(11166);});

    future<vector<int>>result8 = hahaha.push_task([](){return cal_num(11196);});

    future<vector<int>>result9 = hahaha.push_task([](){return cal_num(11179);});

    result.push_back(result1.get());
    result.push_back(result2.get());
    result.push_back(result3.get());
    result.push_back(result4.get());
    result.push_back(result5.get());
    result.push_back(result6.get());
    result.push_back(result7.get());
    result.push_back(result8.get());
    result.push_back(result9.get());

    for(int i=0;i<result.size();i++){
        reverse(result[i].begin(),result[i].end());
        for(int j : result[i]){
            cout<<j;
        }
        cout<<endl<<endl;
    }
    
    return 0;  
}
