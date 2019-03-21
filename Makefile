MAKEFILE_PATH := $(lastword $(MAKEFILE_LIST))
BASE_DIR := $(dir $(MAKEFILE_PATH))
BASE_DIR := $(BASE_DIR:%/=%)
SOURCE_DIR := $(BASE_DIR)/shoutout
BUILD_DIR := $(BASE_DIR)/build

SRCS := $(shell find "$(SOURCE_DIR)" \( -iname '*.cc' -and -not -iname '*_test.cc' \) | sort)
OBJS := $(SRCS:$(SOURCE_DIR)/%.cc=$(BUILD_DIR)/%.o)
TESTS := $(shell find "$(SOURCE_DIR)" -iname '*_test.cc' | sort)
TESTS_RUN := $(TESTS:%=%.run)
TIDYS := $(SRCS:%=%.tidy)
LINTS := $(SRCS:%=%.lint)

MKDIR_P ?= mkdir -p
CPPLINT ?= cpplint
CLANG_TIDY ?= clang-tidy

DEBUG ?= 1

CPPFLAGS += -std=c++11 -Wall -Wextra -Wc++11-compat -Werror -pedantic-errors -ffast-math -ftrapv -I"$(BASE_DIR)"

ifeq ($(DEBUG), 1)
	CPPFLAGS += -glldb -O0
else
	CPPFLAGS += -O3
endif

TIDYFLAGS := $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $(TARGET_ARCH)
TIDYFLAGS := $(TIDYFLAGS:%=-extra-arg="%")


.PHONY: all tidy lint test docs clean

all: $(BUILD_DIR)/ShoutOut

$(BUILD_DIR)/ShoutOut: $(OBJS)
	$(LINK.cpp) $^ -o "$@"

$(BUILD_DIR)/%_test: $(SOURCE_DIR)/%_test.cc
	@$(MKDIR_P) "$(dir $@)"
	$(LINK.cpp) "$<" -o "$@"

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cc $(wildcard $(SOURCE_DIR)/%.h)
	@$(MKDIR_P) "$(dir $@)"
	$(COMPILE.cpp) "$<" -o "$@"

$(SOURCE_DIR)/%_test.run: $(BUILD_DIR)/%_test
	"$@"

$(SOURCE_DIR)/%.tidy: $(SOURCE_DIR)/%
	"$(CLANG_TIDY)" -header-filter="$(SOURCE_DIR)" "$(@:%.tidy=%)"

$(SOURCE_DIR)/%.lint: $(SOURCE_DIR)/%
	"$(CPPLINT)" --filter=-readability/nolint "$(@:%.lint=%)"

tidy: $(TIDYS)

lint: $(LINTS)

test: $(TESTS_RUN)
	@echo "Tests passed"

docs:
	doxygen

clean:
	$(RM) -r $(BUILD_DIR)
