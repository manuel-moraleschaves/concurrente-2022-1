APPNAME=$(shell basename $(shell pwd))
HEADERS=$(wildcard *.hpp)
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:%.cpp=build/%.o)
EXEFILE=bin/$(APPNAME)

CC=mpic++
CFLAGS=-Wall -Wextra -fopenmp
LIBS=

LINTFILTERS=$\
	-build/c++11,$\
	-build/header_guard,$\
	-build/include_subdir,$\
	-runtime/references

.SECONDEXPANSION:

default: debug

debug: CFLAGS += -g
debug: $(EXEFILE)
release: CFLAGS += -O3 -DNDEBUG
release: $(EXEFILE)
asan: CFLAGS += -fsanitize=address -fno-omit-frame-pointer
asan: debug
msan: CFLAGS += -fsanitize=memory
msan: CXX = clang++
msan: debug
tsan: CFLAGS += -fsanitize=thread
tsan: debug
ubsan: CFLAGS += -fsanitize=undefined
ubsan: debug

$(EXEFILE): $(OBJECTS) | $$(@D)/.
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

build/%.o: %.cpp | $$(@D)/.
	$(CC) -c $(CFLAGS) $< -o $@ $(LIBS)

.PRECIOUS: %/.
%/.:
	mkdir -p $(dir $@)

.PHONY: memcheck
memcheck:
	valgrind $(EXEFILE)

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
	echo "bin/ build/" | tr " " "\n" > .gitignore

.PHONY: clean
clean:
	rm -rf bin/ build/