/**/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

char get_next_char(const char c, const char *const choices) {
	const int len_n = strlen(choices);

	for (int i = 0; i < len_n; i++)
		if (c == choices[i])
			return choices[i + 1];
	return '\0';
}

int main(const int argc, char *const argv[]) {
	const char num[11] = "0123456789",
	lower[27] = "abcdefghijklmnopqrstuvwxyz",
	upper[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
	symbol[34] = "`~!@#$%%^&*()-_=+[]\\{}|;':\",./<>? ";
	char choicess[96] = "0123456";

	if ((argc < 2) || (argc > 8)) {
		printf("Usage: ./a [-he] [-c Char set] [-p num] <filename>\n");
		exit(EXIT_FAILURE);
	}

	int opt;
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
			strncpy(choicess, "", 1);
			switch (optarg[0]) {
			case 'n':
				strncat(choicess, num, 10);
				break;
			case 'u':
				strncat(choicess, upper, 26);
				break;
			case 'l':
				strncat(choicess, lower, 26);
				break;
			case 'p':
				strncat(choicess, upper, 26);
				strncat(choicess, lower, 26);
				break;
			case 'a':
				strncat(choicess, num, 10);
				strncat(choicess, upper, 26);
				strncat(choicess, lower, 26);
				break;
			case 'w':
				strncat(choicess, num, 10);
				strncat(choicess, lower, 26);
				break;
			case 'e':
				strncat(choicess, num, 10);
				strncat(choicess, upper, 26);
				break;
			case 's':
				strncat(choicess, num, 10);
				strncat(choicess, upper, 26);
				strncat(choicess, lower, 26);
				strncat(choicess, symbol, 33);
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

	// Start code section?

	const char *const choices = strncpy(choicess, choicess, strlen(choicess));
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

	// End code section?
	fclose(fp);

	return 1;
}
