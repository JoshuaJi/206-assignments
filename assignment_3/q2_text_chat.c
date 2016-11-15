#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX_MSG_LENGTH 1200

int first_time_entry;
char last_reseived_msg[MAX_MSG_LENGTH];

// print the error message and exit
void error_and_die(const char* msg){
	perror(msg);
	exit(0);
}

// check if the incoming file has changed content
void check_avaliable_message(const char *file_name){

	// initiating a buffer and set it to empty
	char buff[MAX_MSG_LENGTH];
	buff[0] = '\0';

	// constently open and close the incoming file to see if anything changed
	do{

		// checking the initial state of incoming message, if no message, print "Nothing received yet."
		FILE *incoming_file = fopen(file_name, "r");
		if (incoming_file == NULL && first_time_entry)
		{
			printf("Nothing received yet.\n");
			first_time_entry = 0;
			break;
		}

		// if incoming file doesn't exist, keep trying in this while loop
		while (incoming_file == NULL)
			incoming_file = fopen(file_name, "r");

		// copy the message from file to buffer, and compare it with last received message, 
		// if they are different, print the message and break the while loop
		fgets(buff, MAX_MSG_LENGTH, incoming_file);
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

		// close the incoming file
		fclose(incoming_file);
	} while(!first_time_entry);
}

// write the outgoing message into the ourgoing file with username
void send_message(const char *file_name, const char *username){

	// prompt the user to enter new message
	char buff[MAX_MSG_LENGTH];
	printf("Send:     ");
	fgets(buff, MAX_MSG_LENGTH, stdin);

	// open the outgoing file and prepare to write the message with username
	FILE *outgoing_file = fopen(file_name, "w");
	fputs("[", outgoing_file);
	fputs(username, outgoing_file);
	fputs("] ", outgoing_file);
	fputs(buff, outgoing_file);

	// close the outgoing file
	fclose(outgoing_file);
}

int main(int argc, char const *argv[])
{
	// sanity check of the input
	if (argc != 4){
		printf("Please enter 3 arguments\n Usage: %s <incoming_file> <outgoing_file> <chat_username>\n", argv[0]);
		exit(0);
	}

	// initiating the variables
	first_time_entry = 1;
	const char *username = argv[3];
	strcpy(last_reseived_msg, "\0");

	// check and send message until user hit ctl-c
	while(1){
		check_avaliable_message(argv[1]);
		send_message(argv[2], username);
	}

	return 0;
}