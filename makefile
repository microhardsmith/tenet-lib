# 检测操作系统类型
ifeq ($(OS),Windows_NT)
    PLATFORM = windows
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        PLATFORM = linux
    endif
    ifeq ($(UNAME_S),Darwin)
        PLATFORM = macos
    endif
endif

# 编译器和编译选项
CC = clang
CFLAGS = -fcolor-diagnostics -fansi-escape-codes -Wall -g -shared
LDFLAGS = -O2 -v

# 目标文件和库文件
ifeq ($(PLATFORM),windows)
    TARGET = lib_win.dll
    SRC_DIR = $(CURDIR)/c/windows
else ifeq ($(PLATFORM),linux)
    TARGET = lib_linux.so
    SRC_DIR = $(CURDIR)/c/linux
    CFLAGS += -fPIC
else ifeq ($(PLATFORM),macos)
    TARGET = lib_macos.dylib
    SRC_DIR = $(CURDIR)/c/macos
    CFLAGS += -fPIC
endif

# 源代码文件列表
SRC = $(wildcard $(SRC_DIR)/*.c)

# 生成的动态库放置的目录
LIB_DIR = lib

# 生成目标文件规则
$(LIB_DIR)/$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: all
all: $(LIB_DIR)/$(TARGET)

