#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <thread>

using namespace std;

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
	vector<int> result=cal_num(100);
	int start = result.size() - 1;
    for (int i = result.size() - 1; i >= 0; i--) {
        cout << result[i];
    }
    cout << endl;
	return 0;
}
