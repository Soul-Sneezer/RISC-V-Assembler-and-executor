#define MAX_TREE_HT 100

struct MinHeapNode
{
	char* data;
	unsigned int freq;
	struct MinHeapNode *left, *right;
};

struct MinHeap
{
	unsigned int size;
	unsigned int capacity;
	struct MinHeapNode** array;
};

void HuffmanCodes(char* data[], int freq[], int size);
