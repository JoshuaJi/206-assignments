#include <stdio.h>
#include <stdlib.h>

// print the error message and exit
void error_and_die(const char* msg){
	perror(msg);
	exit(0);
}

int main(int argc, char const *argv[]){

	// declare a message variable that big enough to contain 1000 characters and a terminator
	char message[1001];

	// a int variable that tracks the current index
	int index = 0;

	// sanity check of arguments
	if (argc != 2){
		printf("Please enter 1 arguments\n Usage: %s <file>\n", argv[0]);
		exit(0);
	}

	// if the file doesn't exist, print error message and exit
	FILE *fp = fopen(argv[1], "r");
	if (fp == NULL)
		error_and_die("File does not exist");

	// copy string from file to message variable char by char
	char c;
	int size;
	while((c = fgetc(fp)) != EOF){
		message[index++] = c;
	}
	size = --index;

	// brute force cracking the message by using 0-255 as shifter, 
	// see which number give the readable message (message only contains a-z A-Z spaces and terminaters)
	char shifted_c;
	for (int n = 0; n < 256; n++){
		int correct = 1;
		index = size;
		while(--index > -1){
			shifted_c = (message[index]-n+256)%256;
			if (!((shifted_c >= 'a' && shifted_c <= 'z') || (shifted_c >= 'A' && shifted_c <= 'Z') || shifted_c == ' ' || shifted_c == '\0')){
				correct = 0;
				break;
			}
		}

		// if found the readable message, print it in reverse and exit
		if (correct){
			printf("%d\n", n);
			index = size;
			while(--index > -1){
				printf("%c", (message[index]-n+256)%256);
			}
			printf("\n");
			exit(0);
		}
	}
	
	
	return 0;
}