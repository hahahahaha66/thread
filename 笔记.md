#### 线程
与进程类似，但开销更小，可以共享全局内存

#### 创建线程
    #include<pthread.h>
    int pthread_create(pthread_t*thread,const pthread_attr_t *attr,void *(*start)(void*),void*arg);
thread是线程号，与进程类似，区分不同的线程
attr可以指定新线程的各种属性，通常设为NULL
start,函数指针，注意参数及返回值都是void*
arg函数参数
注意返回值为0是创建成功

#### 终止线程
通常有四种方式终止线程
+ start函数执行完返回
+ 线程调用pthread_exit()
+ 调用othread_cancal()
+ 任意线程调用exit或主线程调用return
    #inlcude<pthread.h>
    void pthread_exit(void*retval)
reval获取线程返回值

#### 获取线程号
线程号区分每个不同的线程，一个线程可以自己调用pthread_self来获取自己的线程号

#### 连接线程
    #include<pthread.h>
    int pthread_join(pthread_t thread,void **retval)
retval保存进程返回值
类似于进程中wait调用，回收线程
 
#### 线程的分离
    #include<pthread.h>
    int pthread_detach(pthread_t thread)
在调用*pthread_detach*后线程会进入分离状态，无法使用join获取状态，但在结束后会自动回收

### 线程同步

#### 互斥量
互斥量是属于*othread_mutex_t*类型的变量，使用前初始化
可以使用PTHREAD_MUTEX_INITIALIZER 赋给互斥量，使之静态初始化
同时也可以使用pthread_mutex_init来进行初始化，可以指定不同的类型
    #include<pthread.h>
    int pthread_mutex_lock(pthread_mutex_t *mutex);
    int pthread_mutex_unlock(pthread_mutex_t *mutex);
可以使用这两个函数来进行加锁和解锁，以防止竟态的出现，同时滥用加锁也会造成死锁

#### 条件变量
条件变量类型是pthread_cond_t,同样需要初始化，依旧是可以通过PTHREAD_COND_INITALIZER，和*pthread_cond_init(pthread_cond_t *cond,const pthread_condattr_t *attr)进行初始化
    #include<pthread.h>
    int pthread_cond_signal(pthread_cond_t *cond);
    int pthread_cond_boardcast(pthread_cond_t *cond);
    int pthread_cond_wait(pthread_cond_t*cond,pthread_mutex_t*mutex);
boardcast在发送信号后会唤醒所有阻塞的线程，signal只保证唤醒至少一条线程

#### 特殊注意
线程安全函数
一次性初始化