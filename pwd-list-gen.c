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
		if (c == choices[i])
			return choices[i + 1];
	return '\0';
}

int main(void) {
	const char * const choices = "abc";
	const int len_n = strlen(choices);
	const char last_elem = choices[len_n - 1];
	char entry[4] = {'0', '0', '0', '\0'}, end_entry[4] = {'0', '0', '0', '\0'};

	for (int i = 0; i < len_n; i++) // Init string/entry
		entry[i] = choices[0]; // "aaa"
	for (int i = 0; i < len_n; i++)
		end_entry[i] = last_elem;

	printf("Entry: %s\n", entry); // TMP
	printf("This is strlen: %d\n", len_n); // TMP, 3
	printf("This is end_entry: %s\n\n", end_entry); // TMP

	int b = 0;

	while (strncmp(entry, end_entry, 3) != 0 && b != 9) { // While entry not last entry

		for (int i = len_n - 1; i > -1; i--) { // Go back down the entry list from the back

			if (entry[i] == last_elem) { // If the current entry index is the last elem
				for (int j = i; j < len_n; j++)
					if (entry[j] != last_elem)
						goto NOT;
				printf("%s\n", entry);
				entry[i - 1] = get_next_char(entry[i - 1]);

				for (int j = i; j < len_n; j++) // Reset current index and forward ones to base choice
					entry[j] = choices[0];
				NOT: break;
			}
		}
		printf("%s\n", entry);
		entry[len_n - 1] = get_next_char(entry[len_n - 1]); // PROB here now, acc
		b++;
	}

	return 0;
}

/*Print, then check?*/

/* If entry[i] == last_elem
else
break
make current index elem goto next choice
*/
