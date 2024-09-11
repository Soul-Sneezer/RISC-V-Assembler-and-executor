#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"
#include "common.h"
#include "scanner.h"

static int32_t alphabetIndex(char c)
{
	if(c >= '0' && c <= '9')
		return 26 + (c - '0');
	else if(c >= 'a' && c <= 'z')
		return c - 'a';
	else
		return -1;
}

void insertNode(TrieNode* node, char* word, int32_t value)
{
		TrieNode* copy = node;
		int32_t n = strlen(word);
		for(int32_t i = 0; i < n; i++)
		{
			if(alphabetIndex(word[i]) == -1)
				return;
		}
		for(int32_t i = 0; i < n; i++)
		{
			int32_t index = alphabetIndex(word[i]);
			if(copy->nodes[index] == NULL)
			{
				copy->nodes[index] = (TrieNode*)malloc(sizeof(TrieNode));
				copy->nodes[index]->isWord = -1;
			}
			copy = copy->nodes[index];
			
		}

		copy->isWord = value; // true is -2, false is -1

}

TrieNode* createTrie(char** words, int32_t n)
{
	TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
	node->isWord = -1;

	for(int32_t i = 0; i < n; i++)
	{
		insertNode(node, words[i], -2);
	}

	return node;
}

void freeTrie(TrieNode* trie)
{
}


int32_t findWord(TrieNode* node, char* word)
{
	for(int32_t i = 0; word[i] != '\0'; i++)
	{
		int32_t index = alphabetIndex(word[i]);
		if(index == -1)
			return -1;
		if(node->nodes[index] == NULL)
			return -1;
		node = node->nodes[index];
	}

	return node->isWord;
}
