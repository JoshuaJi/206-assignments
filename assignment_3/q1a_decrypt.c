#include <stdio.h>
#include <stdlib.h>

// print the error message and exit
void error_and_die(const char* msg){
	perror(msg);
	exit(0);
}

int main(int argc, char const *argv[])
{
	// declare a message variable that big enough to contain 1000 characters and a terminator
	char message[1001];
	// a int variable that tracks the current index
	int index = 0;

	// sanity check of arguments
	if (argc != 3){
		printf("Please enter 2 arguments\n Usage: %s <number> <file>\n", argv[0]);
		exit(0);
	}

	// check if the shift number user entered is valid
	int n = atoi(argv[1]);
	if (n < 0 || n > 256)
		error_and_die("Please enter a number range from 0 to 256");

	// check if the message file exist and readable
	FILE *fp = fopen(argv[2], "r");
	if (fp == NULL)
		error_and_die("File does not exist");

	// literate through the file char by char, and shift the char on the fly
	char c;
	while((c = fgetc(fp)) != EOF){
		message[index++] = (c-n+256)%256;
	}
	index--;

	// print the message in reverse
	while(--index > -1)
		printf("%c", message[index]);
	printf("\n");
	return 0;
}