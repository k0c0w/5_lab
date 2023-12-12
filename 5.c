#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <libgen.h>
#include <dirent.h>

int main (int argc, char* argv[], char* envp[])
{
	DIR* targetDirDir;
	struct stat sourceFileInfo;
	FILE* sourceFile;
	FILE* targetFile;
	char* targetDir;
	char c;

	if (argc != 3){
		printf("Enter source file as first argument and destination file as second.\n");
		exit(1);
	}

	if (strcmp(argv[1], argv[2]) == 0) {
		printf("Source file and destination can not be same.\n");
		exit(1);
	}
	sourceFile = fopen(argv[1], "r");
	if (sourceFile == NULL) {
		printf("Error while opening file %s", argv[1]);
		exit(1);
	}

	if (stat(argv[1], &sourceFileInfo) != 0) {
		printf("Error while reading file stats.");
		fclose(sourceFile);
		exit(1);
	}

	targetFile = fopen(argv[2], "w");
	if (targetFile == NULL){
		printf("Error while opening destination file. Check if such directory exists.\n");
		fclose(sourceFile);
		exit(1);
	}

	printf("Coping file...\n");
	while((c = getc(sourceFile)) != EOF)
		putc(c, targetFile);
	printf("Done\n");

	chmod(argv[1], sourceFileInfo.st_mode);
	fclose(sourceFile);
	fclose(targetFile);

	return 0;
}
