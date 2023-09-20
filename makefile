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

CC = clang
CFLAGS = -fcolor-diagnostics -fansi-escape-codes -Wall -g0 -shared
SRC_DIR = $(CURDIR)/src
LDFLAGS = -O2 -v

ifeq ($(PLATFORM),windows)
    TARGET = libtenet.dll
    SRC = $(SRC_DIR)/lib_win.c $(SRC_DIR)/wepoll.c $(SRC_DIR)/share.c
else ifeq ($(PLATFORM),linux)
    TARGET = libtenet.so
    CFLAGS += -fPIC
    SRC = $(SRC_DIR)/lib_linux.c $(SRC_DIR)/share.c
else ifeq ($(PLATFORM),macos)
    TARGET = libtenet.dylib
    CFLAGS += -fPIC
    SRC = $(SRC_DIR)/lib_macos.c $(SRC_DIR)/share.c
else
    $(error Invalid PLATFORM)
endif

# 生成的动态库放置的目录
LIB_DIR = lib

# 生成目标文件规则
$(LIB_DIR)/$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: all
all: $(LIB_DIR)/$(TARGET)

