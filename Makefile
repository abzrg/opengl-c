CC := clang
CFLAGS := -std=c2x -Wall -Wextra
CFLAGS += -g -O0
CPPFLAGS := -MMD -I./include/ `pkg-config --cflags glfw3`
LDFLAGS := `pkg-config --libs-only-L glfw3`
LDLIBS := `pkg-config --libs-only-l glfw3`

SRCS := $(wildcard src/*.c)
OBJS := $(patsubst src/%.c, build/%.o, $(SRCS))
DEPS := $(patsubst src/%.c, build/%.d, $(SRCS))

all: build/main

build:
	@mkdir -pv $@

build/main: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) $(LDLIBS) -o $@

build/sds.o: src/sds.c | build
	$(CC) $(CPPFLAGS) -Wall -std=c99 -pedantic -g -O0 -c $< -o $@

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	@$(RM) -rv build/
	@$(RM) -v compile_commands.json

.PHONY: all clean
