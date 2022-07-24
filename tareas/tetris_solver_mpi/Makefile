APPNAME=$(shell basename $(shell pwd))
DIRS=src
HEADERS=$(wildcard $(DIRS:%=%/*.h))
SOURCES=$(wildcard $(DIRS:%=%/*.c))
OBJECTS=$(SOURCES:%.c=build/%.o)
EXEFILE=bin/$(APPNAME)

CC=cc -std=gnu11
CFLAGS=-Wall -Wextra
LIBS= -fopenmp
LIBS2= -lm

LINTFILTERS=$\
	-build/c++11,$\
	-build/header_guard,$\
	-build/include_subdir,$\
	-readability/casting,$\
	-runtime/references

debug: CFLAGS += -g
release: CFLAGS += -O3 -DNDEBUG

asan: CFLAGS += -fsanitize=address -fno-omit-frame-pointer -g
msan: CFLAGS += -fsanitize=memory -g
msan: CC = clang
tsan: CFLAGS += -fsanitize=thread -g
ubsan: CFLAGS += -fsanitize=undefined -g

.SECONDEXPANSION:

default: clean $(EXEFILE)
debug: clean $(EXEFILE)
release: clean $(EXEFILE)
asan: clean $(EXEFILE)
msan: clean $(EXEFILE)
tsan: clean $(EXEFILE)
ubsan: clean $(EXEFILE)

$(EXEFILE): $(OBJECTS) | $$(@D)/.
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS) $(LIBS2)

build/%.o: %.c | $$(@D)/.
	$(CC) -c $(CFLAGS) $< -o $@ $(LIBS) $(LIBS2)

.PRECIOUS: %/.
%/.:
	mkdir -p $(dir $@)

.PHONY: memcheck
memcheck:
	valgrind --leak-check=full --show-leak-kinds=all $(EXEFILE)

.PHONY: helgrind
helgrind:
	valgrind --tool=helgrind --quiet $(EXEFILE)

.PHONY: rebuild
rebuild: clean $(EXEFILE)

.PHONY: lint
lint:
	cpplint --filter=$(LINTFILTERS) $(HEADERS) $(SOURCES)

.PHONY: gitignore
gitignore:
	echo "bin/ build/ tetris_play_*" | tr " " "\n" > .gitignore

.PHONY: clean
clean:
	rm -rf bin/ build/