TARGET			= super-clock
BUILD_DIR		= .build
SRC_DIR			?= src

CXX				?= g++
CFLAGS_BASE		= -std=c++20 $(shell pkg-config --cflags sdl2 SDL2_image SDL2_ttf SDL2_mixer fmt)
CFLAGS_RELEASE	= -O3 -march=native -flto -fno-plt -fomit-frame-pointer \
				  -fno-strict-aliasing -Wall -Wextra -Werror -Wpedantic \
				  -Wwrite-strings -Wconversion -Wmissing-declarations \
				  -Wfloat-equal -Wsign-compare -Wundef -Wcast-align \
				  -Wswitch-default -Wmissing-include-dirs
LDFLAGS			?=
LDLIBS_BASE		= $(shell pkg-config --libs sdl2 SDL2_image SDL2_ttf SDL2_mixer fmt) -lstdc++

SRCS			= $(wildcard $(SRC_DIR)/*.cpp)
OBJS			= $(addprefix $(BUILD_DIR)/, $(notdir $(SRCS:.cpp=.o)))
DEPS			= $(OBJS:.o=.d)

ifeq ($(OS),Windows_NT)
	CFLAGS_DEV	= -O0 -g3 -ggdb3 -fsanitize=address \
				  -fsanitize-address-use-after-scope \
				  -fno-strict-aliasing -Wall -Wextra \
				  -Werror -Wpedantic -Wwrite-strings -Wconversion \
				  -Wshadow -Wmissing-declarations -Wfloat-equal \
				  -Wsign-compare -Wundef -Wcast-align -Wswitch-default \
				  -Wmissing-include-dirs
	LDLIBS_DEV	=
	CLEAN 		= del /f $(TARGET).exe & if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
	MKDIR		= if not exist $(BUILD_DIR) mkdir
else
	CFLAGS_DEV	= -O0 -g3 -ggdb3 -fsanitize=address \
				  -fsanitize-address-use-after-scope \
				  -fstack-protector-strong -fno-strict-aliasing \
				  -Wall -Wextra -Werror -Wpedantic -Wwrite-strings \
				  -Wconversion -Wmissing-declarations -Wfloat-equal \
				  -Wsign-compare -Wundef -Wcast-align -Wswitch-default \
				  -Wmissing-include-dirs
	LDLIBS_DEV	= -fsanitize=address -fsanitize-address-use-after-scope
	CLEAN		= $(RM) -r $(TARGET) $(BUILD_DIR)
	MKDIR		= mkdir $(BUILD_DIR)
endif

CFLAGS		?= $(CFLAGS_BASE) $(CFLAGS_DEV)
LDLIBS		?= $(LDLIBS_BASE) $(LDLIBS_DEV)

$(BUILD_DIR):
	$(MKDIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CFLAGS) -MMD -MP -c $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $^ -o $@ $(LDLIBS)

-include $(DEPS)

.PHONY: all clean run rebuild release

all: $(TARGET)

release: CFLAGS = $(CFLAGS_BASE) $(CFLAGS_RELEASE)
release: LDLIBS = $(LDLIBS_BASE)
release: all

clean:
	$(CLEAN)

run: $(TARGET)
	./$<

rebuild: clean all
