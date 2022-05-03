#ifndef JSON_HELPER_CONTAINERS_TABLE_H
#define JSON_HELPER_CONTAINERS_TABLE_H

#include <unordered_map>
#include <cassert>

#include "json/json_helper.h"

#include "rapidjson/writer.h"
#include "rapidjson/document.h"

namespace json{

template<typename _Tp, typename _Key = typename _Tp::pk_type,
    typename _Hash = std::hash<_Key>,
    typename _Pred = std::equal_to<_Key>,
    typename _Alloc = std::allocator<std::pair<const _Key, _Tp>>>
class table : public std::unordered_map<_Key,_Tp,_Hash,_Pred,_Alloc>
{
public:

    template<typename... Args>
    table(Args&&... args);

    table(const rapidjson::Value &v);

    template<class T>
    void printJSON(rapidjson::Writer<T>& w) const;
    void loadJSON(const rapidjson::Value &v);
};


// ******** table.cpp ********* //

template<typename _Tp, typename _Key,typename _Hash,typename _Pred,typename _Alloc>
template<typename... Args>
table<_Tp,_Key,_Hash,_Pred,_Alloc>::table(Args&&... args) 
    : std::unordered_map<_Key,_Tp,_Hash,_Pred,_Alloc>(std::forward<Args>(args)...)
{}
template<typename _Tp, typename _Key,typename _Hash,typename _Pred,typename _Alloc>
table<_Tp,_Key,_Hash,_Pred,_Alloc>::table(const rapidjson::Value &v) 
    : std::unordered_map<_Key,_Tp,_Hash,_Pred,_Alloc>()
{
    loadJSON(v);
}

template<typename _Tp, typename _Key,typename _Hash,typename _Pred,typename _Alloc>
template <class T>
void table<_Tp,_Key,_Hash,_Pred,_Alloc>::printJSON(rapidjson::Writer<T>& w) const {
             
    w.StartArray();    
    for (auto& e : *this)        
        json::writeJSON_value(w,e.second);
        
    w.EndArray();
}

template<typename _Tp, typename _Key,typename _Hash,typename _Pred,typename _Alloc>
void table<_Tp,_Key,_Hash,_Pred,_Alloc>::loadJSON(const rapidjson::Value &v){

    assert(v.IsArray());       // Needs to be an Array 

    for(size_t i=0; i<v.Size(); i++)                this->emplace( json::readJSON_kv<_Key>(v[i], _Tp::primary_key), json::readJSON_value<_Tp>(v[i]) );
}
}   // namespace json
#endif