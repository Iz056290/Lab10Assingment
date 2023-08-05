#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie structure
struct TrieNode
{
    struct TrieNode *children[26];
     int count;
};

struct TrieNode *createTrieNode()
{
    struct TrieNode *node = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    if (node)
    {
        for (int i = 0; i < 26; ++i)
        {
            node->children[i] = NULL;
        }
        node->count = 0;
    }
    return node;
}

struct Trie
{	
   struct TrieNode *root;
};

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    struct TrieNode *current = pTrie->root;
    for (int i = 0; word[i] != '\0'; ++i)
    {
        int index = word[i] - 'a';
        if (current->children[index] == NULL)
        {
            current->children[index] = createTrieNode();
        }
        current = current->children[index];
    }
    current->count++;
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    struct TrieNode *current = pTrie->root;
    for (int i = 0; word[i] != '\0'; ++i)
    {
        int index = word[i] - 'a';
        if (current->children[index] == NULL)
        {
            return 0;
         }
        current = current->children[index];
    }
       if (current->count > 0)
    {
        return current->count;
    }
    return 0;
}

void deallocateTrieNode(struct TrieNode *node)
{
    if (node)
    {
        for (int i = 0; i < 26; ++i)
        {
            if (node->children[i])
            {
                deallocateTrieNode(node->children[i]);
            }
        }
        free(node);
    }
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
    if (pTrie)
    {
        deallocateTrieNode(pTrie->root);
        free(pTrie);
    }
    return NULL;
}

// Initializes a trie structure
struct Trie *createTrie()
{
    struct Trie* newTrie = (struct Trie*)malloc(sizeof(struct Trie));
    if (newTrie)
    {
        newTrie->root = createTrieNode();
        if (newTrie->root == NULL)
        {
            printf("Error creating trie node. Exiting...\n");
            free(newTrie);
            return NULL;
        }
    }
    return newTrie;
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
      FILE* fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Error opening file: %s\n", filename);
        exit(1);
    }

    int numWords = 0;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        buffer[strcspn(buffer, "\r\n")] = '\0'; // Remove newline characters
        pInWords[numWords] = strdup(buffer);
        numWords++;
    }

    fclose(fp);
    return numWords;
}

int main(void)
{
	char *inWords[256];
	
	//read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
	for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}
	
	struct Trie *pTrie = createTrie();

     if (pTrie->root == NULL)
    {
        printf("Error creating trie. Exiting...\n");
        deallocateTrie(pTrie);
        return 1;
    }

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