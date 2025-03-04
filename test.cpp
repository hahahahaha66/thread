#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <random>
#include <iostream>
#include <pthread.h>
#include <queue>
#include <unistd.h>

std::queue<int> buffer;
const int BUFFER_SIZE = 5;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_producer = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_consumer = PTHREAD_COND_INITIALIZER;

void* producer(void*) {
    for (int i = 1; i <= 10; ++i) {
        pthread_mutex_lock(&mutex);
        while (buffer.size() == BUFFER_SIZE) {
            pthread_cond_wait(&cond_producer, &mutex);
        }
        buffer.push(i);
        std::cout << "Produced: " << i << std::endl;
        pthread_cond_signal(&cond_consumer);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return nullptr;
}

void* consumer(void*) {
    for (int i = 1; i <= 10; ++i) {
        pthread_mutex_lock(&mutex);
        while (buffer.empty()) {
            pthread_cond_wait(&cond_consumer, &mutex);
        }
        int item = buffer.front();
        buffer.pop();
        std::cout << "Consumed: " << item << std::endl;
        pthread_cond_signal(&cond_producer);
        pthread_mutex_unlock(&mutex);
        sleep(2);
    }
    return nullptr;
}

int main() {
    pthread_t producer_thread, consumer_thread;
    pthread_create(&producer_thread, nullptr, producer, nullptr);
    pthread_create(&consumer_thread, nullptr, consumer, nullptr);
    pthread_join(producer_thread, nullptr);
    pthread_join(consumer_thread, nullptr);
    return 0;
}
