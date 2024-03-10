CXX = g++                         
CXXFLAGS = -std=c++11    
TARGET = test                     
SRCS = test.cpp                  
HEADERS = SkipList/SkipList.h LinkedList/LinkedList.h 

$(TARGET): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)
