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

void decrypt_message(char *msg, char *decrypted_msg, int shift){
	int i = 0;
	int size = strlen(msg) - 1;

	// reverse and shift the message char by char
	while(msg[i] != EOF){
		if (i <= size)
			decrypted_msg[size-i] = (msg[i]-shift+256)%256;
		else
			decrypted_msg[i] = '\0';
		i++;
	}
}

void encrypt_message(char *msg, char *encrypted_msg, int shift){
	int i = 0;
	int size = strlen(msg) - 1;

	// reverse and shift the message char by char
	while(msg[i] != EOF){
		if (i <= size)
			encrypted_msg[size-i] = (msg[i]+shift)%256;
		else
			encrypted_msg[i] = '\0';
		i++;
	}
}

// check if the incoming file has changed content
void check_avaliable_message(const char *file_name, int shift){

	// initiating 2 buffers and set them to empty
	char buff[MAX_MSG_LENGTH];
	char decrypted_msg[MAX_MSG_LENGTH];
	decrypted_msg[0] = '\0';
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
		// if they are different, decode and print the message before break the while loop
		fgets(buff, MAX_MSG_LENGTH, incoming_file);
		if (strcmp(last_reseived_msg, buff) != 0){
			first_time_entry = 0;
			decrypt_message(buff, decrypted_msg, shift);
			printf("Received: %s", decrypted_msg);
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
void send_message(const char *file_name, const char *username, int shift){

	// prompt the user to enter new message
	char buff[MAX_MSG_LENGTH];
	char msg[MAX_MSG_LENGTH];
	char encrypted_msg[MAX_MSG_LENGTH];
	msg[0] = '\0';
	encrypted_msg[0] = '\0';
	printf("Send:     ");
	fgets(buff, MAX_MSG_LENGTH, stdin);

	// open the outgoing file and prepare to write the message with username
	FILE *outgoing_file = fopen(file_name, "w");

	strcat(msg, "[");
	strcat(msg, username);
	strcat(msg, "] ");
	strcat(msg, buff);

	// encode the message and write it to outgoing file
	encrypt_message(msg, encrypted_msg, shift);
	fputs(encrypted_msg, outgoing_file);

	// close the outgoing file
	fclose(outgoing_file);
}

int main(int argc, char const *argv[])
{

	// sanity check of the input
	if (argc != 5){
		printf("Please enter 4 arguments\n Usage: %s <incoming_file> <outgoing_file> <chat_username> <key>\n", argv[0]);
		exit(0);
	}

	// initiating the variables
	first_time_entry = 1;
	const char *username = argv[3];
	strcpy(last_reseived_msg, "\0");

	// check if the key is in valide range
	int key = atoi(argv[4]);
	if (key < 0 || key > 256)
		error_and_die("Please enter a number range from 0 to 256");

	// check and send message until user hit ctl-c
	while(1){
		check_avaliable_message(argv[1], key);
		send_message(argv[2], username, key);
	}

	return 0;
}