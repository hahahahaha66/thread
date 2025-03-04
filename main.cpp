#include <cstddef>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <pthread.h>
const int num=100000000;
const int Thread_num=8;
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
    // pthread_create(&right_thread,NULL,Quicksort,static_cast<void*>(iright));
    // pthread_join(left_thread,NULL);
    // pthread_join(right_thread,NULL);
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
// vector<vector<int>> CutArrays(vector<int>&arr,int j){
//     vector<vector<int>> result;
//     if (j <= 0 || arr.empty()) return result;
//     int n=arr.size();
//     int base_size=n/j;
//     int remainder=n%j;
//     int start=0;
//     for(int i=0;i<j;i++){
//         int end=start+base_size+(i<remainder?1:0);
//         end=min(end,n);
//         result.emplace_back(arr.begin()+start,arr.begin()+end);
//         start=end;
//     }
//     return result;
// }
int main(){
    vector<int> arr(num);
    GenerateRandomNumber(arr);
    ThreadArgs*mainArgs=new ThreadArgs(&arr,0,arr.size()-1);
    pthread_t main_thread;
    pthread_create(&main_thread, NULL, Quicksort, mainArgs);
    pthread_join(main_thread,NULL);
    delete(mainArgs);
    // int a=Thread_num;
    // while(a>=1){
    //     vector<vector<int>> subarray=CutArrays(arr,a);
    //     if (subarray.empty()) break; 
    //     vector<pthread_t> pthreadset(subarray.size());
    //     vector<ThreadArgs*> Args;
    //     for(int i=0;i<a;i++){
    //         Args.push_back(new ThreadArgs(&subarray[i],0,subarray[i].size()-1));
    //     }
    //     for(int i=0;i<a;i++){
    //         pthread_create(&pthreadset[i],NULL,Quicksort,Args[i]);
    //     }
    //     for(int i=0;i<a;i++){
    //         pthread_join(pthreadset[i],NULL);
    //         delete Args[i];
    //     }
    //     for(int o=1;o<subarray.size();o++){
    //         subarray.insert(subarray.end(),make_move_iterator(subarray[o].begin()),make_move_iterator(subarray[o].end()));
    //     }
    //     arr=subarray[0];
    //     a/=2;
    // }
    for(int i : arr){
        cout<<i<<" ";
    }
    cout<<endl;
    return 0;
}