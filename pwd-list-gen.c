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
#include <libgen.h>
#include <stdbool.h>
#include <pthread.h>

#define NUM_LEN 10
#define ALPHA_LEN 26
#define SYMBOL_LEN 33

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

#define NUMS "0123456789"
#define LOWER "abcdefghijklmnopqrstuvwxyz"
#define UPPER "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define SYMBOL "`~!@#$%%^&*()-_=+[]\\{}|;':\",./<>? "
#define DEFAULT_FILENAME "list.txt"
#define DEFAULT_CHOICE_SET "0123456789"

unsigned long long _entry_count = 0;

bool _from_zero = false, _fs_flag = false, _quiet_flag = false;

unsigned long long get_total_entries(const char *const choice_set, const int entry_len) {
	return (unsigned int) pow((double) strnlen(choice_set, MAX_STR_LEN), (double) entry_len);
}

void *process_time_stats(void *total_entries) {
	const unsigned long long t_entries = (unsigned long long) total_entries;
	unsigned long long pentry_count = 0;
	unsigned int entry_ratio = 0;
	double percent_done = 0;

	while (_entry_count != t_entries) {
		sleep(30);
		percent_done = ((double) _entry_count / (double) t_entries) * 100;
		entry_ratio = (_entry_count - pentry_count) / 30;

		printf("%llu of %llu entries generated (%u entries/s). %.2f%% finished\n",
			_entry_count, t_entries, entry_ratio, percent_done);
		pentry_count = _entry_count;
	}
	pthread_exit(NULL);
}

void compute_flags(int *const entry_len, char *const choice_set, const int argc, char *const argv[]) {
	int opt = NULL_OPT;

	while ((opt = getopt(argc, argv, "hagql:c:")) != -1) {
		switch (opt) {
		case 'h':
			printf("The default parameters are length = 8; Character set of Numbers, Upper, & Lower case; File type of .txt\n\n"
				"Usage: %s [-hagq] [-l unsigned int] [-c Char set] <filename>\n\n"
				"\tOptions:\n\n"
				"\t-h\tHelp menu\n\n"
				"\t-a\tCreate passwords starting from length = 0 to specified length\n\n"
				"\t-g\tDisplay only the estimated filesize\n\n"
				"\t-q\tQuiet; Do not output to screen\n\n"
				"\t-l\tSet password length (DEFAULT: 8)\n\n"
				"\t-c\tChoose character set (DEFAULT: NUM)\n"
				"\t\tu UPPER\n"
				"\t\tl LOWER\n"
				"\t\tp ALPHA\n"
				"\t\ta ALNUM\n"
				"\t\tw NUM + LOWER\n"
				"\t\te NUM + UPPER\n"
				"\t\ts ALNUM + SYMBOL\n", basename(argv[0]));
			exit(EXIT_SUCCESS);
			break;
		case 'l':
			*entry_len = atoi(optarg);
			if (*entry_len < MIN_ENTRY_LEN) {
				printf("Password length must be one (1) or bigger\n");
				exit(EXIT_FAILURE);
			}
			break;
		case 'a':
			_from_zero = true;
			break;
		case 'g':
			_fs_flag = true;
			break;
		case 'q':
			_quiet_flag = true;
			break;
		case 'c':
			strncpy(choice_set, "", 1);
			switch (optarg[FIRST_ELEM]) {
			case 'u':
				strncat(choice_set, UPPER, ALPHA_LEN);
				break;
			case 'l':
				strncat(choice_set, LOWER, ALPHA_LEN);
				break;
			case 'p':
				strncat(choice_set, UPPER, ALPHA_LEN);
				strncat(choice_set, LOWER, ALPHA_LEN);
				break;
			case 'a':
				strncat(choice_set, NUMS, NUM_LEN);
				strncat(choice_set, UPPER, ALPHA_LEN);
				strncat(choice_set, LOWER, ALPHA_LEN);
				break;
			case 'w':
				strncat(choice_set, NUMS, NUM_LEN);
				strncat(choice_set, LOWER, ALPHA_LEN);
				break;
			case 'e':
				strncat(choice_set, NUMS, NUM_LEN);
				strncat(choice_set, UPPER, ALPHA_LEN);
				break;
			case 's':
				strncat(choice_set, NUMS, NUM_LEN);
				strncat(choice_set, UPPER, ALPHA_LEN);
				strncat(choice_set, LOWER, ALPHA_LEN);
				strncat(choice_set, SYMBOL, SYMBOL_LEN);
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
}

char *get_estimated_filesize(char *const buffer, const char *const choice_set, const int entry_len) {
	const char fs_units[6] = {'B', 'K', 'M', 'G', 'T', 'P'};
	const unsigned long long data_denom[6] = {1, 1024, 1024000, 1024000000, 1024000000000, 1024000000000000};
	const double fs_size = (get_total_entries(choice_set, entry_len) * (entry_len + 1));
	double reduced_fs = 0;

	for (int i = 5; i > -1; i--) {
		reduced_fs = fs_size / data_denom[i];

		if (reduced_fs > 1) {
			snprintf(buffer, 7, "%.1f%c", reduced_fs, fs_units[i]);
			memcpy(buffer, buffer, 6);
			return buffer;
		}
	}
	return "0B";
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
				_entry_count++;
				entry[i - PREV_INDEX] = get_next_char(entry[i - PREV_INDEX], choices);

				for (int j = i; j < entry_len; j++) // Reset current index and forward ones to base choice
					entry[j] = choices[FIRST_ELEM];
			}
			break;
		}
		fprintf(fp, "%s\n", entry);
		_entry_count++;
		entry[entry_len - PREV_INDEX] = get_next_char(entry[entry_len - PREV_INDEX], choices);
	}
	fprintf(fp, "%s\n", entry);
	_entry_count++;
}

