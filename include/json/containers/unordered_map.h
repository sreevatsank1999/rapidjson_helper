#ifndef JSON_HELPER_CONTAINERS_UNORDERED_MAP_H
#define JSON_HELPER_CONTAINERS_UNORDERED_MAP_H

#include <unordered_map>
#include <cassert>

#include "json/json_helper.h"

#include "rapidjson/writer.h"
#include "rapidjson/document.h"

namespace json{

template<typename _Key, typename _Tp,
    typename _Hash = std::hash<_Key>,
    typename _Pred = std::equal_to<_Key>,
    typename _Alloc = std::allocator<std::pair<const _Key, _Tp>>>
class unordered_map : public std::unordered_map<_Key,_Tp,_Hash,_Pred,_Alloc>
{
public:

    template<typename... Args>
    unordered_map(Args&&... args);

    unordered_map(const rapidjson::Value &v);

    template<class T>
    void printJSON(rapidjson::Writer<T>& w) const;
    void loadJSON(const rapidjson::Value &v);
};


// ******** unordered_map.cpp ********* //

template<typename _Key, typename _Tp,typename _Hash,typename _Pred,typename _Alloc>
template<typename... Args>
unordered_map<_Key,_Tp,_Hash,_Pred,_Alloc>::unordered_map(Args&&... args) 
    : std::unordered_map<_Key,_Tp,_Hash,_Pred,_Alloc>(std::forward<Args>(args)...)
{}
template<typename _Key, typename _Tp,typename _Hash,typename _Pred,typename _Alloc>
unordered_map<_Key,_Tp,_Hash,_Pred,_Alloc>::unordered_map(const rapidjson::Value &v) 
    : std::unordered_map<_Key,_Tp,_Hash,_Pred,_Alloc>()
{
    loadJSON(v);
}

template<typename _Key, typename _Tp,typename _Hash,typename _Pred,typename _Alloc>
template <class T>
void unordered_map<_Key,_Tp,_Hash,_Pred,_Alloc>::printJSON(rapidjson::Writer<T>& w) const {
             
    w.StartArray();    
    for (auto& e : *this){        
        w.StartObject();
        json::writeJSON_kv(w,"Key",e.first);
        json::writeJSON_kv(w,"Value",e.second);
        w.EndObject();
    }   
    w.EndArray();
}

template<typename _Key, typename _Tp,typename _Hash,typename _Pred,typename _Alloc>
void unordered_map<_Key,_Tp,_Hash,_Pred,_Alloc>::loadJSON(const rapidjson::Value &v){

    assert(v.IsArray());       // Needs to be an Array 

    for(size_t i=0; i<v.Size(); i++)                this->emplace( json::readJSON_kv<_Key>(v[i],"Key"), json::readJSON_kv<_Tp>(v[i],"Value") );
}
}   // namespace json
#endif