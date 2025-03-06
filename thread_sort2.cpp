#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>
#include <random>
#include <algorithm>
#include <pthread.h>
const int num=100000000;
const int Thread_num=12;
using namespace std;
struct ThreadArgs{
    vector<int>*a;
    int left;
    int right;
    ThreadArgs(vector<int>* v,int left,int right) : a(v),left(left),right(right) {}
};
void* Quicksort(void*example){
    ThreadArgs*temp=static_cast<ThreadArgs*>(example);
    int left=temp->left,right=temp->right;
    vector<int>* array=temp->a;

    if(left>=right){
        delete temp;
        return nullptr;
    }
     
    int i=left-1;
    int pivot=(*array)[right];
    for(int j=left;j<right;j++){
        if((*array)[j]<=pivot){
            i++;
            swap((*array)[i],(*array)[j]);
        }
    }
    swap((*array)[i+1],(*array)[right]);
    int l=i+1;

    pthread_t left_thread, right_thread;
    bool left_thread_created=false, right_thread_created=false;

    if(right-left>num/Thread_num){
        ThreadArgs* ileft=new ThreadArgs(array,left,l-1);
        if(!pthread_create(&left_thread,NULL,Quicksort,ileft)){
            left_thread_created=true;
        }
        else{
            Quicksort(ileft);
        }
        ThreadArgs* iright=new ThreadArgs(array,l+1,right);
        if(!pthread_create(&right_thread,NULL,Quicksort,iright)){
            right_thread_created=true;
        }
        else{
            Quicksort(iright);
        }
    }
    else{
        ThreadArgs* ileft=new ThreadArgs(array,left,l-1);
        ThreadArgs* iright=new ThreadArgs(array,l+1,right);
        Quicksort(ileft);
        Quicksort(iright);
    }

    if(left_thread_created){
        pthread_join(left_thread,NULL);
    }
    if(right_thread_created){
        pthread_join(right_thread,NULL);
    }

    delete temp;
    return nullptr;
}

void GenerateRandomNumber(vector<int>&arr){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(1,num*10);
    for(int& i : arr){
        i=dis(gen);
    }
}

vector<vector<int>> cut(vector<int>&arr){
    vector<vector<int>> result(Thread_num);
    size_t m=num/Thread_num;
    size_t extra=num%Thread_num;
    size_t start=0;
    for(int i=0;i<Thread_num;i++){
        int end=start+m+(i<extra?1:0);
        result[i]=vector<int>(arr.begin()+start,arr.begin()+end);
        start=end;
    }
    return result;
}

int main(){
    vector<int> arr(num);
    vector<vector<int>> cut_arr(Thread_num);
    GenerateRandomNumber(arr);
    cut_arr=cut(arr);
    vector<pthread_t> threads(Thread_num);
    for(int i=0;i<Thread_num;i++){
        ThreadArgs*mainArgs=new ThreadArgs(&cut_arr[i],0,cut_arr[i].size()-1);
        pthread_create(&threads[i], NULL, Quicksort, mainArgs);
        // pthread_join(main_thread,NULL);
    }
    for(int i=0;i<Thread_num;i++){
        pthread_join(threads[i],NULL);
    }
    vector<int>array=cut_arr[0];
    for(int j=1;j<Thread_num;j++){
        vector<int> temp(array.size()+cut_arr[j].size());
        merge(array.begin(),array.end(),cut_arr[j].begin(),cut_arr[j].end(),temp.begin());
        array=std::move(temp);
    }
    // for(int i : array){
    //     cout<<i<<" ";
    // }
    // cout<<endl;
    return 0;
}