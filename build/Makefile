# 定义编译器
CC=g++
# 定义编译选项
CFLAGS=-g -std=c++11

# 定义源代码目录、构建目录和二进制文件目录
SRC_DIR=../src
BUILD_DIR=.
BIN_DIR=../bin

# 确保二进制文件目录存在
$(shell mkdir -p $(BIN_DIR))

# 定义对象文件列表
OBJS=$(BUILD_DIR)/RedisHelper.o

# 默认目标
all: server client

# 编译RedisHelper为对象文件
$(BUILD_DIR)/RedisHelper.o: $(SRC_DIR)/RedisHelper.cpp $(SRC_DIR)/RedisHelper.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/RedisHelper.cpp -o $(BUILD_DIR)/RedisHelper.o

# 编译server
server: $(SRC_DIR)/server.cpp $(SRC_DIR)/RedisServer.h $(OBJS)
	$(CC) $(CFLAGS) $(SRC_DIR)/server.cpp $(OBJS) -o $(BIN_DIR)/server

# 编译client
client: $(SRC_DIR)/client.cpp $(SRC_DIR)/RedisClient.h $(OBJS)
	$(CC) $(CFLAGS) $(SRC_DIR)/client.cpp $(OBJS) -o $(BIN_DIR)/client

# 清理目标
clean:
	rm -f $(BIN_DIR)/server $(BIN_DIR)/client $(OBJS)
