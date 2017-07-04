/**/

#include <stdio.h>
#include <string.h>

/*
#include <unistd.h>
#include <stdlib.h>

while (getopt(argc, argv, "hp:c:") != 0) {
	switch () {
		case 'h':
			break;
		case 'p':
			break
		case 'c':
			break
		default:
			fprintf(stderr, "Error\n");
			exit(EXIT_FAILURE);
	}
}

FILE *fp = NULL;

fp = fopen("pwd-list.txt", "w");

fputs(entry, fp);

fclose(fp);
*/

char get_next_char(const char c) {
	const char * const choices = "abc";

	for (int i = 0; i < 3; i++)
		if (c == 'c')
			return choices[0];
		else if (c == choices[i])
			return choices[i + 1];
	return '\0';
}

int main(void) {
	const char * const choices = "abc";
	const int len_n = strlen(choices);
	const char last_elem = choices[len_n - 1];
	char entry[4] = {'0', '0', '0', '\0'}, end_entry[4] = {'0', '0', '0', '\0'};

	for (int i = 0; i < len_n; i++) { // Init string/entry
		entry[i] = choices[0]; // "aaa"
		end_entry[i] = last_elem; // "ccc"
	}

	// printf("Entry: %s\n", entry); // TMP
	// printf("This is strlen: %d\n", len_n); // TMP, 3
	// printf("This is end_entry: %s\n\n", end_entry); // TMP

	int b = 0;

	// entry[0] = 'a';
	// entry[1] = 'b';
	// entry[2] = 'c';

	char bob = '0';

	while (strncmp(entry, end_entry, len_n) != 0 && b != 32) { // While entry not last entry

		// This loop only applies if you have acc -> baa
		for (int i = len_n - 1; i > -1; i--) { // Go back down the entry list from the back

			if (entry[i] == last_elem) { // If the current entry index is the last elem

				if (entry[i - 1] == last_elem)
					continue;

				// for (int j = i; j < len_n; j++) // Run over previous elements and check if they are also last_elem
				// 	if (entry[j] != last_elem)
				// 		goto NOT;

				printf("%s\n", entry);
				bob = get_next_char(entry[i - 1]);
				entry[i - 1] = bob;
				// entry[i - 1] = get_next_char(entry[i - 1]);

				for (int j = i; j < len_n; j++) // Reset current index and forward ones to base choice
					entry[j] = choices[0];
				break;

			} else
				break;
		}
		printf("%s\n", entry);
		bob = get_next_char(entry[len_n - 1]);
		entry[len_n - 1] = bob;
		// entry[len_n - 1] = get_next_char(entry[len_n - 1]); // PROB here now, acc
		b++;
	}
	printf("%s\n", entry);

	return 1;
}

/*Print, then check?*/

/* If entry[i] == last_elem
else
break
make current index elem goto next choice
*/
