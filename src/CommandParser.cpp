#include "CommandParser.h"

// 静态成员变量的初始化
std::shared_ptr<RedisHelper> CommandParser::redisHelper = std::make_shared<RedisHelper>();

// SelectParser 
std::string SelectParser::parse(std::vector<std::string>& tokens) {
    if (tokens.size() < 2) {
        return "wrong number of arguments for SELECT.";
    }
    int index = 0;
    try {
        index = std::stoi(tokens[1]);
    } catch (std::invalid_argument const& e) {
        return tokens[1] + " is not a numeric type";
    }
    return redisHelper->select(index);
}

// SetParser 
std::string SetParser::parse(std::vector<std::string>& tokens) {
    if (tokens.size() < 3) {
        return "wrong number of arguments for SET.";
    }
    if (tokens.size() == 4) {
        if (tokens.back() == "NX") {
            return redisHelper->set(tokens[1], tokens[2], NX);
        } else if (tokens.back() == "XX") {
            return redisHelper->set(tokens[1], tokens[2], XX);
        }
    }
    return redisHelper->set(tokens[1], tokens[2]);
}

// SetnxParser 
std::string SetnxParser::parse(std::vector<std::string>& tokens) {
    if (tokens.size() < 3) {
        return "wrong number of arguments for SETNX.";
    }
    return redisHelper->setnx(tokens[1], tokens[2]);
}

// SetexParser 
std::string SetexParser::parse(std::vector<std::string>& tokens) {
    if (tokens.size() < 3) {
        return "wrong number of arguments for SETEX.";
    }
    return redisHelper->setex(tokens[1], tokens[2]);
}

// GetParser 
std::string GetParser::parse(std::vector<std::string>& tokens) {
    if (tokens.size() < 2) {
        return "wrong number of arguments for GET.";
    }
    return redisHelper->get(tokens[1]);
}

// KeysParser 
std::string KeysParser::parse(std::vector<std::string>& tokens) {
    return redisHelper->keys();
}

// DBSizeParser 
std::string DBSizeParser::parse(std::vector<std::string>& tokens) {
    return redisHelper->dbsize();
}

// ExistsParser 
std::string ExistsParser::parse(std::vector<std::string>& tokens) {
    if (tokens.size() < 2) {
        return "wrong number of arguments for EXISTS.";
    }
    tokens.erase(tokens.begin()); // 移除命令本身
    return redisHelper->exists(tokens);
}

// DelParser 
std::string DelParser::parse(std::vector<std::string>& tokens) {
    if (tokens.size() < 2) {
        return "wrong number of arguments for DEL.";
    }
    tokens.erase(tokens.begin()); // 移除命令本身
    return redisHelper->del(tokens);
}

// RenameParser 
std::string RenameParser::parse(std::vector<std::string>& tokens) {
    if (tokens.size() < 3) {
        return "wrong number of arguments for RENAME.";
    }
    return redisHelper->rename(tokens[1], tokens[2]);
}

// IncrParser 
std::string IncrParser::parse(std::vector<std::string>& tokens) {
    if (tokens.size() < 2) {
        return "wrong number of arguments for INCR.";
    }
    return redisHelper->incr(tokens[1]);
}

// IncrbyParser 
std::string IncrbyParser::parse(std::vector<std::string>& tokens) {
    if (tokens.size() < 3) {
        return "wrong number of arguments for INCRBY.";
    }
    int increment = 0;
    try {
        increment = std::stoi(tokens[2]);
    } catch (std::invalid_argument const& e) {
        return tokens[2] + " is not a numeric type";
    }
    return redisHelper->incrby(tokens[1], increment);
}

// IncrbyfloatParser 
std::string IncrbyfloatParser::parse(std::vector<std::string>& tokens) {
    if (tokens.size() < 3) {
        return "wrong number of arguments for INCRBYFLOAT.";
    }
    double increment = 0.0;
    try {
        increment = std::stod(tokens[2]);
    } catch (std::invalid_argument const& e) {
        return tokens[2] + " is not a numeric type";
    }
    return redisHelper->incrbyfloat(tokens[1], increment);
}

// DecrParser 
std::string DecrParser::parse(std::vector<std::string>& tokens) {
    if (tokens.size() < 2) {
        return "wrong number of arguments for DECR.";
    }
    return redisHelper->decr(tokens[1]);
}

// DecrbyParser 
std::string DecrbyParser::parse(std::vector<std::string>& tokens) {
    if (tokens.size() < 3) {
        return "wrong number of arguments for DECRBY.";
    }
    int decrement = 0;
    try {
        decrement = std::stoi(tokens[2]);
    } catch (std::invalid_argument const& e) {
        return tokens[2] + " is not a numeric type";
    }
    return redisHelper->decrby(tokens[1], decrement);
}

// MSetParser 
std::string MSetParser::parse(std::vector<std::string>& tokens) {
    if (tokens.size() < 3 || tokens.size() % 2 == 0) { // 需要成对的键值
        return "wrong number of arguments for MSET.";
    }
    tokens.erase(tokens.begin()); // 移除命令本身
    return redisHelper->mset(tokens);
}

// MGetParser 
std::string MGetParser::parse(std::vector<std::string>& tokens) {
    if (tokens.size() < 2) {
        return "wrong number of arguments for MGET.";
    }
    tokens.erase(tokens.begin()); // 移除命令本身
    return redisHelper->mget(tokens);
}

// StrlenParser 
std::string StrlenParser::parse(std::vector<std::string>& tokens) {
    if (tokens.size() < 2) {
        return "wrong number of arguments for STRLEN.";
    }
    return redisHelper->strlen(tokens[1]);
}

// AppendParser 
std::string AppendParser::parse(std::vector<std::string>& tokens) {
    if (tokens.size() < 3) {
        return "wrong number of arguments for APPEND.";
    }
    return redisHelper->append(tokens[1], tokens[2]);
}
