#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_MSG_LENGTH 1200

int first_time_entry;
char last_reseived_msg[MAX_MSG_LENGTH];

void error_and_die(const char* msg){
	perror(msg);
	exit(0);
}

void decrypt_message(char *msg, char *decrypted_msg, int shift){
	int i = 0;
	while(msg[i] != EOF){
		decrypted_msg[i] = (msg[i]-shift+256)%256;
		i++;
	}
}

void encrypt_message(char *msg, char *encrypted_msg, int shift){
	int i = 0;
	while(msg[i] != '\0'){
		encrypted_msg[i] = (msg[i]+shift)%256;
		i++;
	}
}

void check_avaliable_message(const char *file_name, int shift){
	char buff[MAX_MSG_LENGTH];
	char decrypted_msg[MAX_MSG_LENGTH];
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

		fgets(buff, MAX_MSG_LENGTH, incoming_file);
		if (strcmp(last_reseived_msg, buff) != 0){
			first_time_entry = 0;
			decrypt_message(buff, decrypted_msg, shift);
			printf("Received: %s\n", decrypted_msg);
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

void send_message(const char *file_name, const char *username, int shift){
	char buff[MAX_MSG_LENGTH];
	char encrypted_msg[MAX_MSG_LENGTH];
	printf("Send:     ");
	fgets(buff, MAX_MSG_LENGTH, stdin);
	FILE *outgoing_file = fopen(file_name, "w");

	strcat(encrypted_msg, "[");
	strcat(encrypted_msg, username);
	strcat(encrypted_msg, "] ");
	strcat(encrypted_msg, buff);

	encrypt_message(encrypted_msg, encrypted_msg, shift);
	printf("encrypted msg: %s\n", encrypted_msg);
	fputs(encrypted_msg, outgoing_file);
	fclose(outgoing_file);
}

int main(int argc, char const *argv[])
{
	if (argc != 5){
		printf("Please enter 4 arguments\n Usage: %s <incoming_file> <outgoing_file> <chat_username> <key>\n", argv[0]);
		exit(0);
	}

	first_time_entry = 1;

	const char *username = argv[3];
	int key = atoi(argv[4]);

	strcpy(last_reseived_msg, "\0");

	while(1){
		check_avaliable_message(argv[1], key);
		send_message(argv[2], username, key);
	}

	return 0;
}