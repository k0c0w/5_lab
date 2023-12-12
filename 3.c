#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[], char* envp[])
{
	FILE* f;
	char c;

	if(argc == 1)
	{
		printf("Enter output file as first argument. Make sure directory exists.\n");
		exit(1);
	}

	f = fopen(argv[1], "w");
	if (f == NULL) {
		printf("Error while creating file %s.\n", argv[1]);
		exit(1);
	}

	printf("Enter text. Press Ctrl+F to mark end of the file.\n");

	while ((c = getc(stdin)) != 6)
		putc(c,f);
	fclose(f);

	return 0;
}
