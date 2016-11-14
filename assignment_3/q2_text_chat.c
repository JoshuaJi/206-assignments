#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define WAIT 1
#define DO_NOT_WAIT 0

int first_time_entry;
char last_reseived_msg[1000];

void error_and_die(const char* msg){
	perror(msg);
	exit(0);
}

void check_avaliable_message(const char *file_name){
	char buff[1000];
	buff[0] = '\0';
	do{
		FILE *incoming_file = fopen(file_name, "r");
		if (incoming_file == NULL && first_time_entry)
		{
			printf("Nothing received yet.\n");
			first_time_entry = 0;
			break;
		}
		while (incoming_file == NULL)
			incoming_file = fopen(file_name, "r");

		fgets(buff, 1000, incoming_file);
		if (strcmp(last_reseived_msg, buff) != 0){
			first_time_entry = 0;
			printf("Received: %s", buff);
			strcpy(last_reseived_msg, buff);
			break;
		}else if (first_time_entry){
			printf("Nothing received yet.\n");
			first_time_entry = 0;	
			break;
		}
		fclose(incoming_file);
	} while(!first_time_entry);
}

void send_message(const char *file_name, const char *username){
	char buff[1000];
	printf("Send:     ");
	fgets(buff, 1000, stdin);
	FILE *outgoing_file = fopen(file_name, "w");
	fputs("[", outgoing_file);
	fputs(username, outgoing_file);
	fputs("] ", outgoing_file);
	fputs(buff, outgoing_file);
	fclose(outgoing_file);
}

int main(int argc, char const *argv[])
{
	if (argc != 4){
		printf("Please enter 3 arguments\n Usage: %s <incoming_file> <outgoing_file> <chat_username>\n", argv[0]);
		exit(0);
	}

	first_time_entry = 1;

	const char *username = argv[3];

	strcpy(last_reseived_msg, "\0");

	while(1){
		check_avaliable_message(argv[1]);
		send_message(argv[2], username);
	}

	return 0;
}