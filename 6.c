#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>

int isDir(char* path) {
	struct stat buff;

	if (stat(path, &buff) == 1)
		return 1;

	return S_ISDIR(buff.st_mode);
}

void printDirContent(char* path) {
	DIR* dp = opendir(path);
	if (dp == NULL){
		printf("Can not open dir: %s", path);
		exit(1);
	}
	struct dirent* info;

	while((info = readdir(dp)) != NULL) {
		printf("%s\n", info->d_name);
	}

	if (closedir(dp) == 1){
		printf("Error while closing dir: %s", path);
		exit(1);
	}
}

int main(int argc, char* argv[], char* envp[])
{
	struct stat sourceFileInfo;
	FILE* sourceFile;
	FILE* targetFIle;
	char* targetDir;
	char c;

	if (argc != 2){
		printf("Enter target dir path as first argument");
		exit(1);
	}

	printf("Current directory\n");
	printDirContent(".");


	if (isDir(argv[1])) {
		printf("\n\n%s directory\n", argv[1]);
		printDirContent(argv[1]);
	}

	return 0;
}