int main(const int argc, char *const argv[]) {

	if (argc > ARG_MAX) {
		printf("Usage: %s [-hagq] [-l unsigned int] [-c Char set] <filename>\n", basename(argv[0]));
		exit(EXIT_FAILURE);
	}

	const char *extension = "", *filename = DEFAULT_FILENAME;
		  char choice_set[NUM_LEN + (ALPHA_LEN << 1) + SYMBOL_LEN + NT_LEN] = DEFAULT_CHOICE_SET,
			   fs_buf[7] = "";
	int entry_len = DEFAULT_ENTRY_LEN;
	pthread_t pthread_id = -1;

	compute_flags(&entry_len, choice_set, argc, argv);

	if (!_quiet_flag)
		printf("Password List Gen  Copyright (C) 2017  Elliott Sobek\n"
			"This program comes with ABSOLUTELY NO WARRANTY.\n"
			"This is free software, and you are welcome to redistribute it\n"
			"under certain conditions.\n\n"
			"The estimated file size will be: %s\n", get_estimated_filesize(fs_buf, choice_set, entry_len));

	if (_fs_flag)
		exit(EXIT_SUCCESS);

	extension = strrchr(argv[argc - NT_LEN], '.'); // Extract process as function?
	if (extension)
		if (strncmp(extension, ".txt", EXT_LEN) == 0)
			filename = argv[argc - NT_LEN];

	FILE *fp = fopen(filename, "w");

	if (!_quiet_flag)
		pthread_create(&pthread_id, NULL, &process_time_stats, (void *) get_total_entries(choice_set, entry_len));

	if (_from_zero)
		for (int i = MIN_ENTRY_LEN; i <= entry_len ; i++)
			gen_entries(choice_set, i, fp);
	else
		gen_entries(choice_set, entry_len, fp);

	fclose(fp);

	if (!_quiet_flag) {
		pthread_cancel(pthread_id);
		pthread_join(pthread_id, NULL);
	}

	return 0;
}
