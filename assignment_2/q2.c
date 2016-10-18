#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define STAR_LENGTH 1
#define SPACE_LENGTH 1
#define NUMBER_OF_MONTH 12
#define NUMBER_DAYS_IN_WEEK 7

// Sanity check for the input, if the arguments are not integer/out of range or # of arguments is not 4, it will exit
void sanity_check(int argc, char *argv[], int *day_spacing, int *first_day){
	if (argc != 4){
		printf(ANSI_COLOR_RED "Please provide exactly 4 arguemnts" ANSI_COLOR_RESET "\n");
		printf(ANSI_COLOR_RED "Usage: ./calendar [localization file] [day_spacing] [first_day]" ANSI_COLOR_RESET "\n");
		exit(-1);
	}

	if (atoi(argv[2]) < 2){
		printf(ANSI_COLOR_RED "Day spacing should be at least 2" ANSI_COLOR_RESET "\n");
		exit(-1);
	}

	if (atoi(argv[3]) < 1 || atoi(argv[3]) > 7){
		printf(ANSI_COLOR_RED "Starting day is invalid, should be an integer between 1 and 7" ANSI_COLOR_RESET "\n");
		exit(-1);
	}

	// Parse arguments to int and assign them to variables
	*day_spacing = atoi(argv[2]);
	*first_day = atoi(argv[3])-1;
}

// Retrieve the localization file, if it exists, read and parse the words into an array
// If the file does not exist, terminate the program
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

// Print a line of stars with length argument
void print_full_length_star(int line_length){
	for (int i = 0; i < line_length; ++i){
		printf("*");
	}
	printf("\n");
}

// Print dates banner with format (ex. * Sun * Mon * Tue * Wed * Thu * Fri * Sat)
void print_dates_banner(char *date_loc_list[], int first_day, int day_spacing){
	for (int i = 0; i < NUMBER_DAYS_IN_WEEK; ++i){
		int num_char_printed = 0;
		printf("* ");
		while(date_loc_list[i][num_char_printed] && num_char_printed < day_spacing){
			printf("%c", date_loc_list[i][num_char_printed]);
			num_char_printed++;
		}

		for (int j = 0; j < day_spacing - num_char_printed; ++j){
			printf(" ");
		}
		printf(" ");
	}
	printf("\n");
}

/* Print month name with two lines of stars
*   ex.
*	*********************
*   * November
*   *********************
*/
void print_month_title(int line_length, int day_spacing, char *month, char *date_loc_list[], int first_day){
	print_full_length_star(line_length);
	printf("* %s\n", month);
	print_full_length_star(line_length);
	print_dates_banner(date_loc_list, first_day, day_spacing);
	print_full_length_star(line_length);
}

// Print 1-30 with empty space padings 
int print_month_dates(int first_day, int day_spacing){
	int total_dates_printed = 0;
	int current_date = 1;
	first_day = first_day % NUMBER_DAYS_IN_WEEK;
	char spaces[day_spacing+1];
	for (int i = 0; i < day_spacing; i++){
		spaces[i] = ' ';
	}
	spaces[day_spacing] = '\0';
	while((current_date <= 30) || (total_dates_printed%7 != 0)){
		if (total_dates_printed%7 == 0 && current_date > 1){
			printf("\n");
		}
		if (total_dates_printed < first_day){
			printf("* %s ", spaces);
		}else if(current_date <= 30){
			printf("* %d", current_date);
			int space_needed = day_spacing-(current_date<10?1:2);
			if (space_needed > 0)
				printf("%*s", space_needed, " ");
			printf(" ");
			current_date++;
		}else{
			printf("* %s ", spaces);
		}
		total_dates_printed++;
	}
	printf("\n");
	return (current_date + first_day - 1)%7;
}

// Print a full month (name and dates) with format
int print_month_calender(int line_length, int day_spacing, char *month, char *date_loc_list[], int first_day){
	int end_date;
	print_month_title(line_length, day_spacing, month, date_loc_list, first_day);
	end_date = print_month_dates(first_day, day_spacing);
	return end_date;
}

// Print a whole year calender (Jan - Dec) with format
void print_year_calender(int line_length, int first_day, int day_spacing, char *month_loc_list[], char *date_loc_list[]){
	int start_day_for_month = first_day;
	for (int i = 0; i < NUMBER_OF_MONTH; ++i){
		start_day_for_month = print_month_calender(line_length, day_spacing, month_loc_list[i], date_loc_list, start_day_for_month);
	}
}

int main(int argc, char *argv[]){

	// Declare variables and do sanity check with the input
	int day_spacing, first_day;
	sanity_check(argc, argv, &day_spacing, &first_day);

	// Calculate the length of stars
	int line_length = NUMBER_DAYS_IN_WEEK * (day_spacing + STAR_LENGTH + 2*SPACE_LENGTH);

	// Declare variables and get localization data
	char * date_loc_list[8];
	char * month_loc_list[13];
	get_localization(argv[1], date_loc_list, month_loc_list);

	// Print year calender
	print_year_calender(line_length, first_day, day_spacing, month_loc_list, date_loc_list);
	return 0;
}