#include <stdio.h>
#include <string.h>

char get_next_char(char c) {
	const char * const choices = "abc";

	for (int i = 0; i < 3; i++)
		if (c == choices[i])
			return choices[i + 1];
	return '\0';
}

int main(void) {
	const char * const choices = "abc";
	char entry[4] = {'0', '0', '0', '\0'};
	const int len_n = strlen(choices);
	const char last_elem = choices[len_n - 1];

	for (int i = 0; i < len_n; i++) // Init string/entry
		entry[i] = choices[0]; // "aaa"

	printf("Entry: %s\n", entry); // TMP
	printf("This is strlen: %d\n\n", len_n); // TMP, 3

	int b = 0;

	while (strncmp(entry, "ccc", 3) != 0 && b != 9) { // While entry not last entry

		for (int i = len_n - 1; i > -1; i--) { // Go back down the entry list from the back

			if (entry[i] == last_elem) { // If the current entry index is the last elem
				printf("%s\n", entry);
				entry[i - 1] = get_next_char(entry[i - 1]);

				for (int j = i; j < len_n; j++) // Reset current index and forward ones to base choice
					entry[j] = choices[0];
				break;
			}
		}
		printf("%s\n", entry);
		entry[len_n - 1] = get_next_char(entry[len_n - 1]);
		b++;
	}

	return 0;
}

/* If entry[i] == last_elem
else
break
make current index elem goto next choice
*/
