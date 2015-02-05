#include <Preprocessor.h>
#include <cstdlib>
#include <cstdio>

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("Usage: ./fa_to_regex <fa_file_name>\n");
		exit(EXIT_FAILURE);
	}

	Preprocessor p;
	p.run({argv[1]});

	printf("Hello, World!\n");
	return 0;
}
