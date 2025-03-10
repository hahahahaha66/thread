#include <iostream>
#include <vector>
#include <thread>
#include <dirent.h>
#include <filesystem>
#include <string>
using namespace std;
namespace fs=std::filesystem;
int now_pthrread=0;
struct SearchConfig {
    string root_path;    // 要搜索的根目录
    string file_type;    // 要搜索的文件类型，如 ".txt"、".cpp" 等
    int max_concurrency;      // 最大并发数
    int max_depth;            // 最大搜索深度
    bool skip_hidden;         // 是否跳过隐藏文件或目录
    std::vector<std::string> skip_paths;   // 要跳过的目录或文件的路径
};
void search(string address,struct SearchConfig config){
    fs::path p=address;
    for(auto& entry : fs::recursive_directory_iterator(p)){
        if(entry.is_directory()&&config.max_concurrency>now_pthrread){
            thread newthread(search,entry.path(),config);
            
        }
    }
}
int main(){
    struct SearchConfig config;
    config.root_path="/home/hahaha/";
    config.file_type=".cpp";
    config.max_concurrency=12;
    config.max_depth=6;
    config.skip_hidden=false;
    config.skip_hidden={};
    
    return 0;
}