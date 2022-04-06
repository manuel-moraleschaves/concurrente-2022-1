compile:
	cc -g hello_multiple.c -Wall -Wextra -o hello_multiple -pthread

msan:
	clang -g hello_multiple.c -fsanitize=memory -Wall -Wextra -o hello_multiple_msan -pthread

asan:
	clang -g hello_multiple.c -fsanitize=address -Wall -Wextra -o hello_multiple_asan -pthread

tsan:
	clang -g hello_multiple.c -fsanitize=thread -Wall -Wextra -o hello_multiple_tsan -pthread