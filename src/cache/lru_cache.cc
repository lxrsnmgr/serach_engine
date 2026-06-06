#include "../include/cache/lru_cache.h"
#include <iostream>

namespace simple{
LruCache::LruCache(int cache_size)
: _cache_size(cache_size)
{

}

void LruCache::ReadFromFile(const string &file){

}

void LruCache::AddRecord(const string &key, const string& result){
    auto it = _hash_map.find(key);
    if(it != _hash_map.end()){
        MoveToFront(key);
        _result_list.begin()->second = result;
    } else {
        if(_result_list.size() == _cache_size){
            DeleteRear();
        }
        AddFront(key, result);
        AddToPending(key, result);
    }
}

void LruCache::DeleteRear(){
    string key = _result_list.back().first;
    _result_list.pop_back();
    _hash_map.erase(key);
}

void LruCache::AddFront(const string& key, const string &value){
    _result_list.emplace_front(make_pair(key, value));
    _hash_map.emplace(key, _result_list.begin());
}

void LruCache::MoveToFront(const string &key){
    auto it = _hash_map[key];
    pair<string, string> tmp = *it;
    _result_list.erase(it);
    _result_list.emplace_front(tmp);
    _hash_map[key] = _result_list.begin();
}

void LruCache::AddToPending(const string &key, const string &value){
    if(_pending_update_list.size() == _cache_size){
        _pending_update_list.pop_back();
    }
    _pending_update_list.emplace_front(make_pair(key, value));
}

void LruCache::Update(const LruCache&){

}

void LruCache::LruTest(){
    for(auto &elem:_result_list){
        std::cout << elem.first << " " << elem.second << "\n";
    }
    std::cout << "\n";
    for(auto &elem:_hash_map){
        std::cout << elem.first << " " << elem.second->first << "\n";
    }
    std::cout << "\n\n\n";
}
}
