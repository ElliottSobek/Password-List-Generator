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
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <libgen.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <termios.h>
#include <sys/stat.h>
#include <sys/types.h>

#define NL_LEN 1
#define NT_LEN 1
#define SECOND 1
#define PERCENT 1
#define EXT_LEN 4
#define NUM_LEN 10
#define ARG_MAX 11
#define ALPHA_LEN 26
#define FIRST_ELEM 0
#define NEXT_INDEX 1
#define PREV_INDEX 1
#define FS_OUT_LEN 6
#define SYMBOL_LEN 33
#define MAX_STR_LEN 98
#define NULL_STR_LEN -1
#define MIN_ENTRY_LEN 1
#define DATA_DENOM_LEN 6
#define KEY_SPACE_CODE 32
#define DEFAULT_ENTRY_LEN 8

#define NUMS "0123456789"
#define DEFAULT_CHOICE_SET NUMS
#define LOWER "abcdefghijklmnopqrstuvwxyz"
#define UPPER "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define SYMBOL "`~!@#$^&*()-_=+[]{}|;':,./<>? %\\\""

#define BYTE_S 1L
#define KBYTE_S 1024L
#define MBYTE_S ((unsigned long long) (KBYTE_S * KBYTE_S))
#define GBYTE_S ((unsigned long long) (MBYTE_S * KBYTE_S))
#define TBYTE_S ((unsigned long long) (GBYTE_S * KBYTE_S))
#define PBYTE_S ((unsigned long long) (TBYTE_S * KBYTE_S))

unsigned long long _entry_count = 0;

bool _from_zero = false, _fs_flag = false, _quiet_flag = false, _file_flag = false,
	_min_flag = false;

void init_kb_intterupt(struct termios kb_config) {
	kb_config.c_lflag &= ~(ICANON | ECHO);
	kb_config.c_cc[VMIN] = BYTE_S;
	kb_config.c_cc[VTIME] = SECOND * 0;

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &kb_config);
}

void *enable_pause_feature(void *const restrict kb_config) {
	init_kb_intterupt(*((struct termios *) kb_config));

	while (1) {
		if (getchar() == KEY_SPACE_CODE)
			printf("Hit space\n");
	}
	return NULL;
}

void *process_time_stats(void *const restrict total_entries) {
	const unsigned long long t_entries = (unsigned long long) total_entries;
	unsigned long long pentry_count = 0;
	unsigned long entry_ratio;
	const unsigned short denominator = SECOND * 30, hundred_percent = PERCENT * 100;
	double percent_done;

	while (1) {
		sleep(30);
		percent_done = ((double) _entry_count / (double) t_entries) * hundred_percent;
		entry_ratio = (_entry_count - pentry_count) / denominator;

		printf("%llu of %llu entries generated (%lu entries/s). %.2f%% finished\n",
			_entry_count, t_entries, entry_ratio, percent_done);
		pentry_count = _entry_count;
	}
	return NULL;
}

