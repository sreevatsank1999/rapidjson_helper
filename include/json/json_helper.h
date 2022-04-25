#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#include <cstdio>
#include <cassert>
#include <string>

#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/writer.h"
#include "rapidjson/reader.h"

#include "impl/type_traits.h"

namespace json {

template <typename obj_ty>
void writeJSON (const std::string &filename, const obj_ty& objT, const std::string& key = "");

template <typename obj_ty,class T>
void writeJSON_kv (rapidjson::Writer<T>& w, const std::string& key, const obj_ty& objT);

template <typename obj_ty,class T>
void writeJSON_value (rapidjson::Writer<T>& w, const obj_ty& objT);


template <typename obj_ty>
void loadJSON(const std::string &filename, obj_ty& objT, const std::string& key = "");

template <typename obj_ty>
void loadJSON_kv(const rapidjson::Value &v, const std::string& key, obj_ty& objT);

template <typename obj_ty>
void loadJSON_value(const rapidjson::Value &v, obj_ty& objT);



template <typename obj_ty>
void writeJSON (const std::string &filename, const obj_ty& objT, const std::string& key){
    
    std::FILE* jsonfile = std::fopen(filename.c_str(),"wb");
    if(jsonfile == NULL)        assert(false);      // File failed to open

    char write_buffer[64*BUFSIZ];
    rapidjson::FileWriteStream fws(jsonfile, write_buffer, sizeof(write_buffer));

    rapidjson::Writer<rapidjson::FileWriteStream> w(fws);
    
    if (key.empty())
        writeJSON_value(w,objT);
    else {
        w.StartObject();
       writeJSON_kv(w,key,objT);
        w.EndObject();
    }

    fclose(jsonfile);
}

template <typename obj_ty,class T>
void writeJSON_kv (rapidjson::Writer<T>& w, const std::string& key, const obj_ty& objT){

    w.Key(key.c_str());       writeJSON_value(w,objT);
}

template <typename obj_ty,class T>
void writeJSON_value (rapidjson::Writer<T>& w, const obj_ty& objT){

    if constexpr (!std::is_compound_v<typename std::decay_t<obj_ty>>) {
        if constexpr (std::is_floating_point_v<typename std::decay_t<obj_ty>>)                                               w.Double(objT);
        else if constexpr (std::is_same_v<typename std::decay_t<obj_ty>,bool>)                                               w.Bool(objT);
        else if constexpr (std::is_unsigned_v<typename std::decay_t<obj_ty>>)                                                w.Uint64(objT);
        else if constexpr (std::is_signed_v<typename std::decay_t<obj_ty>> && std::is_integral_v<typename std::decay_t<obj_ty>>)         w.Int64(objT);
        else if constexpr (std::is_same_v<char, typename std::decay_t<obj_ty>>)                                              w.String(objT);
    }           
    else {
        if constexpr (is_string<obj_ty>::value)
        {
                 if constexpr (std::is_same_v<std::string, typename std::decay_t<obj_ty>>)                                          w.String(objT.c_str(),objT.size());
            else if constexpr (std::is_same_v<std::string_view, typename std::decay_t<obj_ty>>)                                          w.String(objT.data(),objT.size());
            else if constexpr (is_char_array_v<obj_ty>)                                                                  w.String(objT,std::strlen(objT));
        }
        else                                                                                                         objT.printJSON(w);
    }
}


template <typename obj_ty>
void loadJSON(const std::string &filename, obj_ty& objT, const std::string& key){

    std::FILE* jsonfile = std::fopen(filename.c_str(), "rb");
    if(jsonfile == NULL)        assert(false);      // File failed to open

    static char read_buffer[64*BUFSIZ];
    rapidjson::FileReadStream json_stream(jsonfile,read_buffer,sizeof(read_buffer));        

    rapidjson::Document d;  d.ParseStream(json_stream);
    const rapidjson::Value& v = d;

    if(key.empty())     loadJSON_value(v, objT);
    else                loadJSON_kv(v, key, objT);

    fclose(jsonfile);
}
template <typename obj_ty>
void loadJSON_kv(const rapidjson::Value &v, const std::string& key, obj_ty& objT){

    loadJSON_value( v[key.c_str()], objT );
}
template <typename obj_ty>
void loadJSON_value(const rapidjson::Value &v, obj_ty& objT){

    if constexpr (!std::is_compound_v<typename std::decay_t<obj_ty>>) {
             if constexpr (std::is_floating_point_v<typename std::decay_t<obj_ty>>)                                          objT = v.GetDouble();
        else if constexpr (std::is_same_v<typename std::decay_t<obj_ty>,bool>)                                               objT = v.GetBool();
        else if constexpr (std::is_unsigned_v<typename std::decay_t<obj_ty>> && !std::is_same_v<typename std::decay_t<obj_ty>,bool>)         objT = v.GetUint64();
        else if constexpr (std::is_signed_v<typename std::decay_t<obj_ty>> && std::is_integral_v<typename std::decay_t<obj_ty>>)             objT = v.GetInt64();
        else if constexpr (std::is_same_v<char, typename std::decay_t<obj_ty>>)                                              objT = v.GetString();                       
    }           
    else {
        if constexpr (is_string<obj_ty>::value)
        {
                 if constexpr (std::is_same_v<std::string, typename std::decay_t<obj_ty>>)                                             objT = std::string(v.GetString());
            else if constexpr (std::is_same_v<std::string_view, typename std::decay_t<obj_ty>>)                                        objT = v.GetString();
            else if constexpr (is_char_array_v<obj_ty>)                                                                         std::strcpy(objT,v.GetString());
        }
        else                                                                                                            objT.loadJSON(v);
    }    
}


template <typename obj_ty>
decltype(auto) readJSON(const std::string &filename, const std::string& key = ""){

    std::FILE* jsonfile = std::fopen(filename.c_str(), "rb");
    if(jsonfile == NULL)        assert(false);      // File failed to open

    static char read_buffer[64*BUFSIZ];
    rapidjson::FileReadStream json_stream(jsonfile,read_buffer,sizeof(read_buffer));        

    rapidjson::Document d;  d.ParseStream(json_stream);
    const rapidjson::Value& v = d;

    decltype(auto) ret_val =  ( key.empty() ? readJSON_value<obj_ty>(v) : readJSON_kv<obj_ty>(v, key) ); 

    fclose(jsonfile);

    return ret_val;
}
template <typename obj_ty>
decltype(auto) readJSON_kv(const rapidjson::Value &v, const std::string& key){

    return readJSON_value<obj_ty>(v[key.c_str()]);
}
template <typename obj_ty>
decltype(auto) readJSON_value(const rapidjson::Value &v){

    if constexpr (!std::is_compound_v<typename std::decay_t<obj_ty>>) {
             if constexpr (std::is_floating_point_v<typename std::decay_t<obj_ty>>)                                          return v.GetDouble();
        else if constexpr (std::is_same_v<typename std::decay_t<obj_ty>,bool>)                                               return v.GetBool();
        else if constexpr (std::is_unsigned_v<typename std::decay_t<obj_ty>> && !std::is_same_v<typename std::decay_t<obj_ty>,bool>)         return v.GetUint64();
        else if constexpr (std::is_signed_v<typename std::decay_t<obj_ty>> && std::is_integral_v<typename std::decay_t<obj_ty>>)             return v.GetInt64();
        else if constexpr (std::is_same_v<char, typename std::decay_t<obj_ty>>)                                              return v.GetString();                       
    }           
    else {
        if constexpr (is_string<obj_ty>::value)
        {
                 if constexpr (std::is_same_v<std::string, typename std::decay_t<obj_ty>>)                                          return std::string(v.GetString());
            else if constexpr (std::is_same_v<std::string_view, typename std::decay_t<obj_ty>>)                                     return v.GetString();
            else if constexpr (is_char_array_v<obj_ty>)                                                                      return v.GetString();
        }
        else                                                                                                            return v;
    }    
}

}
#endif  