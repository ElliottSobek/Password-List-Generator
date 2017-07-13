/**/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

char get_next_char(const char c, const char *const choice_set) {
	const int len_n = strlen(choice_set);

	for (int i = 0; i < len_n; i++)
		if (c == choice_set[i])
			return choice_set[i + 1];
	return '\0';
}

void gen_entries(char *choice_set, const int entry_len, FILE *fp) {
	const char *const choices = strncpy(choice_set, choice_set, strlen(choice_set));
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
				printf("%s\n", entry);
				entry[i - 1] = get_next_char(entry[i - 1], choices);

				for (int j = i; j < entry_len; j++) // Reset current index and forward ones to base choice
					entry[j] = choices[0];
				break;

			} else
				break;
		}
		fprintf(fp, "%s\n", entry);
		printf("%s\n", entry);
		entry[entry_len - 1] = get_next_char(entry[entry_len - 1], choices);
	}
	fprintf(fp, "%s\n", entry);
	printf("%s\n", entry);
}

int main(const int argc, char *const argv[]) {
	const char num[11] = "0123456789",
	lower[27] = "abcdefghijklmnopqrstuvwxyz",
	upper[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
	symbol[34] = "`~!@#$%%^&*()-_=+[]\\{}|;':\",./<>? ";
	char choice_set[96] = "0123456789";

	if ((argc < 2) || (argc > 8)) {
		printf("Usage: ./a [-he] [-c Char set] [-p num] <filename>\n");
		exit(EXIT_FAILURE);
	}

	char opt;
	int entry_len = 8;
	bool from_zero = false;

	while ((opt = getopt(argc, argv, "hal:c:")) != -1) {
		switch (opt) {
		case 'h':
			printf("The default parameters are length = 8; Character set of Numbers, Upper, & Lower case; File type of .txt\n\n"
				"Usage: ./a [-ha] [-l unsigned int] [-c Char set] <filename>\n\n"
				"\tCommands:\n"
				"\t-h\tHelp menu\n\n"
				"\t-l\tSet password length\n\n"
				"\t-c\tChoose character set\n\n"
				"\t-a\tCreate passwords starting from length = 0 to specified length\n\n"
				"\t-v\tChoose file type to .csv\n");
			exit(EXIT_SUCCESS);
			break;
		case 'l':
			entry_len = atoi(optarg);
			break;
		case 'a':
			from_zero = true;
			break;
		case 'c':
			strncpy(choice_set, "", 1);
			switch (optarg[0]) {
			case 'n':
				strncat(choice_set, num, 10);
				break;
			case 'u':
				strncat(choice_set, upper, 26);
				break;
			case 'l':
				strncat(choice_set, lower, 26);
				break;
			case 'p':
				strncat(choice_set, upper, 26);
				strncat(choice_set, lower, 26);
				break;
			case 'a':
				strncat(choice_set, num, 10);
				strncat(choice_set, upper, 26);
				strncat(choice_set, lower, 26);
				break;
			case 'w':
				strncat(choice_set, num, 10);
				strncat(choice_set, lower, 26);
				break;
			case 'e':
				strncat(choice_set, num, 10);
				strncat(choice_set, upper, 26);
				break;
			case 's':
				strncat(choice_set, num, 10);
				strncat(choice_set, upper, 26);
				strncat(choice_set, lower, 26);
				strncat(choice_set, symbol, 33);
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

	if (entry_len < 1) {
		printf("Password length must be one (1) or bigger\n");
		exit(EXIT_FAILURE);
	}

	FILE *fp = fopen(argv[argc - 1], "w");

	if (from_zero)
		for (int i = 1; i <= entry_len ; i++)
			gen_entries(choice_set, i, fp);
	else
		gen_entries(choice_set, entry_len, fp);

	fclose(fp);

	return 1;
}
