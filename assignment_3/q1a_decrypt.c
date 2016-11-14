#include <stdio.h>
#include <stdlib.h>


void error_and_die(const char* msg){
	perror(msg);
	exit(0);
}

int main(int argc, char const *argv[])
{
	char message[1001];
	int index = 0;

	if (argc != 3){
		printf("Please enter 2 arguments\n Usage: %s <number> <file>\n", argv[0]);
		exit(0);
	}

	int n = atoi(argv[1]);
	if (n < 0 || n > 256)
		error_and_die("Please enter a number range from 0 to 256");

	FILE *fp = fopen(argv[2], "r");
	if (fp == NULL)
		error_and_die("File does not exist");

	char c;
	while((c = fgetc(fp)) != EOF){
		message[index++] = (c-n+256)%256;
	}
	index--;
	while(--index > -1)
		printf("%c", message[index]);
	printf("\n");
	return 0;
}