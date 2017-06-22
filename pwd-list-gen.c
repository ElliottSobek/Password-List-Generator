#include <stdio.h>
#include <string.h>

int main(void) {
	const char * const choices = "abc";
	char entry[4] = {'0', '0', '0', '\0'};
	int len_n = strlen(choices);
	char const last_elem = choices[len_n - 1];

	for (int i = 0; i < len_n; i++) // Init string/entry
		entry[i] = choices[0]; // "aaa"

	printf("Entry: %s\n", entry); // TMP
	printf("This is strlen: %d\n", len_n); // TMP

	while (strncmp(entry, "ccc", 3) != 0) { // While entry not last entry

		for (int i = len_n; i > 0; i--) { // Go back down the entry list from the back

			if (entry[i - 1] == last_elem) { // If the current entry index is the last elem
				printf("%s\n", entry);
				entry[i - 1] = choices[i];

				for (int j = i; j < len_n; j++) // Reset current index and forward ones to base choice
					entry[j] = choices[0];
			}
			printf("%s\n", entry);
			entry[i] = choices[(len_n - 1) - i]; // Invrement current index choice to the next one

			break; // TMP

		}

		break; // TMP

	}

	return 0;
}
