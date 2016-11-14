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

	if (argc != 2){
		printf("Please enter 1 arguments\n Usage: %s <file>\n", argv[0]);
		exit(0);
	}

	FILE *fp = fopen(argv[1], "r");
	if (fp == NULL)
		error_and_die("File does not exist");

	char c;
	int size;
	while((c = fgetc(fp)) != EOF){
		message[index++] = c;
	}
	size = --index;

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