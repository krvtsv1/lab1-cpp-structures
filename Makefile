CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic
TARGET = dbms

SRCS = main.cpp \
       Array/Array.cpp \
       LinkedList/LinkedList.cpp \
       DoubleLinkedList/DoubleLinkedList.cpp \
       Stack/Stack.cpp \
       Queue/Queue.cpp \
       FullBinaryTree/FullBinaryTree.cpp \
       HashSet/HashSet.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
