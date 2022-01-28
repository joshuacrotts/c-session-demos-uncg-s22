#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 1024

static int qsort_string_comparator(const void *a, const void *b);
static int binary_search(char **array, int a_len, char *key);

/**
 * Problem: Read in a file of words. Sort those files using qsort with a comparator for strings. Then,
 *          prompt the user to enter a word. Binary search the list of words and return the index. If 
 *          they enter Q, quit the program. Be aware that this list has over 460,000 words, so we want 
 *          any operations to be efficient! What is the time complexity of binary search? What is the time
 *          complexity of the randomized quick-sort?
 * 
 * Source: https://raw.githubusercontent.com/dwyl/english-words/master/words.txt
 */
int main(int argc, char *argv[]) {
    // Read data into a file.
    char buffer[MAX_BUFFER_SIZE];
    FILE *file = fopen("words.txt", "r");

    int num_words;
    int idx = 0;

    // Read the number of words in the file (should be the first line).
    int ret = fscanf(file, "%i\n", &num_words);
    if (ret == 0 || ret == EOF) {
        fprintf(stderr, "Failed to read in number of words\n");
	exit(1);
    }

    // Read in each word line by line and store it in the array.
    char **words = malloc(sizeof(char *) * num_words);
    if (words == NULL) {
        fprintf(stderr, "Failed to malloc ** char\n");
	exit(1);
    }

    while (fgets(buffer, sizeof buffer, file) != NULL) {
	buffer[strlen(buffer)-1] = '\0';
        words[idx] = malloc(strlen(buffer) + 1);
        if (words[idx] == NULL) {
            fprintf(stderr, "Failed to malloc space for word\n");
	    exit(1);
	}
	strcpy(words[idx], buffer);
	idx++;
    }

    // Sort the array with the custom comparator.
    qsort(words, num_words, sizeof(char *), qsort_string_comparator);
    
    // Read in a string from stdin and search for it. Make sure to null terminate the string!
    while (1) {
	printf("Enter a word: \n");
        memset(buffer, 0, sizeof buffer);
        if (fgets(buffer, sizeof buffer, stdin)) {
	    buffer[strlen(buffer)-1] = '\0';
            printf("Word is at index %i\n", binary_search(words, num_words, buffer));
    	} else {
            printf("Please enter some text!\n");
	}
    }

    // Free the word array and close the FILE *.
    for (int i = 0; i < num_words; i++) {
	free(words[i]);
    }

    free(words);
    fclose(file);
}

static int qsort_string_comparator(const void *a, const void *b) {
    return strcmp(*(const char **) a, *(const char **) b);
}

static int binary_search(char **array, int a_len, char *key) {
    int left = 0;
    int right = a_len - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
	int cmpVal = strcmp(array[mid], key);
	if (cmpVal == 0) {
	    return mid;
	} else if (cmpVal < 0) {
            left = mid + 1;
	} else {
            right = mid - 1;
	}
    }
    return -1;
}

