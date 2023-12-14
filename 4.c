#include <stdio.h>
#include <stdlib.h>


int main (int argc, char* argv[], char* envp[])
{
	FILE* f;
	char c;


	if (argc != 3)
	{
		printf("Pass file to print as first arg, amount of lines to print (n = 0 for whole text)");
		exit(1);
	}
	int N = atoi(argv[2]);
	if (N < 0){
		printf("Wrong lines count: %d!\n", N);
		exit(1);
	}

	f = fopen(argv[1], "r");
	if (f == NULL)
	{
		printf("Can not open file %s", argv[1]);
		exit(1);
	}



	int printedLines = 0;
	while((c = getc(f)) != EOF){
		if (N == 0){
			printf("%c", c);
		}
		else {
			if (printedLines == N)
			{
				printf("--- Press any key to continue ---\r");
				getc(stdin);
				printedLines = 0;
			}
			printf("%c", c);
			if (c ==10)
			{
				printedLines++;
			}
		}
	}
	printf("--- End of the file ---\n");

	if (fclose(f) !=0)
	{
		printf("Error during file close: %s", argv[1]);
		exit(1);
	}

	return 0;
}
