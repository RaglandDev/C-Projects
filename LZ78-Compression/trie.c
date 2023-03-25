#include "trie.h"
#include "code.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Constructor for TrieNode (on the heap)
// Sets code to index
// Sets each child node pointer to NULL
// Returns pointer to TrieNode node
TrieNode *trie_node_create(uint16_t code) {
    // Allocate memory for node
    TrieNode *node = (TrieNode *) malloc(sizeof(TrieNode));
    if (node == NULL) {
        perror("Could not allocate memory for TrieNode\n");
        return NULL;
    }

    // Set code
    node->code = code;

    // Set child node pointers to NULL
    for (int i = 0; i < ALPHABET; i++) {
        node->children[i] = NULL;
    }

    return node;
}

// "Deletes" n node
void trie_node_delete(TrieNode *n) {
    free(n);
}

// Initialize trie with root's code = EMPTY_CODE
// Returns pointer to initialized trie root
TrieNode *trie_create(void) {
    // Initialize root
    TrieNode *root_node = trie_node_create(EMPTY_CODE);

    // Initialization successful
    if (root_node != NULL) {
        return root_node;
    }
    // Initialization unsuccessful
    else {
        return NULL;
    }
}

// Resets trie to just the root
void trie_reset(TrieNode *root) {

    // Loop through each child
    for (int i = 0; i < ALPHABET; i++) {
        // If not leaf node
        if (root->children[i] != NULL) {
            trie_reset(root->children[i]);
            trie_node_delete(root->children[i]);
            root->children[i] = NULL;
        }
    }
}

// Recursively "deletes" subtree with root n
// Frees each child with trie_node_delete()
// Sets each child pointer to NULL
void trie_delete(TrieNode *n) {
    if (n == NULL) {
        return;
    }

    // Loop through each child
    for (int i = 0; i < ALPHABET; i++) {
        // If not leaf node
        if (n->children[i] != NULL) {
            // Recurse until leaf node
            trie_delete(n->children[i]);

            // Set pointer to null
            n->children[i] = NULL;
        }
    }
    // Delete node
    trie_node_delete(n);
}

// Returns pointer to child node representing symbol sym
TrieNode *trie_step(TrieNode *n, uint8_t sym) {
    if (n->children[sym] != NULL) {
        return n->children[sym];
    } else {
        return NULL;
    }
}
