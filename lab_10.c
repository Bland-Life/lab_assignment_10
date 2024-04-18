#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORD_LENGTH 100
#define ALPHABET_LEN 26

// Trie structure
struct Trie {
    int count;
    struct Trie* alphabet[ALPHABET_LEN];
};

int readDictionary();
struct Trie* createTrie();
void insert();
int numberOfOccurences();
struct Trie* deallocateTrie();


// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word) {

    struct Trie* temp = pTrie;
    const int WORD_LEN = strlen(word);
    int letter_pos;
    
    // Traverses the Trie, expanding it whenever it encounters a NULL
    // position
    for (int i = 0; i < WORD_LEN; i++) {

        letter_pos = word[i] - 'a';

        if (temp->alphabet[letter_pos] == NULL) {
            temp->alphabet[letter_pos] = createTrie();
        }

        temp = temp->alphabet[letter_pos];
    }

    temp->count++;
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word) {

    struct Trie* temp = pTrie;
    const int WORD_LEN = strlen(word);
    int letter_pos;

    // Traverses the Trie, ending early only if it encounters NULL
    // at a position that should have a node for the word
    for (int i = 0; i < WORD_LEN; i++) {

        letter_pos = word[i] - 'a';
        if (temp->alphabet[letter_pos] == NULL) {
            return 0;
        }

        temp = temp->alphabet[letter_pos];
    }

    return temp->count;
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie) {

    // Recursive call on the children of the Trie node
    for (int i = 0; i < ALPHABET_LEN; i++) {

        if (pTrie->alphabet[i] != NULL) {
            deallocateTrie(pTrie->alphabet[i]);
        }
    }

    free(pTrie);
    pTrie = NULL;
    return pTrie;
}

// Initializes a trie structure
struct Trie *createTrie() {

    struct Trie* new = (struct Trie*) malloc(sizeof(struct Trie));
    if (new == NULL) {
        printf("Error allocating memory for Trie");
        return new;
    }

    new->count = 0;
    for (int i = 0; i < ALPHABET_LEN; i++) {
        new->alphabet[i] = NULL;
    }

    return new;
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords) {

    FILE* dictionary;
    if ((dictionary = fopen(filename, "r")) == NULL) {
        printf("Error opening dictionary.");
        return 0;
    }

    int num_of_words;
    fscanf(dictionary, "%d", &num_of_words);

    for (int i = 0; i < num_of_words; i++) {

        pInWords[i] = (char*) malloc(sizeof(char) * WORD_LENGTH);
        if (pInWords[i] == NULL) {
            printf("Error allocating allocating memory for words");
            return 0;
        }

        fscanf(dictionary, "%s", pInWords[i]);
    }

    return num_of_words;
}

int main(void) {

	char *inWords[256];
	
	//read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
	for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}
	
	struct Trie *pTrie = createTrie();
	for (int i=0;i<numWords;i++)
	{
		insert(pTrie, inWords[i]);
	}
	// parse lineby line, and insert each word to the trie data structure
	char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
	for (int i=0;i<5;i++)
	{
		printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
	}
	pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");
	return 0;
}