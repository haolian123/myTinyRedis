#ifndef PARSER_FLYWEIGHT_FACTORY
#define PARSER_FLYWEIGHT_FACTORY
#include"CommandParser.h"
#include<unordered_map>
//享元模式工厂

class ParserFlyweightFactory{
private:
    std::unordered_map<std::string,std::shared_ptr<CommandParser>> parserMaps;
    std::shared_ptr<CommandParser> createCommandParser(std::string& command);
public:
    std::shared_ptr<CommandParser> getParser(std::string& command);
};

#endif