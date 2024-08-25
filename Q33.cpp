#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100
#define MAX_WORDS 10000

typedef struct {
    char word[MAX_WORD_LENGTH];
    int frequency;
} WordFreq;

void to_lowercase(char *str) {
    for (; *str; ++str) *str = tolower(*str);
}

void replace_punctuation_with_space(char *str) {
    char *p = str;
    while (*p) {
        if (ispunct((unsigned char)*p)) {
            *p = ' ';
        }
        p++;
    }
}

int find_or_add_word(WordFreq *word_freqs, int *word_count, char *word) {
    for (int i = 0; i < *word_count; i++) {
        if (strcmp(word_freqs[i].word, word) == 0) {
            return i;
        }
    }
    strcpy(word_freqs[*word_count].word, word);
    word_freqs[*word_count].frequency = 0;
    return (*word_count)++;
}

int compare_freq(const void *a, const void *b) {
    WordFreq *wf_a = (WordFreq *)a;
    WordFreq *wf_b = (WordFreq *)b;
    return wf_b->frequency - wf_a->frequency;
}

void find_frequent_words(const char *path, int n) {
    FILE *file = fopen(path, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    WordFreq word_freqs[MAX_WORDS];
    int word_count = 0;
    char word[MAX_WORD_LENGTH];

    while (fscanf(file, "%99s", word) == 1) {
        to_lowercase(word);
        replace_punctuation_with_space(word);

        char *token = strtok(word, " ");
        while (token) {
            int index = find_or_add_word(word_freqs, &word_count, token);
            word_freqs[index].frequency++;
            token = strtok(NULL, " ");
        }
    }

    fclose(file);

    qsort(word_freqs, word_count, sizeof(WordFreq), compare_freq);

    printf("The %d most frequent words are:\n", n);
    for (int i = 0; i < n && i < word_count; i++) {
        printf("%s: %d\n", word_freqs[i].word, word_freqs[i].frequency);
    }
}

int main() {
    const char *path = "D://Dataset.txt";  
    int n;

    printf("Enter the number of most frequent words to find: ");
    scanf("%d", &n);

    find_frequent_words(path, n);

    return 0;
}

