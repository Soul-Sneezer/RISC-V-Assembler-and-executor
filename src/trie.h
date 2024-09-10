#ifndef RISKV_TRIE
#define RISKV_TRIE

#include <stdint.h>
#include <stdbool.h>

#define ALPHABET_SIZE 36

typedef struct TrieNode TrieNode;

struct TrieNode
{
	TrieNode* nodes[ALPHABET_SIZE];
	int32_t isWord;
};

TrieNode* getNode(void);
void insertNode(TrieNode* node, char* word, int32_t value);
void createTrie(TrieNode* node, char** words, int32_t n);
int32_t findWord(TrieNode* node, char* word);

#endif
