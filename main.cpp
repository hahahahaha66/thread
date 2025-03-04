#include <cstddef>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <pthread.h>
const int num=100000000;
const int Thread_num=5;
using namespace std;
struct ThreadArgs{
    vector<int>*a;
    int left;
    int right;
    ThreadArgs(vector<int>* v,int left,int right) : a(v),left(left),right(right) {}
};
void* Quicksort(void*example){
    ThreadArgs*temp=static_cast<ThreadArgs*>(example);
    int left=temp->left;
    int right=temp->right;
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
int main(){
    vector<int> arr(num);
    GenerateRandomNumber(arr);
    ThreadArgs*mainArgs=new ThreadArgs(&arr,0,arr.size()-1);
    pthread_t main_thread;
    pthread_create(&main_thread, NULL, Quicksort, mainArgs);
    pthread_join(main_thread,NULL);
    delete(mainArgs);
    
    for(int i : arr){
        cout<<i<<" ";
    }

    cout<<endl;
    return 0;
}