/**/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

/*

fp = fopen("test.txt", "w");

fputs(entry, fp);

fclose(fp);
*/

char get_next_char(const char c) {
	const char *const choices = "abc";
	const int len_n = strlen(choices);

	for (int i = 0; i < len_n; i++)
		if (c == choices[i])
			return choices[i + 1];
	return '\0';
}

int main(const int argc, char *const argv[]) {
	int opt;

	while ((opt = getopt(argc, argv, "hp:c:")) != -1) {
		switch (opt) {
		case 'h':
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

	const FILE *fp = NULL;

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

				printf("%s\n", entry);
				entry[i - 1] = get_next_char(entry[i - 1]);

				for (int j = i; j < len_n; j++) // Reset current index and forward ones to base choice
					entry[j] = choices[0];
				break;

			} else
				break;
		}
		printf("%s\n", entry);
		entry[len_n - 1] = get_next_char(entry[len_n - 1]);
	}
	printf("%s\n", entry);

	return 1;
}
