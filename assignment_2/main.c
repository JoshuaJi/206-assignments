#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define INPUT_LENGTH 20

void sanity_check(int argc){
	if (argc != 2){
		printf(ANSI_COLOR_RED "Please provide exactly one arguemnt" ANSI_COLOR_RESET "\n");
		printf(ANSI_COLOR_RED "Usage: date | ./date_translate [localization file]" ANSI_COLOR_RESET "\n");
		exit(-1);
	}
}

int read_input(char * input[]){

	char * line = NULL;
	char * word;
	size_t linecap = 0;
	int length = (int)getline(&line, &linecap, stdin);
	if (length <= 0) {
        exit(-1);
    }
    int index = 0;
	while ((word = strsep(&line, " \t\n")) != NULL) {
		input[index++] = word;
	}
	return index-1;
}

void get_localization(char* file_name, char *date_loc_list[], char *month_loc_list[]){
	FILE *fp;
	char *line = NULL;
	size_t linecap = 0;
	char *word;
	int index = 0;

	if((fp = fopen(file_name, "r")) == NULL){
		printf(ANSI_COLOR_RED "File doesn't exist" ANSI_COLOR_RESET "\n");
		exit(-1);
	}
	getline(&line, &linecap, fp);
	while((word = strsep(&line, " \t\n")) != NULL){
		date_loc_list[index++] = strdup(word);
	}

	index = 0;
	getline(&line, &linecap, fp);
	while((word = strsep(&line, " \t\n")) != NULL){
		month_loc_list[index++] = strdup(word);
	}	
}

void do_string_localization(char* input[], int word_count, char* date_loc_list[], char* month_loc_list[]){

	char *dates[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
	char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct", "Nov", "Dec"};

	for (int i = 0; i < word_count; i++){

		for (int j = 0; j < sizeof(dates)/sizeof(dates[0]); j++){
			if (strcmp(dates[j], input[i]) == 0){
				input[i] = strdup(*(date_loc_list+j));
			}
		}

		for (int j = 0; j < sizeof(months)/sizeof(months[0]); j++){
			if (strcmp(months[j], input[i]) == 0){
				input[i] = strdup(*(month_loc_list+j));
			}
		}
	}
}

void print_date(char* input[], int length){
	for (int i=0; i < length; i++)
		printf("%s ", input[i]);
	printf("\n");
}

int main(int argc, char *argv[]){
	
	sanity_check(argc);
	char * input[INPUT_LENGTH];
	int word_count = read_input(input);

	char * date_loc_list[8];
	char * month_loc_list[13];
	get_localization(argv[1], date_loc_list, month_loc_list);

	do_string_localization(input, word_count, date_loc_list, month_loc_list);
	print_date(input, word_count);

	return 0;
}