
#ifndef GLOBAL
#define GLOBAL
#include<iostream>
#include<unordered_map>
#include<sstream>
enum SET_MODEL{
    NONE,NX,XX
};

enum Command{
    SET,
    SETNX,
    SETEX,
    GET,
    SELECT,
    DBSIZE,
    EXISTS,
    DEL,
    RENAME,
    INCR,
    INCRBY,
    INCRBYFLOAT,
    DECR,
    DECRBY,
    MSET,
    MGET,
    STRLEN,
    APPEND,
    KEYS,
    INVALID_COMMAND
};

static std::unordered_map<std::string,enum Command>commandMaps={
    {"set",SET},
    {"setnx",SETNX},
    {"setex",SETEX},
    {"get",GET},
    {"select",SELECT},
    {"dbsize",DBSIZE},
    {"exists",EXISTS},
    {"del",DEL},
    {"rename",RENAME},
    {"incr",INCR},
    {"incrby",INCRBY},
    {"incrbyfloat",INCRBYFLOAT},
    {"decr",DECR},
    {"decrby",DECRBY},
    {"mset",MSET},
    {"mget",MGET},
    {"strlen",STRLEN},
    {"append",APPEND},
    {"keys",KEYS},
    
};



static std::vector<std::string> split(const std::string &s, char delimiter=' ') {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}







#endif