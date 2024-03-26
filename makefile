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
CFLAGS = -std=c11 -fcolor-diagnostics -fansi-escape-codes -Wall -Wextra -Werror -g0 -shared -O3
SRC_DIR = $(CURDIR)/src
SRC_FILES = $(SRC_DIR)/share.c $(SRC_DIR)/rpmalloc.c

ifeq ($(PLATFORM),windows)
    TARGET = libtenet.dll
    EXTRA_LIBS = -lAdvapi32
    SRC := $(SRC_FILES) $(SRC_DIR)/lib_win.c $(SRC_DIR)/wepoll.c
else ifeq ($(PLATFORM),linux)
    TARGET = libtenet.so
    CFLAGS += -fPIC -flto -fvisibility=hidden -D_GNU_SOURCE
    SRC := $(SRC_FILES) $(SRC_DIR)/lib_linux.c 
else ifeq ($(PLATFORM),macos)
    TARGET = libtenet.dylib
    CFLAGS += -fPIC -flto -fvisibility=hidden
    SRC := $(SRC_FILES) $(SRC_DIR)/lib_macos.c 
else
    $(error Invalid PLATFORM : $(PLATFORM))
endif

# 生成的动态库放置的目录
LIB_DIR = lib

# 生成目标文件规则
$(LIB_DIR)/$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ -v $^ $(EXTRA_LIBS)

.PHONY: all
all: $(LIB_DIR)/$(TARGET)

