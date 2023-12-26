#ifndef RISKV_TRIE
#define RISKV_TRIE

#include <stdbool.h>

#define ALPHABET_SIZE 36

typedef struct TrieNode TrieNode;

struct TrieNode
{
	TrieNode* nodes[ALPHABET_SIZE];
	int isWord;
};

TrieNode* getNode(void);
void insertNode(TrieNode* node, char* word, int value);
void createTrie(TrieNode* node, char** words, int n);
int findWord(TrieNode* node, char* word);

#endif
