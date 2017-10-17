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

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUM_LEN 10
#define ALPHA_LEN 26
#define SYMBOL_LEN 33
#define CHOICE_SET_MAX 95

#define EXT_LEN 4
#define DEFAULT_ENTRY_LEN 8

#define DEFAULT_CHOICE_SET "0123456789"
#define DEFAULT_FILENAME "list.txt"

int get_total_ammount_of_entries(void) {
	// strlen(choce_set)^entry_len;
	return 0;
}

int get_exact_filesize(void) { // In bytes
	// get_total_ammount_of_entries * 8;
	return 0;
}

char get_next_char(const char c, const char *const choice_set) {
	const int len_n = strlen(choice_set);

	for (int i = 0; i < len_n; i++)
		if (c == choice_set[i])
			return choice_set[i + 1];
	return '\0';
}

void gen_entries(char *choice_set, const int entry_len, FILE *fp) {
	const char *const choices = choice_set;
	const int len_n = strlen(choices);
	const char last_elem = choices[len_n - 1];
	char entry[entry_len + 1], end_entry[entry_len + 1];

	// Clear the arrays
	for (int i = 0; i < entry_len + 1; i++) {
		entry[i] = '\0';
		end_entry[i] = '\0';
	}

	// Init string/entry
	for (int i = 0; i < entry_len; i++) {
		entry[i] = choices[0];
		end_entry[i] = last_elem;
	}

	while (strncmp(entry, end_entry, entry_len) != 0) { // While entry not last entry

		// This loop only applies if you have acc -> baa
		for (int i = entry_len - 1; i > -1; i--) { // Go back down the entry list from the back

			if (entry[i] == last_elem) { // If the current entry index is the last elem

				if (entry[i - 1] == last_elem)
					continue;

				fprintf(fp, "%s\n", entry);
				entry[i - 1] = get_next_char(entry[i - 1], choices);

				for (int j = i; j < entry_len; j++) // Reset current index and forward ones to base choice
					entry[j] = choices[0];
			}
			break;
		}
		fprintf(fp, "%s\n", entry);
		entry[entry_len - 1] = get_next_char(entry[entry_len - 1], choices);
	}
	fprintf(fp, "%s\n", entry);
}

int main(const int argc, char *const argv[]) {
	const char num[NUM_LEN + 1] = "0123456789",
	lower[ALPHA_LEN + 1] = "abcdefghijklmnopqrstuvwxyz",
	upper[ALPHA_LEN + 1] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
	symbol[SYMBOL_LEN + 1] = "`~!@#$%%^&*()-_=+[]\\{}|;':\",./<>? ";
	const char *extension = "", *filename = DEFAULT_FILENAME;
	char choice_set[NUM_LEN + ALPHA_LEN + SYMBOL_LEN + 1] = DEFAULT_CHOICE_SET;

	if (argc > 7) {
		printf("Usage: ./pwd-list-gen [-ha] [-l unsigned int] [-c Char set] <filename>\n");
		exit(EXIT_FAILURE);
	}

	int opt = 0;
	int entry_len = DEFAULT_ENTRY_LEN;
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
				"\t-c\tChoose character set\n"
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
			if (entry_len < 1) {
				printf("Password length must be one (1) or bigger\n");
				exit(EXIT_FAILURE);
			}
			break;
		case 'a':
			from_zero = true;
			break;
		case 'c':
			strncpy(choice_set, "", 1);
			switch (optarg[0]) {
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


	extension = strrchr(argv[argc - 1], '.');
	if (extension)
		if (strncmp(extension, ".txt", EXT_LEN) == 0)
			filename = argv[argc - 1];

	FILE *fp = fopen(filename, "w");

	if (from_zero)
		for (int i = 1; i <= entry_len ; i++)
			gen_entries(choice_set, i, fp);
	else
		gen_entries(choice_set, entry_len, fp);

	fclose(fp);

	return 1;
}