void compute_flags(short *const restrict entry_len, short *const restrict min_len,
	char *const restrict filename, char *const restrict choice_set,
	const unsigned int argc, char *const argv[]) {

	int opt;

	while ((opt = getopt(argc, argv, "hagql:L:c:f:")) != -1) {
		switch (opt) {
		case 'h':
			printf("Usage: %s [-hagq] [-L unsigned int] [-l unsigned int] [-c Char set] [-f filename]\n\n"
				"\tOptions:\n\n"
				"\t-h\tHelp menu\n\n"
				"\t-a\tCreate passwords starting from length = 0 to specified length; Overrides -L\n\n"
				"\t-g\tDisplay only the estimated filesize\n\n"
				"\t-q\tQuiet; Do not output to screen\n\n"
				"\t-L\tSet minimum password length; Can't be larger than max length\n\n"
				"\t-l\tSet maximum password length (DEFAULT: 8)\n\n"
				"\t-c\tChoose character set (DEFAULT: NUM)\n"
				"\t\tu UPPER\n"
				"\t\tl LOWER\n"
				"\t\tp ALPHA\n"
				"\t\ta ALNUM\n"
				"\t\tw NUM + LOWER\n"
				"\t\te NUM + UPPER\n"
				"\t\ts ALNUM + SYMBOL\n\n"
				"\t-f\tWrite output to a specifed file (must be txt)\n",
				basename(argv[FIRST_ELEM]));
			exit(EXIT_SUCCESS);
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
		case 'l':
			*entry_len = atoi(optarg);
			if (*entry_len < MIN_ENTRY_LEN) {
				fprintf(stderr, "Password length must be one (1) or bigger\n");
				exit(EXIT_FAILURE);
			}
			break;
		case 'L':
			_min_flag = true;
			*min_len = atoi(optarg);
			if (*min_len < MIN_ENTRY_LEN) {
				fprintf(stderr, "Password length must be one (1) or bigger\n");
				exit(EXIT_FAILURE);
			}
			break;
		case 'c':
			switch (optarg[FIRST_ELEM]) {
			case 'u':
				choice_set[FIRST_ELEM] = '\0';
				strncat(choice_set, UPPER, ALPHA_LEN);
				break;
			case 'l':
				choice_set[FIRST_ELEM] = '\0';
				strncat(choice_set, LOWER, ALPHA_LEN);
				break;
			case 'p':
				choice_set[FIRST_ELEM] = '\0';
				strncat(choice_set, UPPER, ALPHA_LEN);
				strncat(choice_set, LOWER, ALPHA_LEN);
				break;
			case 'a':
				strncat(choice_set, UPPER, ALPHA_LEN);
				strncat(choice_set, LOWER, ALPHA_LEN);
				break;
			case 'w':
				strncat(choice_set, LOWER, ALPHA_LEN);
				break;
			case 'e':
				strncat(choice_set, UPPER, ALPHA_LEN);
				break;
			case 's':
				strncat(choice_set, UPPER, ALPHA_LEN);
				strncat(choice_set, LOWER, ALPHA_LEN);
				strncat(choice_set, SYMBOL, SYMBOL_LEN);
				break;
			default:
				fprintf(stderr, "Error. Unrecognized character set option.\n");
				exit(EXIT_FAILURE);
			}
			break;
		case 'f':
			_file_flag = true;
			strncpy(filename, optarg, PATH_MAX);
			break;
		default:
			exit(EXIT_FAILURE);
		}
	}
}

char *get_estimated_filesize(char *const restrict buffer, const double fs_size) {
	const char fs_units[DATA_DENOM_LEN] = {'B', 'K', 'M', 'G', 'T', 'P'};
	const unsigned long long data_denom[DATA_DENOM_LEN] = {BYTE_S, KBYTE_S, MBYTE_S, GBYTE_S, TBYTE_S, PBYTE_S};
	double reduced_fs;

	for (int i = DATA_DENOM_LEN - 1; i > -1; i--) {
		reduced_fs = fs_size / data_denom[i];

		if (reduced_fs > 1) {
			snprintf(buffer, FS_OUT_LEN + NT_LEN, "%.1f%c", reduced_fs, fs_units[i]);
			return buffer;
		}
	}
	return "0B";
}

char get_next_char(const char c, const char *const restrict choice_set) {
	const size_t len_n = strnlen(choice_set, MAX_STR_LEN);

	for (unsigned int i = 0; i < len_n; i++)
		if (c == choice_set[i])
			return choice_set[i + NEXT_INDEX];
	return '\0';
}

void gen_entries(char *restrict choice_set, const unsigned short entry_len, const int fd) {
	const char *const restrict choices = choice_set; // Why choices to choice_set?
	const size_t len_n = strnlen(choices, MAX_STR_LEN);
	const char last_elem = choices[len_n - NT_LEN];
	char entry[entry_len + NT_LEN], end_entry[entry_len + NT_LEN];

	// Clear the arrays
	for (int i = 0; i < entry_len + NT_LEN; i++) { // Cant unsign due to NT_LEN being int
		entry[i] = '\0';
		end_entry[i] = '\0';
	}

	// Init string/entry
	for (unsigned int i = 0; i < entry_len; i++) {
		entry[i] = choices[FIRST_ELEM];
		end_entry[i] = last_elem;
	}

	while (strncmp(entry, end_entry, entry_len) != 0) { // While entry not last entry

		// This loop only applies if you have acc -> baa
		for (int i = entry_len - NT_LEN; i > NULL_STR_LEN; i--) { // Go back down the entry list from the back

			if (entry[i] == last_elem) { // If the current entry index is the last elem

				if (entry[i - PREV_INDEX] == last_elem)
					continue;

				dprintf(fd, "%s\n", entry);
				_entry_count++;
				entry[i - PREV_INDEX] = get_next_char(entry[i - PREV_INDEX], choices);

				for (unsigned int j = i; j < entry_len; j++) // Reset current index and forward ones to base choice
					entry[j] = choices[FIRST_ELEM];
			}
			break;
		}
		dprintf(fd, "%s\n", entry);
		_entry_count++;
		entry[entry_len - PREV_INDEX] = get_next_char(entry[entry_len - PREV_INDEX], choices);
	}
	dprintf(fd, "%s\n", entry);
	_entry_count++;
}

