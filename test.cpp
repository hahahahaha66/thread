#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <random>
using namespace std;
const int num=100;

vector<vector<int>> CutArrays(vector<int>&arr,int j){
    vector<vector<int>> result;
    int n=arr.size();
    int base_size=n/j;
    int remainder=n%j;
    int start=0;
    for(int i=0;i<j;i++){
        int end=start+base_size+(i<remainder?1:0);
        result.emplace_back(arr.begin()+start,arr.begin()+min<size_t>(end,n));
        start=end;
    }
    return result;
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
    vector<vector<int>> subarray=CutArrays(arr,10);
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            cout<<subarray[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
    return 0;
}