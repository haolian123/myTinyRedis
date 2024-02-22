# 定义编译器
CC=g++
# 定义编译选项
CFLAGS=-g -std=c++11

# 定义源代码目录、构建目录和二进制文件目录
SRC_DIR=../src
BIN_DIR=../bin

# 确保二进制文件目录存在
$(shell mkdir -p $(BIN_DIR))

# 定义对象文件列表
OBJS=$(SRC_DIR)/RedisHelper.cpp $(SRC_DIR)/EpollManager.cpp $(SRC_DIR)/CommandParser.cpp $(SRC_DIR)/RedisServer.cpp $(SRC_DIR)/RedisClient.cpp $(SRC_DIR)/ParserFlyweightFactory.cpp \
$(SRC_DIR)/RedisValue/Parse.cpp $(SRC_DIR)/RedisValue/RedisValue.cpp
# 默认目标
all: server client

# 编译server
server: $(SRC_DIR)/server.cpp $(OBJS)
	$(CC) $(CFLAGS) $(SRC_DIR)/server.cpp $(OBJS) -o $(BIN_DIR)/server

# 编译client
client: $(SRC_DIR)/client.cpp $(OBJS)
	$(CC) $(CFLAGS) $(SRC_DIR)/client.cpp $(OBJS) -o $(BIN_DIR)/client

# 清理目标
clean:
	rm -f $(BIN_DIR)/server $(BIN_DIR)/client $(BIN_DIR)/*.o
