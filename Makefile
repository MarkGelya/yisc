CXX       ?= g++
CXXFLAGS  ?= -std=c++17
LDFLAGS   ?=

ifeq ($(SYSTEMC_HOME),)
$(error SYSTEMC_HOME not set)
endif

TARGET := main

INCLUDE_DIRS := -I$(SYSTEMC_HOME) -Iinclude
LIB_DIRS     := -L$(SYSTEMC_HOME)
LIBS         := -lsystemc -lpthread

SRC_DIRS := src src/components
SRCS := $(shell find $(SRC_DIRS) -name '*.cpp')
BUILD_DIR := build
OBJS := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SRCS))

$(shell mkdir -p $(BUILD_DIR)/src/components)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $(LIB_DIRS) -o $@ $^ $(LIBS)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean

