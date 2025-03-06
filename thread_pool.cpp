#include <cmath>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <vector>
#include <queue>
#include <functional>
using namespace std;
const int MAX_NUM=10;
queue<function<void()>> workqueue;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond_work=PTHREAD_COND_INITIALIZER;
// pthread_cond_t  cond_finish=PTHREAD_COND_INITIALIZER;

class thread_pool{
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t  cond_work=PTHREAD_COND_INITIALIZER;
    public:
    thread_pool(){
        vector<int> working(MAX_NUM);
        vector<pthread_t> pthread_num(MAX_NUM);
        stop=false;
        for(int i=0;i<MAX_NUM;i++){
            pthread_create(&pthread_num[i],NULL,
            [](void*arg)->void*{static_cast<thread_pool*>(arg)->work();
            return NULL;},this);
        }
    }
    public:
    ~thread_pool(){
        stop=true;
        pthread_cond_broadcast(&cond_work);
        for(int i=0;i<MAX_NUM;i++){
            pthread_join(pthread_num[i],NULL);
        }
    }
    public:
    void work(){
        while(!stop){
            pthread_mutex_lock(&mutex);
            while(workqueue.empty()){
                pthread_cond_wait(&cond_work,&mutex);
            }
            function<void()> haha=workqueue.front();
            workqueue.pop();
            pthread_mutex_unlock(&mutex);
            haha();
        }
        return ;
    }
    public:
    bool stop;
    vector<int> working;
    vector<pthread_t> pthread_num;
};

void push_task(function<void()> task){
    pthread_mutex_lock(&mutex);
    workqueue.push(task);
    pthread_cond_signal(&cond_work);
    pthread_mutex_unlock(&mutex);
}

void test(){
    cout<<"helloworld"<<endl;
}
int main(){
    thread_pool hahaha;
    push_task(test);
    push_task(test);
    push_task(test);
    push_task(test);
    push_task(test);
    push_task(test);
    push_task(test);
    push_task(test);
    push_task(test);
    push_task(test);
    push_task(test);
    push_task(test);
    push_task(test);
    push_task(test);
    return 0;
}
