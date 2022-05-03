#ifndef JSON_HELPER_CONTAINERS_DATABASE_H
#define JSON_HELPER_CONTAINERS_DATABASE_H

#include "json/containers/collection.h"
#include "json/containers/table.h"


namespace json{

template <typename T>
using Database = table<T>;

template <typename T>
using DatabaseCollection = collection<T,typename T::pk_type,Database<T>>;

}
#endif