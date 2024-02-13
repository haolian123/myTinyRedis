#include <iostream>
#include <sys/socket.h> // 提供socket函数及数据结构
#include <netinet/in.h> // 定义数据结构sockaddr_in
#include <arpa/inet.h> // 提供IP地址转换函数
#include <unistd.h> // 提供close函数
#include <cstring> // 提供memset函数
#include <string>

#define DEFAULT_HOSTNAME "127.0.0.1"
#define DEFAULT_PORT 8888

class RedisClient{
private:
    int clientSocket;
    struct sockaddr_in server;
    std::string message;
    char serverMessage[2000];
    ssize_t recvSize;
    std::string hostName="";
    int port;
    std::string PORT="";
public:
    RedisClient(const std::string& Host, int port);
    RedisClient();
    void start();
};

RedisClient::RedisClient(): hostName(DEFAULT_HOSTNAME), port(DEFAULT_PORT){
    PORT=std::to_string(DEFAULT_PORT);
}

RedisClient::RedisClient(const std::string& Host, int port): hostName(Host), port(port){
    PORT=std::to_string(port);
}

void RedisClient::start(){
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cout << "Could not create socket" << std::endl;
        return;
    }

    server.sin_addr.s_addr = inet_addr(hostName.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if (connect(clientSocket, (struct sockaddr *)&server, sizeof(server)) < 0) {
        std::cout << "Connect error." << std::endl;
        return;
    }

    while(true){
        //发送数据
        std::cout << hostName << ":" << PORT << "> ";
        std::getline(std::cin, message);
        if (send(clientSocket, message.c_str(), message.size(), 0) < 0) {
            std::cout << "Send failed" << std::endl;
            break;
        }
        //接收服务器的数据
        if ((recvSize = recv(clientSocket, serverMessage, sizeof(serverMessage), 0)) == -1) {
            std::cout << "recv failed" << std::endl;
        }

        //添加结束字符
        serverMessage[recvSize] = '\0';
        std::cout << serverMessage << std::endl;
    }

    close(clientSocket);
}