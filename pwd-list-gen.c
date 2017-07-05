/**/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

char get_next_char(const char c) {
	const char *const choices = "abc";
	const int len_n = strlen(choices);

	for (int i = 0; i < len_n; i++)
		if (c == choices[i])
			return choices[i + 1];
	return '\0';
}

int main(const int argc, char *const argv[]) {

	if (argc < 2) {
		printf("Usage: ./a [-he] [-c Char set] [-p num] <filename>\n");
		exit(EXIT_FAILURE);
	}

	int opt;

	while ((opt = getopt(argc, argv, "hp:c:")) != -1) {
		switch (opt) {
		case 'h':
			printf("The default parameters are length = 8; Character set of Numbers, Upper, & Lower case; File type of .txt\n\n"
				"Usage: ./a [-he] [-c Char set] [-p num] <filename>\n\n"
				"\tCommands:\n"
				"\t-h\tHelp menu\n\n"
				"\t-c\tChoose character set\n\n"
				"\t-p\tCreate passwords starting from length = 0 to specified length\n\n"
				"\t-v\tChoose file type to .csv\n");
			break;
		case 'p':
			break;
		case 'c':
			break;
		default:
			fprintf(stderr, "Error\n");
			exit(EXIT_FAILURE);
		}
	}

	FILE *fp = fopen(argv[argc - 1], "w");

	const char *const choices = "abc";
	const int len_n = strlen(choices);
	const char last_elem = choices[len_n - 1];
	char entry[4], end_entry[4];

	for (int i = 0; i < len_n; i++) { // Init string/entry
		entry[i] = choices[0]; // "aaa"
		end_entry[i] = last_elem; // "ccc"
	}

	while (strncmp(entry, end_entry, len_n) != 0) { // While entry not last entry

		// This loop only applies if you have acc -> baa
		for (int i = len_n - 1; i > -1; i--) { // Go back down the entry list from the back

			if (entry[i] == last_elem) { // If the current entry index is the last elem

				if (entry[i - 1] == last_elem)
					continue;

				fprintf(fp, "%s\n", entry);
				printf("%s\n", entry);
				entry[i - 1] = get_next_char(entry[i - 1]);

				for (int j = i; j < len_n; j++) // Reset current index and forward ones to base choice
					entry[j] = choices[0];
				break;

			} else
				break;
		}
		fprintf(fp, "%s\n", entry);
		printf("%s\n", entry);
		entry[len_n - 1] = get_next_char(entry[len_n - 1]);
	}
	fprintf(fp, "%s\n", entry);
	printf("%s\n", entry);
	fclose(fp);

	return 1;
}
