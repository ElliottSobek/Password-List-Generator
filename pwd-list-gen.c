#include <stdio.h>
#include <string.h>

int main(void) {
	const char * const choices = "abc";
	char entry[4] = {'0', '0', '0', '\0'};
	int len_n = strlen(choices);
	const char last_elem = choices[len_n - 1];

	for (int i = 0; i < len_n; i++) // Init string/entry
		entry[i] = choices[0]; // "aaa"

	printf("Entry: %s\n", entry); // TMP
	printf("This is strlen: %d\n", len_n); // TMP, 3

	int b = 0;

	while (strncmp(entry, "ccc", 3) != 0 && b != 3) { // While entry not last entry

		for (int i = len_n - 1; i > -1; i--) { // Go back down the entry list from the back

			if (entry[i] == last_elem) { // If the current entry index is the last elem
				printf("%s\n", entry);
				entry[i] = choices[i];

				for (int j = i; j < len_n; j++) // Reset current index and forward ones to base choice
					entry[j] = choices[0];
			} else {
				printf("%s\n", entry);
				entry[i] = choices[len_n - i]; // Increment current index choice to the next one
				break;
			}
		}
		b++;
	}

	return 0;
}

/* If entry[i] == last_elem
else
break
make current index elem goto next choice
*/
