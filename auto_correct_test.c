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

#define DICT_NAME "words.txt"
#define DICT_SIZE ((int)get_dict_size())

int get_dict_size() {
    FILE *fp;
    int count = 0;
    char c;
    fp = fopen(DICT_NAME, "r");
    while ((c = fgetc(fp)) != EOF) {
        if (c == '\n') {
            ++count;
        }
    }
    fclose(fp);
    return count;
}

#define WORD_LIST ((char **)get_dict())

char **get_dict() {
    const int max_string_len = 35; // WARNING: 35 is the max length string
    FILE *fp = fopen(DICT_NAME, "r");

    // get the file size to alloc
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char **dict = malloc(sizeof(int *) * DICT_SIZE);
    int loop_counter = 0;
    char currentline[max_string_len];
    while (fgets(currentline, sizeof(currentline), fp) != NULL) {
        currentline[strlen(currentline) - 1] = '\0';
        char *string = malloc(sizeof(currentline));
        strcpy(string, currentline);
        dict[loop_counter] = string;
        ++loop_counter;
    }
    fclose(fp);

    return dict;
}

#define N 26 // 26 letters - english

struct TrieNode {
    char data; // Storing for printing purposes only
    struct TrieNode *children[N];
    int is_leaf;
};

struct TrieNode *make_trienode(char data) {
    // Allocate memory for a TrieNode
    struct TrieNode *node = (struct TrieNode *)calloc(1, sizeof(struct TrieNode));
    for (int i = 0; i < N; i++)
        node->children[i] = NULL;
    node->is_leaf = 0;
    node->data = data;
    return node;
}

void free_trienode(struct TrieNode *node) {
    // Free the trienode sequence
    for (int i = 0; i < N; i++) {
        if (node->children[i] != NULL) {
            free_trienode(node->children[i]);
        } else {
            continue;
        }
    }
    free(node);
}

struct TrieNode *insert_trie(struct TrieNode *root, char *word) {
    // Inserts the word onto the Trie
    // ASSUMPTION: The word only has lower case characters
    struct TrieNode *temp = root;

    for (int i = 0; word[i] != '\0'; i++) {
        // Get the relative position in the alphabet list
        int idx = (int)word[i] - 'a';
        if (temp->children[idx] == NULL) {
            // If the corresponding child doesn't exist,
            // simply create that child!
            temp->children[idx] = make_trienode(word[i]);
        } else {
            // Do nothing. The node already exists
        }
        // Go down a level, to the child referenced by idx
        // since we have a prefix match
        temp = temp->children[idx];
    }
    // At the end of the word, mark this node as the leaf node
    temp->is_leaf = 1;
    return root;
}

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

#define DICT_TIRE (build_trie())

struct TrieNode *build_trie() {
    struct TrieNode *root = make_trienode('\0');
    for (int i = 0; i < DICT_SIZE; ++i) {
        root = insert_trie(root, WORD_LIST[i]);
    }
    return root;
};

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
    for (int i = 0; i < DICT_SIZE; ++i) {
        printf("%s", WORD_LIST[i]);
    }

    struct TrieNode *root = DICT_TIRE;
    print_search(root, "peer");

    return 0;
}
