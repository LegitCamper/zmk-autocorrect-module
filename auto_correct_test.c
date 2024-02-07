/*
 * Steps:
 * - Every english word
 * - Search list and replace if close to 80% match - or num set in Kconfig
 * - Replace if last charecter is space or puctuation
 * - For faster lookup in large dict create map with addesses of each first char a,b,c...
 * - create trie and store on flash to avoid creation on startup - during compile time
 * - dict will not include names
 * - to cancel a correction remove the space or puctutation and it will automatically replace the
 * previous word
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "trie_dict.h"

#define N 26 // 26 lowercase english letters

// struct TrieNode {
//     char data;
//     struct TrieNode *children[N];
//     int is_leaf;
// };

int search_trie(struct TrieNode *root, char *word) {
    // Searches for word in the Trie
    struct TrieNode *temp = root;

    for (int i = 0; word[i] != '\0'; i++) {
        int position = word[i] - 'a';
        if (temp->children[position] == NULL)
            return 0;
        temp = temp->children[position];
    }
    if (temp != NULL && temp->is_leaf == 1)
        return 1;
    return 0;
}

void print_search(struct TrieNode *root, char *word) {
    printf("Searching for %s: ", word);
    if (search_trie(root, word) == 0)
        printf("Not Found\n");
    else
        printf("Found!\n");
}

typedef char word[15];

struct LetterTable {
    char letter;
    int location;
};

struct CorrectionBuffer {
    word last_word;                   // used if user reverts correction
    const struct TrieNode *root_node; // stores dict trie
    struct LetterTable list[N];       // list of top level letter locations
    char **frequent_words[25];        // 25 words will be cached for fast search of mispelled words
    // TODO: maybe have struct which ranks each word in trie tree and rank by usage ^^^^^
};

// inits buffer to be used in callbacks
struct CorrectionBuffer buffer = {
    .last_word = "",
    .list = {},
    .frequent_words = {},
};

int main() {
    print_search(&root, "car");
    for (long unsigned int i = 0; i < sizeof(root.children); ++i) {
        printf("%i", root.children[i]->data);
    }

    return 0;
}
