#CC = clang++
#CFLAGS = -Wall -Wextra -O3 -march=native -flto -funroll-loops -std=c++20 -stdlib=libc++
#SRC_DIR = src
#BUILD_DIR = build


#SRCS = $(wildcard $(SRC_DIR)/*.cpp)
#OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))


#TARGET = $(BUILD_DIR)/engine1


#all: $(TARGET)


#$(TARGET): $(OBJS)
#	$(CC) $(CFLAGS) -o $@ $^


#$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
#	mkdir -p $(BUILD_DIR)
#	$(CC) $(CFLAGS) -c $< -o $@


#clean:
#	rm -rf $(BUILD_DIR)/*.o $(TARGET)





CC = clang++
CFLAGS = -Wall -Wextra -O3 -march=native -flto -funroll-loops -std=c++20 -stdlib=libc++
SRC_DIR = src
BUILD_DIR = build

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
TARGET = $(BUILD_DIR)/engine1

all: $(TARGET)

$(TARGET): $(SRCS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(TARGET)
