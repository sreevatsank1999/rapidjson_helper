#ifndef JSON_HELPER_CONTAINERS_COLLECTION_H
#define JSON_HELPER_CONTAINERS_COLLECTION_H

#include <functional>
#include <initializer_list>
#include <cassert>

#include "rapidjson/document.h"

#include "json/json_helper.h"

#include "json/containers/vector.h"
#include "json/containers/table.h"


namespace json{

template <typename _Elem, typename _idx_ty = typename _Elem::pk_type, typename _Cont = table<_Elem,_idx_ty>>
class collection {
public:

    collection() = default;

    template <typename ...Args>
    collection(Args... more,_Cont& cont_n);

    bool contains(const _idx_ty& indx) const;
    const _Elem& get(const _idx_ty& indx) const;

    void add(_Cont &new_obj);

    template<class T>
    void printJSON(rapidjson::Writer<T>& w) const ;
    // void loadJSON(rapidjson::Value &v);

protected:
    vector<std::reference_wrapper<_Cont>> C;
};


/************ collection.cpp ***************/

template <typename _Elem, typename _idx_ty, typename _Cont>
template <typename ...Args>
collection<_Elem,_idx_ty,_Cont>::collection(Args... more, _Cont& cont_n)
    : collection(more...)
{
    this->add(cont_n);
}

template <typename _Elem, typename _idx_ty, typename _Cont>
template <typename T>
void collection<_Elem,_idx_ty,_Cont>::printJSON(rapidjson::Writer<T>& w) const {
    
    for (const _Cont& c : C)        static_cast<_Cont&>(c).printJSON(w);

}

template <typename _Elem, typename _idx_ty, typename _Cont>
void collection<_Elem,_idx_ty,_Cont>::add(_Cont& new_obj) {

    C.emplace_back(new_obj);
}

template <typename _Elem, typename _idx_ty, typename _Cont>
bool collection<_Elem,_idx_ty,_Cont>::contains(const _idx_ty& indx) const {
    
    for (const _Cont& c : C)
        if (c.contains(indx))           return true;

    return false;        
}

template <typename _Elem, typename _idx_ty, typename _Cont>
const _Elem& collection<_Elem,_idx_ty,_Cont>::get(const _idx_ty& indx) const {
    
    for (const _Cont& c : C)
        if (c.contains(indx))           return static_cast<const _Cont&>(c).at(indx);

    assert(false);        
}

}
#endif