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
    signal(SIGINT, signalHandler);
    createSocket();
    initServer();
    if (!bindSocket()) {
        return;
    }

    if (listen(serverSocket, 3) < 0) {
        std::cerr << "Listen failed" << std::endl;
        return;
    }
    setFdNoBlock(serverSocket); // 设置监听socket为非阻塞模式

    if (!epollManager->addFd(serverSocket, EPOLLIN | EPOLLET)) { // 添加监听socket到epoll，注意使用边缘触发模式
        std::cerr << "Add listen socket to epoll failed" << std::endl;
        return;
    }

    printLogo();
    printStartMessage();

    while (!stop) {
        int eventCount = epollManager->wait(-1); // 无限期等待事件
        for (int i = 0; i < eventCount; ++i) {
            int fd = epollManager->getEventFd(i);
            if (fd == serverSocket) {
                // 处理新的连接
                while (true) {
                    struct sockaddr_in client;
                    socklen_t clientLen = sizeof(client);
                    int newSocket = accept(serverSocket, (struct sockaddr*)&client, &clientLen);
                    if (newSocket < 0) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) {
                            // 没有更多的连接可以接受
                            break;
                        } else {
                            std::cerr << "Accept failed" << std::endl;
                            continue;
                        }
                    }
                    setFdNoBlock(newSocket); // 设置新socket为非阻塞模式
                    // 为每个客户端连接创建一个新线程来处理请求
                    std::thread(&RedisServer::handleClient, this, newSocket).detach();
                }
            }
        }
    }
}


void RedisServer::handleClient(int clientSocket) {
    ParserFlyweightFactory flyweightFactory;

    // 持续监听来自客户端的请求
    while (!stop) { // 假设stop是一个能够控制服务器停止的全局变量
        char buffer[2048];
        std::string receivedData;
        ssize_t bytesRead;

        // 非阻塞接收数据
        bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';
            receivedData.assign(buffer, bytesRead);

            std::istringstream iss(receivedData);
            std::string command;
            std::vector<std::string> tokens;
            while (iss >> command) {
                tokens.push_back(command);
            }

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
            }
        } else if (bytesRead == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
            // 在非阻塞模式下，没有数据可读时继续循环
            continue;
        } else {
            // 客户端断开连接或发生错误
            break;
        }
    }

    close(clientSocket); // 客户端断开连接或发生错误后关闭socket
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


int RedisServer::setFdNoBlock(int fd){
    assert(fd > 0);
    return fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0) | O_NONBLOCK);
}

RedisServer::RedisServer(int port, const std::string& logoFilePath) 
: port(port), logoFilePath(logoFilePath),epollManager(new EpollManager()){
    pid = getpid();
}
