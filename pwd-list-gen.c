/*
	Generates all possible character combinations for a given character set and
	length to a file.
    Copyright (C) 2017  Elliott Sobek

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUM_LEN 10
#define ALPHA_LEN 26
#define SYMBOL_LEN 36

#define NT_LEN 1
#define EXT_LEN 4
#define ARG_MAX 7
#define NULL_OPT -1
#define NEXT_INDEX 1
#define FIRST_ELEM 0
#define PREV_INDEX 1
#define NEWLINE_LEN 1
#define MAX_STR_LEN 98
#define NULL_STR_LEN -1
#define MIN_ENTRY_LEN 1
#define DEFAULT_ENTRY_LEN 8

#define DEFAULT_FILENAME "list.txt"
#define DEFAULT_CHOICE_SET "0123456789"

int get_total_amount_of_entries(char *choice_set, int entry_len) {
	return (int) pow((double) strnlen(choice_set, MAX_STR_LEN), (double) entry_len);
}

int get_exact_filesize(char *choice_set, int entry_len) { // In bytes
	return get_total_amount_of_entries(choice_set, entry_len) * (entry_len + NEWLINE_LEN);
}

char get_next_char(const char c, const char *const choice_set) {
	const size_t len_n = strnlen(choice_set, MAX_STR_LEN);

	for (unsigned int i = 0; i < len_n; i++)
		if (c == choice_set[i])
			return choice_set[i + NEXT_INDEX];
	return '\0';
}

void gen_entries(char *choice_set, const int entry_len, FILE *fp) {
	const char *const choices = choice_set;
	const size_t len_n = strnlen(choices, MAX_STR_LEN);
	const char last_elem = choices[len_n - NT_LEN];
	char entry[entry_len + NT_LEN], end_entry[entry_len + NT_LEN];

	// Clear the arrays
	for (int i = 0; i < entry_len + NT_LEN; i++) {
		entry[i] = '\0';
		end_entry[i] = '\0';
	}

	// Init string/entry
	for (int i = 0; i < entry_len; i++) {
		entry[i] = choices[FIRST_ELEM];
		end_entry[i] = last_elem;
	}

	while (strncmp(entry, end_entry, entry_len) != 0) { // While entry not last entry

		// This loop only applies if you have acc -> baa
		for (int i = entry_len - NT_LEN; i > NULL_STR_LEN; i--) { // Go back down the entry list from the back

			if (entry[i] == last_elem) { // If the current entry index is the last elem

				if (entry[i - PREV_INDEX] == last_elem)
					continue;

				fprintf(fp, "%s\n", entry);
				entry[i - PREV_INDEX] = get_next_char(entry[i - PREV_INDEX], choices);

				for (int j = i; j < entry_len; j++) // Reset current index and forward ones to base choice
					entry[j] = choices[FIRST_ELEM];
			}
			break;
		}
		fprintf(fp, "%s\n", entry);
		entry[entry_len - PREV_INDEX] = get_next_char(entry[entry_len - PREV_INDEX], choices);
	}
	fprintf(fp, "%s\n", entry);
}

int main(const int argc, char *const argv[]) {
	const char num[NUM_LEN + NT_LEN] = "0123456789",
	lower[ALPHA_LEN + NT_LEN] = "abcdefghijklmnopqrstuvwxyz",
	upper[ALPHA_LEN + NT_LEN] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
	symbol[SYMBOL_LEN + NT_LEN] = "`~!@#$%%^&*()-_=+[]\\{}|;':\",./<>? ";
	const char *extension = "", *filename = DEFAULT_FILENAME;
	char choice_set[NUM_LEN + (ALPHA_LEN << 1) + SYMBOL_LEN + NT_LEN] = DEFAULT_CHOICE_SET;

	if (argc > ARG_MAX) {
		printf("Usage: ./pwd-list-gen [-ha] [-l unsigned int] [-c Char set] <filename>\n");
		exit(EXIT_FAILURE);
	}

	int opt = NULL_OPT, entry_len = DEFAULT_ENTRY_LEN;
	bool from_zero = false;

	while ((opt = getopt(argc, argv, "hal:c:")) != -1) {
		switch (opt) {
		case 'h':
			printf("The default parameters are length = 8; Character set of Numbers, Upper, & Lower case; File type of .txt\n\n"
				"Usage: ./pwd-list-gen [-ha] [-l unsigned int] [-c Char set] <filename>\n\n"
				"\tOptions:\n\n"
				"\t-h\tHelp menu\n\n"
				"\t-a\tCreate passwords starting from length = 0 to specified length\n\n"
				"\t-l\tSet password length\n\n"
				"\t-c\tChoose character set (DEFAULT: NUM)\n"
				"\t\tu UPPER\n"
				"\t\tl LOWER\n"
				"\t\tp ALPHA\n"
				"\t\ta ALNUM\n"
				"\t\tw NUM + LOWER\n"
				"\t\te NUM + UPPER\n"
				"\t\ts ALNUM + SYMBOL\n");
			exit(EXIT_SUCCESS);
			break;
		case 'l':
			entry_len = atoi(optarg);
			if (entry_len < MIN_ENTRY_LEN) {
				printf("Password length must be one (1) or bigger\n");
				exit(EXIT_FAILURE);
			}
			break;
		case 'a':
			from_zero = true;
			break;
		case 'c':
			strncpy(choice_set, "", 1);
			switch (optarg[FIRST_ELEM]) {
			case 'u':
				strncat(choice_set, upper, ALPHA_LEN);
				break;
			case 'l':
				strncat(choice_set, lower, ALPHA_LEN);
				break;
			case 'p':
				strncat(choice_set, upper, ALPHA_LEN);
				strncat(choice_set, lower, ALPHA_LEN);
				break;
			case 'a':
				strncat(choice_set, num, NUM_LEN);
				strncat(choice_set, upper, ALPHA_LEN);
				strncat(choice_set, lower, ALPHA_LEN);
				break;
			case 'w':
				strncat(choice_set, num, NUM_LEN);
				strncat(choice_set, lower, ALPHA_LEN);
				break;
			case 'e':
				strncat(choice_set, num, NUM_LEN);
				strncat(choice_set, upper, ALPHA_LEN);
				break;
			case 's':
				strncat(choice_set, num, NUM_LEN);
				strncat(choice_set, upper, ALPHA_LEN);
				strncat(choice_set, lower, ALPHA_LEN);
				strncat(choice_set, symbol, SYMBOL_LEN);
				break;
			default:
				fprintf(stderr, "Error. Unrecognized character set option.\n");
				exit(EXIT_FAILURE);
			}
			break;
		default:
			exit(EXIT_FAILURE);
		}
	}

	printf("Password List Gen  Copyright (C) 2017  Elliott Sobek\n"
		"This program comes with ABSOLUTELY NO WARRANTY.\n"
		"This is free software, and you are welcome to redistribute it\n"
		"under certain conditions.\n");


	extension = strrchr(argv[argc - NT_LEN], '.');
	if (extension)
		if (strncmp(extension, ".txt", EXT_LEN) == 0)
			filename = argv[argc - NT_LEN];

	FILE *fp = fopen(filename, "w");

	if (from_zero)
		for (int i = MIN_ENTRY_LEN; i <= entry_len ; i++)
			gen_entries(choice_set, i, fp);
	else
		gen_entries(choice_set, entry_len, fp);

	fclose(fp);

	return 1;
}
