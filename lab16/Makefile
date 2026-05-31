CC = clang
LAB_OPTS = -I./src src/entity.c src/list.c
C_OPTS = $(MAC_OPTS) -std=gnu11 -g -gdwarf-4 -Wall -Wextra -Werror -Wformat-security -Wfloat-equal -Wshadow -Wconversion -Wlogical-not-parentheses -Wnull-dereference -Wno-unused-variable -Wno-implicit-int-float-conversion -Werror=vla $(LAB_OPTS)

V_FLAGS=--tool=memcheck --leak-check=full --show-reachable=yes \
--undef-value-errors=yes --track-origins=no --child-silent-after-fork=no \
--trace-children=no --error-exitcode=1

DOXYGEN  = doxygen
DOXYFILE = Doxyfile

clean:
	rm -rf dist

prep:
	mkdir dist

compile: main.bin test.bin

main.bin: src/main.c
	$(CC) $(C_OPTS) src/menu.c $< -o ./dist/$@

test.bin: test/test.c
	$(CC) $(C_OPTS) $< -fprofile-instr-generate -fcoverage-mapping -o ./dist/$@ -pthread -lcheck_pic -lrt -lm -lsubunit

run: clean prep compile
	./dist/main.bin "./assets/input.txt"

test: clean prep compile
	LLVM_PROFILE_FILE="dist/test.profraw" ./dist/test.bin

coverage: clean prep compile
	LLVM_PROFILE_FILE="dist/test.profraw" ./dist/test.bin
	llvm-profdata-14 merge -sparse dist/test.profraw -o dist/test.profdata
	llvm-cov-14 report dist/test.bin -instr-profile=dist/test.profdata src/*.c
	llvm-cov-14 show dist/test.bin -instr-profile=dist/test.profdata src/*.c --format html > dist/coverage.html

leak-check: clean prep compile
	valgrind $(V_FLAGS) --log-file=dist/valgrind.log ./dist/main.bin ./assets/input.txt </dev/null
	valgrind $(V_FLAGS) --log-file=dist/valgrind_test.log ./dist/test.bin

check:
	clang-format --verbose -dry-run --Werror src/*
	clang-tidy src/*.c -checks=-readability-uppercase-literal-suffix,-readability-magic-numbers,-clang-analyzer-deadcode.DeadStores,-clang-analyzer-security.insecureAPI.rand
	rm -rf src/*.dump

doxygen:
	$(DOXYGEN) $(DOXYFILE)

all: clean prep compile run
