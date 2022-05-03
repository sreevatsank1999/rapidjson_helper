#ifndef JSON_HELPER_CONTAINERS_UNORDERED_MAP_H
#define JSON_HELPER_CONTAINERS_UNORDERED_MAP_H

#include <unordered_set>
#include <cassert>

#include "json/json_helper.h"

#include "rapidjson/writer.h"
#include "rapidjson/document.h"

namespace json{

template<typename _Key, 
    typename _Hash = std::hash<_Key>,
    typename _Pred = std::equal_to<_Key>,
    typename _Alloc = std::allocator<_Key>>
class unordered_set : public std::unordered_set<_Key,_Hash,_Pred,_Alloc>
{
public:

    template<typename... Args>
    unordered_set(Args&&... args);

    unordered_set(const rapidjson::Value &v);

    template<class T>
    void printJSON(rapidjson::Writer<T>& w) const;
    void loadJSON(const rapidjson::Value &v);
};


// ******** unordered_set.cpp ********* //

template<typename _Key, typename _Hash,typename _Pred,typename _Alloc>
template<typename... Args>
unordered_set<_Key,_Hash,_Pred,_Alloc>::unordered_set(Args&&... args) 
    : std::unordered_set<_Key,_Hash,_Pred,_Alloc>(std::forward<Args>(args)...)
{}
template<typename _Key, typename _Hash,typename _Pred,typename _Alloc>
unordered_set<_Key,_Hash,_Pred,_Alloc>::unordered_set(const rapidjson::Value &v) 
    : std::unordered_set<_Key,_Hash,_Pred,_Alloc>()
{
    loadJSON(v);
}

template<typename _Key, typename _Hash,typename _Pred,typename _Alloc>
template <class T>
void unordered_set<_Key,_Hash,_Pred,_Alloc>::printJSON(rapidjson::Writer<T>& w) const {
             
    w.StartArray();    
    for (auto& e : *this)       
        json::writeJSON_value(w,e);

    w.EndArray();
}

template<typename _Key, typename _Hash,typename _Pred,typename _Alloc>
void unordered_set<_Key,_Hash,_Pred,_Alloc>::loadJSON(const rapidjson::Value &v){

    assert(v.IsArray());       // Needs to be an Array 

    for(size_t i=0; i<v.Size(); i++)                this->emplace( json::readJSON_value<_Key>(v[i]) );
}
}   // namespace json
#endif