int main(const int argc, char *const argv[]) {

	if (argc > ARG_MAX) {
		printf("Usage: %s [-hagq] [-L unsigned int] [-l unsigned int] "
			"[-c Char set] [-f filename]\n",
			basename(argv[FIRST_ELEM]));
		exit(EXIT_FAILURE);
	}

	const char *restrict extension;
		  char fs_buf[FS_OUT_LEN + NT_LEN], filename[PATH_MAX + NT_LEN],
		  choice_set[NUM_LEN + (ALPHA_LEN << 1) + SYMBOL_LEN + NT_LEN] = DEFAULT_CHOICE_SET;
	short entry_len = DEFAULT_ENTRY_LEN, min_len;
	unsigned long long entry_amt, total_entries = 0;
	double fs_size = 0;
	pthread_t update_tid, kb_tid;
	const mode_t f_mode = 0664;
	struct termios kb_config;

	compute_flags(&entry_len, &min_len, filename, choice_set, argc, argv);

	if (min_len > entry_len) {
		fprintf(stderr, "Minimum length must be larger than maximum length\n");
		exit(EXIT_FAILURE);
	}

	if (!_min_flag && !_from_zero)
		min_len = entry_len;
	else if (_from_zero)
		min_len = MIN_ENTRY_LEN;

	for (int i = min_len; i <= entry_len; i++) {
		entry_amt = (unsigned long long) pow((double) strnlen(choice_set, MAX_STR_LEN), (double) i);
		total_entries += entry_amt;
		fs_size += entry_amt * (i + NL_LEN);
	}

	if (!isatty(STDIN_FILENO))
		_quiet_flag = true;

	if (!_quiet_flag)
		printf("Password List Gen  Copyright (C) 2017  Elliott Sobek\n"
			"This program comes with ABSOLUTELY NO WARRANTY.\n"
			"This is free software, and you are welcome to redistribute it\n"
			"under certain conditions.\n\n"
			"Total entries: %llu\n"
			"The estimated file size will be: %s\n", total_entries,
			get_estimated_filesize(fs_buf, fs_size));

	if (_fs_flag)
		exit(EXIT_SUCCESS);

	tcgetattr(STDIN_FILENO, &kb_config);
	if (pthread_create(&kb_tid, NULL, &enable_pause_feature, (void*) &kb_config) != 0) {
		perror(strerror(errno));
		exit(EXIT_FAILURE);
	}

	if (!_quiet_flag)
		if (pthread_create(&update_tid, NULL, &process_time_stats, (void *) total_entries) != 0) {
			perror(strerror(errno));
			exit(EXIT_FAILURE);
		}

	if (_file_flag) {
		extension = strrchr(filename, '.');
		if (extension) {
			if (strncmp(extension, ".txt", EXT_LEN) != 0) {
				fprintf(stderr, "File must be textfile (.txt)\n");
				exit(EXIT_FAILURE);
			}
		} else {
			fprintf(stderr, "File must have extension\n");
			exit(EXIT_FAILURE);
		}

		const int fd = open(filename, O_CREAT | O_WRONLY, f_mode);

		if (fd == -1) {
			fprintf(stderr, "Error: %s for %s\n", strerror(errno), filename);
			exit(EXIT_FAILURE);
		}

		for (int i = min_len; i <= entry_len; i++)
			gen_entries(choice_set, i, fd);
		close(fd);
	} else
		for (int i = min_len; i <= entry_len; i++)
			gen_entries(choice_set, i, STDOUT_FILENO);

	if (!_quiet_flag) {
		pthread_cancel(update_tid);
		pthread_join(update_tid, NULL);
	}

	pthread_cancel(kb_tid);
	pthread_join(kb_tid, NULL);

	tcsetattr(STDIN_FILENO, TCSANOW, &kb_config);

	return 0;
}
