#ifndef PARSER_FLYWEIGHT_FACTORY
#define PARSER_FLYWEIGHT_FACTORY
#include"CommandParse.h"
#include<unordered_map>
//享元模式工厂

class ParserFlyweightFactory{
private:
    std::unordered_map<std::string,std::shared_ptr<CommandParser>> parserMaps;
    std::shared_ptr<CommandParser> createCommandParser(std::string& command);
public:
    std::shared_ptr<CommandParser> getParser(std::string& command);
};

std::shared_ptr<CommandParser> ParserFlyweightFactory::getParser(std::string& command){

    if(parserMaps.find(command)!=parserMaps.end()){
        return parserMaps[command];
    }
    return createCommandParser(command);
}



std::shared_ptr<CommandParser> ParserFlyweightFactory::createCommandParser(std::string& command){
    Command op;
    if(commandMaps.find(command)==commandMaps.end()){
        op=INVALID_COMMAND;
    }else{
        op=commandMaps[command];
    }
    
    switch(op){
        case SET:{
            parserMaps[command]=std::make_shared<SetParser>();
            break;
        }
        case SETNX:{
            parserMaps[command]=std::make_shared<SetnxParser>();
            break;
        }
        case SETEX:{
            parserMaps[command]=std::make_shared<SetexParser>();
            break;
        }
        case DBSIZE:{
            parserMaps[command]=std::make_shared<DBSizeParser>();
            break;
        }
        case GET:{
            parserMaps[command]=std::make_shared<GetParser>();
            break;
        }
        case KEYS:{
            parserMaps[command]=std::make_shared<KeysParser>();
            break;
        }
        case EXISTS:{
            parserMaps[command]=std::make_shared<ExistsParser>();
            break;
        }
        case DEL:{
            parserMaps[command]=std::make_shared<DelParser>();
            break;
        }
        case RENAME:{
            parserMaps[command]=std::make_shared<RenameParser>();
            break;
        }
        case INCR:{
            parserMaps[command]=std::make_shared<IncrParser>();
            break;
        }
        case INCRBY:{
            parserMaps[command]=std::make_shared<IncrbyParser>();
            break;
        }
        case INCRBYFLOAT:{
            parserMaps[command]=std::make_shared<IncrbyfloatParser>();
            break;
        }
        case DECR:{
            parserMaps[command]=std::make_shared<DecrParser>();
            break;
        }
        case DECRBY:{
            parserMaps[command]=std::make_shared<DecrbyParser>();
            break;
        }
        case MSET:{
            parserMaps[command]=std::make_shared<MSetParser>();
            break;
        }
        case MGET:{
            parserMaps[command]=std::make_shared<MGetParser>();
            break;
        }
        case STRLEN:{
            parserMaps[command]=std::make_shared<StrlenParser>();
            break;
        }
        case APPEND:{
            parserMaps[command]=std::make_shared<AppendParser>();
            break;
        }
        case SELECT:{
            parserMaps[command]=std::make_shared<SelectParser>();
            break;
        }
        default:{
            return nullptr;
        }
    }
    return parserMaps[command];
}
#endif