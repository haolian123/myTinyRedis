#include"RedisServer.h"



std::shared_ptr<RedisServer> RedisServer::getInstance() {
        static std::shared_ptr<RedisServer> instance(new RedisServer());
        return instance;
    }

void RedisServer::createSocket() {
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        std::cerr << "Could not create socket" << std::endl;
    }
}

void RedisServer::initServer() {
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    memset(&client, 0, sizeof(client)); 
}

bool RedisServer::bindSocket() {
    if (bind(serverSocket, (struct sockaddr *)&server, sizeof(server)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return false;
    }
    return true;
}

void RedisServer::printLogo() {
    std::ifstream ifs(logoFilePath);
    if(!ifs.is_open()){
        std::cout<<"logoFilePath不存在"<<std::endl;
    }
    std::string line = "";
    while (std::getline(ifs, line)) {
        replaceText(line, "PORT", std::to_string(port));
        replaceText(line, "PID", std::to_string(pid));
        std::cout << line << std::endl;
    }
}

void RedisServer::printStartMessage() {
    std::string startMessage = "[PID] DATE # Server started.";
    std::string initMessage = "[PID] DATE * The server is now ready to accept connections on port PORT";
    
    replaceText(startMessage, "PID", std::to_string(pid));
    replaceText(startMessage, "DATE", getDate());
    replaceText(initMessage, "PORT", std::to_string(port));
    replaceText(initMessage, "PID", std::to_string(pid));
    replaceText(initMessage, "DATE", getDate());
    
    std::cout << startMessage << std::endl;
    std::cout << initMessage << std::endl;
}

void RedisServer::start() {
    signal(SIGINT, signalHandler); // Set signal handler for SIGINT
    createSocket();
    initServer();
    if (!bindSocket()) {
        return;
    }

    listen(serverSocket, 3); // Start listening for connections

    printLogo();
    printStartMessage();

    while (!stop) {
        c = sizeof(struct sockaddr_in);
        newSocket = accept(serverSocket, (struct sockaddr *)&client, &c);
        if (newSocket < 0) {
            std::cerr << "Accept failed" << std::endl;
            continue;
        }

        std::thread(&RedisServer::handleClient, this, newSocket).detach();
    }
}

void RedisServer::handleClient(int clientSocket) {
    ParserFlyweightFactory flyweightFactory;
    char buffer[2048];
    std::string receivedData;
    ssize_t bytesRead;

    while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytesRead] = '\0'; 
        receivedData.assign(buffer, bytesRead);

        // 假定接收的数据是命令行格式，用空格分隔命令和参数
        std::istringstream iss(receivedData);
        std::string command;
        std::vector<std::string> tokens;
        while (iss >> command) {
            tokens.push_back(command);
        }

        // 假定第一个token是命令，其余的是参数
        if (!tokens.empty()) {
            command = tokens.front();

            std::shared_ptr<CommandParser> commandParser = flyweightFactory.getParser(command);
            std::string responseMessage;

            if (commandParser == nullptr) {
                responseMessage = "Error: Command '" + command + "' not recognized.";
            } else {
                try {
                    responseMessage = commandParser->parse(tokens);
                } catch (const std::exception& e) {
                    responseMessage = "Error processing command '" + command + "': " + e.what();
                }
            }

            // 发送响应消息回客户端
            send(clientSocket, responseMessage.c_str(), responseMessage.length(), 0);
        } else {
            std::string errorMessage = "Error: No command received.";
            send(clientSocket, errorMessage.c_str(), errorMessage.length(), 0);
        }
    }

    close(clientSocket);
}


void RedisServer::replaceText(std::string &text, const std::string &toReplaceText, const std::string &newText) {
    size_t start_pos = text.find(toReplaceText);
    while (start_pos != std::string::npos) {
        text.replace(start_pos, toReplaceText.length(), newText);
        start_pos = text.find(toReplaceText, start_pos + newText.length());
    }
}

std::string RedisServer::getDate() {
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);

    std::tm local_tm;
    localtime_r(&now_c, &local_tm); 

    std::ostringstream oss;
    oss << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void RedisServer::signalHandler(int sig) {
    if (sig == SIGINT) {
        CommandParser::getRedisHelper()->flush();
        exit(0);
    }
}

