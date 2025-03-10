#include <iostream>
#include <vector>
#include <thread>
#include <dirent.h>
#include <filesystem>
#include <string>
#include <functional>
#include <mutex>
#include <atomic>
#include <set>
using namespace std;
namespace fs=std::filesystem;
std::mutex mtx;
std::atomic<int> now_pthrread=0;
atomic<int> now_depth=0;
set<string> result;
struct SearchConfig {
    string root_path;    // 要搜索的根目录
    string file_type;    // 要搜索的文件类型，如 ".txt"、".cpp" 等
    int max_concurrency;      // 最大并发数
    int max_depth;            // 最大搜索深度
    bool skip_hidden;         // 是否跳过隐藏文件或目录
    std::vector<std::string> skip_paths;   // 要跳过的目录或文件的路径
};

void search(string address,struct SearchConfig& config){
    if(now_depth.load()>=config.max_depth){
        return;
    }
    fs::path p=address;
    vector<thread> threads;
    try{
        for(auto& entry : fs::recursive_directory_iterator(p)){
            bool haha=true;
            for(int i=0;i<config.skip_paths.size();i++){
                if((entry.path().string().compare(config.skip_paths[i]))==0){
                    haha=false;
                    break;
                }
            }
        if(!haha){
            continue;
        }
        if(config.skip_hidden){
            if(entry.path().filename().string().rfind(".",0)==0){
                continue;
            }
        }
        if(entry.is_directory()&&config.max_concurrency>now_pthrread){
            now_depth.fetch_add(1);
            now_pthrread.fetch_add(1);
            threads.emplace_back([&,path=entry.path().string()]()
            {search(entry.path().string(),config);
            now_pthrread.fetch_sub(1);
            now_depth.fetch_sub(1);
            });
            continue;
        }
        if(entry.is_directory()){
            now_depth.fetch_add(1);
            continue;
        }
        string temp=entry.path().string();
        if(temp.size()>=4&&(temp.compare(temp.size()-4,4,config.file_type)==0)){
            mtx.lock();
            result.insert(temp);
            mtx.unlock();
        }
    }
    }
    catch(const fs::filesystem_error &e){
        cout<<"File error"<<endl;
        return;
    }catch (const std::exception &e) {
        cerr << "Standard exception: " << e.what() << endl;
    } catch (...) {
        cerr << "Unknown error occurred!" << endl;
    }
    for(auto&i : threads){
        i.join();
    }
}

int main(){
    struct SearchConfig config;
    config.root_path="/home/hahaha/work";
    config.file_type=".cpp";
    config.max_concurrency=12;
    config.max_depth=6;
    config.skip_hidden=true;
    config.skip_paths={};
    search(config.root_path,config);
    for(auto i : result){
        cout<<i<<endl;
    }
    return 0;
}