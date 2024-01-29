#include <stdlib.h>

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/led.h>
#include <zephyr/kernel.h>
#include <zephyr/init.h>

#include <zmk/ble.h>
#include <zmk/endpoints.h>
#include <zmk/keymap.h>
#include <zmk/split/bluetooth/peripheral.h>
#include <zmk/battery.h>
#include <zmk/event_manager.h>
#include <zmk/events/keycode_state_changed.h>
#include <zmk/events/layer_state_changed.h>

#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

const struct device *dev = zmk_behavior_get_binding(binding->behavior_dev);
struct behavior_auto_correct_data *data = dev->correction_buffer ;

// inits main struct
static int auto_correct_init(const struct device *dev) {
    correction_buffer;
    return 0;
}

// manages new keycodes presses
ZMK_LISTENER(behavior_auto_correct, auto_correct_keycode_changed);
ZMK_SUBSCRIPTION(behavior_auto_correct, zmk_position_state_changed);
int keycode_state_changed(const zmk_event_t *eh) {
    
}

struct TrieNode {
    // The Trie Node Structure
    // Each node has N children, starting from the root
    // and a flag to check if it's a leaf node
    char data; // Storing for printing purposes only
    TrieNode* children[26];
    int is_leaf;
};

TrieNode* make_trienode(char data) {
    // Allocate memory for a TrieNode
    TrieNode* node = (TrieNode*) calloc (1, sizeof(TrieNode));
    for (int i=0; i<26; i++)
        node->children[i] = NULL;
    node->is_leaf = 0;
    node->data = data;
    return node;
}

void free_trienode(TrieNode* node) {
    // Free the trienode sequence
    for(int i=0; i<26; i++) {
        if (node->children[i] != NULL) {
            free_trienode(node->children[i]);
        }
        else {
            continue;
        }
    }
    free(node);
}

typedef char word[10];

typedef struct TrieNode TrieNode;

struct correction_buffer {
  word last_word;
  TrieNode nodes;
};


