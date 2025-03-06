#include <cstddef>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <pthread.h>
const int num=101;
const int Thread_num=5;
using namespace std;

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
    size_t start=-0;
    for(int i=0;i<Thread_num;i++){
        int end=start+m+(i<extra?1:0);
        result[i]=vector<int>(arr.begin()+start,arr.begin()+end);
        start=end;
    }
    return result;
}

int main(){
    vector<int> arr(num);
    GenerateRandomNumber(arr);
    auto cut_arr=cut(arr);
    for(int i=0;i<Thread_num;i++){
        for(int j : cut_arr[i]){
            cout<<j<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
    return 0;
}