// This file is generated from trie_dict_gen.py to create 
// the trie during compile time and save space

#define N 26 // 26 lowercase english letters

struct TrieNode {
    char data;
    struct TrieNode *children[N];
    int is_leaf;
};    

#define S(NAME, ...) static struct TrieNode NAME = __VA_ARGS__

// trie_node structs -- aliased for smaller file size
S(care_leaf, { .data = 'e', .children = {}, .is_leaf = 1 }); 
S(cart_leaf, { .data = 't', .children = {}, .is_leaf = 1 }); 
S(eat_leaf, { .data = 't', .children = {}, .is_leaf = 1 }); 
S(car_leaf, { .data = 'r', .children = {}, .is_leaf = 1 }); 
S(cat_leaf, { .data = 't', .children = {}, .is_leaf = 1 }); 
S(care, { .data = 'e', .children = {[4] = &care_leaf}, .is_leaf = 0 }); 
S(cart, { .data = 't', .children = {[19] = &cart_leaf}, .is_leaf = 0 }); 
S(eat, { .data = 't', .children = {[19] = &eat_leaf}, .is_leaf = 0 }); 
S(car, { .data = 'r', .children = {[17] = &car_leaf, [4] = &care, [19] = &cart}, .is_leaf = 0 }); 
S(cat, { .data = 't', .children = {[19] = &cat_leaf}, .is_leaf = 0 }); 
S(ea, { .data = 'a', .children = {[19] = &eat}, .is_leaf = 0 }); 
S(ca, { .data = 'a', .children = {[17] = &car, [19] = &cat}, .is_leaf = 0 }); 
S(e, { .data = 'e', .children = {[0] = &ea}, .is_leaf = 0 }); 
S(c, { .data = 'c', .children = {[0] = &ca}, .is_leaf = 0 }); 
struct TrieNode root = { .children = {[4] = &e, [2] = &c}, .is_leaf = 0 };
