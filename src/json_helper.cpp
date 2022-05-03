#include <cstdio>
#include <cassert>
#include <string>

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

#include "json/json_helper.h"

namespace json
{

rapidjson::Document openJSON(const std::string &filename)
{    
    std::FILE* jsonfile = std::fopen(filename.c_str(), "rb");
    if(jsonfile == NULL)        assert(false);      // File failed to open

    static char read_buffer[64*BUFSIZ];
    rapidjson::FileReadStream json_stream(jsonfile,read_buffer,sizeof(read_buffer));        

    rapidjson::Document d;  d.ParseStream(json_stream);

    fclose(jsonfile);

    return std::move(d);
}

}