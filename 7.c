#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

void putIntoFile(const char* str, FILE* outputFile);

int isDir(char* path) {
	struct stat buff;
	
	stat(path, &buff);

	return S_ISREG(buff.st_mode);
}

int fileHasSameContent(char* pathToFirst, char* pathToSecond) {
	FILE* first;
	FILE* second;
	char fC;
	char sC;
	
	if (strcmp(pathToFirst, pathToSecond) == 0)
		return 1;

	first = fopen(pathToFirst, "r");
	if(first == NULL)
	{
		return 1;
	}

	second = fopen(pathToSecond, "r");
	if(second == NULL){
		fclose(first);
		return 1;
	}

	fC = getc(first);
	sC = getc(second);
	while (fC != EOF || sC != EOF) {
		if (fC != sC){
			fclose(first);
			fclose(second);
			return 1;
		}
		fC = getc(first);
		sC = getc(second);
	}

	fclose(first);
	fclose(second);

	if (fC == EOF && sC == EOF)
		return 0;

	return 1;
}

char* concat(char* first, char* second) {
	char* str = malloc(strlen(first) + strlen(second) + 2);
	sprintf(str, "%s/%s", first, second);
	return str;
}

void recordFile(char* first, char* second, FILE* outputFile) {
	struct stat firstInfo;
	struct stat secondInfo;
	char* firstInfoStr;
	char* secondInfoStr;
	const char* title = "-- Found files with same content.\n\n";
	const char* outputFormat = "\t %s\n\t\tSIZE: %db\n\t\tMODIFIED DATE: %s\n\t\tRIGHTS: %d\n\t\tDESCRIPTOR: %lu\n";
	
	stat(first, &firstInfo);
	stat(second, &secondInfo);

	asprintf(&firstInfoStr, outputFormat, first, firstInfo.st_size, ctime(&firstInfo.st_ctime), firstInfo.st_mode, firstInfo.st_ino);
        asprintf(&secondInfoStr, outputFormat, second, secondInfo.st_size, ctime(&secondInfo.st_ctime), secondInfo.st_mode, secondInfo.st_ino);
	
	printf("%s", title);
	putIntoFile(title, outputFile);
	printf("%s", firstInfoStr);
        putIntoFile(firstInfoStr, outputFile);
	printf("%s", secondInfoStr);
	putIntoFile(secondInfoStr, outputFile);

	free(firstInfoStr);
	free(secondInfoStr);
}

void putIntoFile(const char* str, FILE* file)
{
	size_t i = 0;
	while(str[i] != '\0')
	{
		putc(str[i], file);
		i++;
	}
}

void deepSearch(char* pathToFileToCompare, char* dir, FILE* outputFile) {
	DIR* dp;
	struct dirent* dInfo;
	char* currentFilePath;
	
	dp = opendir(dir);
	if(dp == NULL)
		return;

	dInfo = readdir(dp);
	while(dInfo != NULL) {
		if (strcmp(dInfo->d_name, ".") != 0 && strcmp(dInfo->d_name, "..") != 0){
			currentFilePath = concat(dir, dInfo->d_name);
			if (isDir(currentFilePath) == 0){
				deepSearch(pathToFileToCompare, currentFilePath, outputFile);
			}
			else {
				if(fileHasSameContent(pathToFileToCompare, currentFilePath) == 0) {
					recordFile(pathToFileToCompare, currentFilePath, outputFile);
				}
			}

			free(currentFilePath);
		}
		dInfo = readdir(dp);
	}

	if (closedir(dp) != 0){
		printf("Error while closing dir: %s", dir);
		fclose(outputFile);
		exit(1);
	}
}

void traverse(char* pathToScanDir, char* searchRootDir, FILE* outputFile) {
	DIR* dp;
	struct dirent* dInfo;
	char* currentFilePath;

	dp = opendir(pathToScanDir);
	if (dp == NULL)
		return;

	dInfo = readdir(dp);
	while (dInfo != NULL) {
		if (strcmp(dInfo->d_name, ".") != 0 && strcmp(dInfo->d_name, "..") != 0){
			currentFilePath = concat(pathToScanDir, dInfo->d_name); 
			if (isDir(currentFilePath) == 0){
				traverse(currentFilePath, searchRootDir, outputFile);
			}
			else
				deepSearch(currentFilePath, searchRootDir, outputFile);

			free(currentFilePath);
		}
		dInfo = readdir(dp);
	}

	if (closedir(dp) != 0)
	{
		printf("Error while closing dir: %s", pathToScanDir);
		fclose(outputFile);
		exit(1);
	}
}

int main(int argc, char* argv[], char* envp) {
	FILE* outputFile;
	
	if(argc != 4) {
		printf("First and second args must be scanning folders paths. Third - output file.\n");
		exit(1);
	}

	if (isDir(argv[1]) != 0 || isDir(argv[2]) != 0) {
		printf("Wrong argument values. Must be existing folders.\n");
		exit(1);
	}

	if ((outputFile = fopen(argv[3], "w")) == NULL)
	{
		printf("Can not open %s file. Check if folder exists.\n", argv[3]);
		exit(1);
	}

	traverse(argv[1], argv[2], outputFile);

	fclose(outputFile);

	return 0;
}

