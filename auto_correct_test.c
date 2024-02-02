#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int WORD_SIZE = 15;

int get_dict_size(char *filename) {
    FILE *fp;
    int count = 0;
    char c;
    fp = fopen(filename, "r");
    while ((c = fgetc(fp)) != EOF) {
        if (c == '\n') {
            ++count;
        }
    }
    fclose(fp);
    return count;
}

char **get_dict(char *filename, const int count) {
    const int max_string_len = 35; // WARNING: 35 is the max length string
    FILE *fp = fopen(filename, "r");

    // get the file size to alloc
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char **dict = malloc(sizeof(int *) * count);
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

void print_trie(struct TrieNode *root) {
    // Prints the nodes of the trie
    if (!root)
        return;
    struct TrieNode *temp = root;
    printf("%c -> ", temp->data);
    for (int i = 0; i < N; i++) {
        print_trie(temp->children[i]);
    }
}

void print_search(struct TrieNode *root, char *word) {
    printf("Searching for %s: ", word);
    if (search_trie(root, word) == 0)
        printf("Not Found\n");
    else
        printf("Found!\n");
}

typedef char word[10];
typedef struct TrieNode TrieNode;

struct CorrectionBuffer {
    word last_word;
    TrieNode nodes;
};

// inits buffer to be used in callbacks
static struct CorrectionBuffer buffer;

int main() {
    char *dict_name = "words.txt";
    int dict_size = get_dict_size(dict_name);
    printf("%i\n", dict_size);
    char **dict = get_dict(dict_name, dict_size);
    for (int i = 0; i < dict_size; ++i) {
        printf("%s\n", dict[i]);
    }

    // buffer.nodes = *make_trienode('\0');
    // TrieNode *root = &buffer.nodes;
    // root = insert_trie(root, "hello");
    // root = insert_trie(root, "hi");
    // root = insert_trie(root, "teabag");
    // root = insert_trie(root, "teacan");
    // print_search(root, "tea");
    // print_search(root, "teabag");
    // print_search(root, "teacan");
    // print_search(root, "hi");
    // print_search(root, "hey");
    // print_search(root, "hello");
    // print_trie(root);
    // free_trienode(&buffer.nodes);
    return 0;
}
