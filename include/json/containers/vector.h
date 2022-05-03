#ifndef JSON_HELPER_CONTAINERS_VECTOR_H
#define JSON_HELPER_CONTAINERS_VECTOR_H

#include <vector>
#include <string>
#include <cassert>

#include "json/json_helper.h"

#include "rapidjson/writer.h"
#include "rapidjson/document.h"

namespace json{

template <typename _Tp, typename _Alloc = std :: allocator < _Tp >>
class vector : public std::vector<_Tp,_Alloc>
{
public:

    template<typename... Args>
    vector(Args&&... args);

    vector(const rapidjson::Value &v);

    template<class T>
    void printJSON(rapidjson::Writer<T>& w) const;
    void loadJSON(const rapidjson::Value &v);
    void reloadJSON(const rapidjson::Value &v);
};

// ******** vector.cpp ********* //

template <typename _Tp, typename _Alloc>
template<typename... Args>
vector<_Tp,_Alloc>::vector(Args&&... args) 
    : std::vector<_Tp,_Alloc>(std::forward<Args>(args)...)
{}

template <typename _Tp, typename _Alloc>
vector<_Tp,_Alloc>::vector(const rapidjson::Value &v)
    : std::vector<_Tp,_Alloc>()
{
    loadJSON(v);
}

template <typename _Tp, typename _Alloc>
template <class T>
void vector<_Tp,_Alloc>::printJSON(rapidjson::Writer<T>& w) const {

    w.StartArray();    
    for (auto& e : *this)           json::writeJSON_value(w,e);
    w.EndArray();
}

template <typename _Tp, typename _Alloc>
void vector<_Tp,_Alloc>::loadJSON(const rapidjson::Value &v){

    assert(v.IsArray());       // Needs to be an Array 

    for(size_t i=0; i<v.Size(); i++)        this->emplace_back(json::readJSON_value<_Tp>(v[i]));
}

template <typename _Tp, typename _Alloc>
void vector<_Tp,_Alloc>::reloadJSON(const rapidjson::Value &v){

    assert(v.IsArray());       // Needs to be an Array 
    assert(v.Size() == this->size());       // Assert equal number of elements 

    for(size_t i=0; i<this->size(); i++)        json::loadJSON_value<_Tp>(v[i],*(this+i));
}


}   // namespace json
#endif