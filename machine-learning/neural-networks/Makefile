SHELL = bash 

ifeq ($(OS),Windows_NT)
    RM = del /Q
    TARGET_EXT = .exe
else
    RM = rm -f
    TARGET_EXT =
endif

CXX = g++
CXXFLAGS = -Wall -g
TARGET = main
SRCS = main.cpp value.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	$(TARGET)$(TARGET_EXT)

clean:
	$(RM) $(TARGET)$(TARGET_EXT) $(OBJS)

.PHONY: all run clean
