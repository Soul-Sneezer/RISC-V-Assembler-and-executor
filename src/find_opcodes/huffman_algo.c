#include <stdio.h>
#include <stdlib.h>
#include "huffman_algo.h"

struct MinHeapNode* newNode(char* data, unsigned freq) 
{ 
    struct MinHeapNode* temp = (struct MinHeapNode*)malloc( 
        sizeof(struct MinHeapNode)); 
  
    temp->left = temp->right = NULL; 
    temp->data = data; 
    temp->freq = freq; 
  
    return temp; 
} 
  
// A utility function to create 
// a min heap of given capacity 
struct MinHeap* createMinHeap(unsigned capacity) 
  
{ 
  
    struct MinHeap* min_heap 
        = (struct MinHeap*)malloc(sizeof(struct MinHeap)); 
  
    // current size is 0 
    min_heap->size = 0; 
  
    min_heap->capacity = capacity; 
  
    min_heap->array = (struct MinHeapNode**)malloc( 
        min_heap->capacity * sizeof(struct MinHeapNode*)); 
    return min_heap; 
} 
  
// A utility function to 
// swap two min heap nodes 
void swapMinHeapNode(struct MinHeapNode** a, 
                     struct MinHeapNode** b) 
  
{ 
  
    struct MinHeapNode* t = *a; 
    *a = *b; 
    *b = t; 
} 
  
// The standard minHeapify function. 
void minHeapify(struct MinHeap* min_heap, int idx) 
  
{ 
  
    int smallest = idx; 
    int left = 2 * idx + 1; 
    int right = 2 * idx + 2; 
  
    if (left < min_heap->size 
        && min_heap->array[left]->freq 
               < min_heap->array[smallest]->freq) 
        smallest = left; 
  
    if (right < min_heap->size 
        && min_heap->array[right]->freq 
               < min_heap->array[smallest]->freq) 
        smallest = right; 
  
    if (smallest != idx) { 
        swapMinHeapNode(&min_heap->array[smallest], 
                        &min_heap->array[idx]); 
        minHeapify(min_heap, smallest); 
    } 
} 
  
// A utility function to check 
// if size of heap is 1 or not 
int isSizeOne(struct MinHeap* min_heap) 
{ 
  
    return (min_heap->size == 1); 
} 
  
// A standard function to extract 
// minimum value node from heap 
struct MinHeapNode* extractMin(struct MinHeap* min_heap) 
  
{ 
  
    struct MinHeapNode* temp = min_heap->array[0]; 
    min_heap->array[0] = min_heap->array[min_heap->size - 1]; 
  
    --min_heap->size; 
    minHeapify(min_heap, 0); 
  
    return temp; 
} 
  
// A utility function to insert 
// a new node to Min Heap 
void insertMinHeap(struct MinHeap* min_heap, 
                   struct MinHeapNode* min_heap_node) 
  
{ 
  
    ++min_heap->size; 
    int i = min_heap->size - 1; 
  
    while (i 
           && min_heap_node->freq 
                  < min_heap->array[(i - 1) / 2]->freq) { 
  
        min_heap->array[i] = min_heap->array[(i - 1) / 2]; 
        i = (i - 1) / 2; 
    } 
  
    min_heap->array[i] = min_heap_node; 
} 
  
// A standard function to build min heap 
void buildMinHeap(struct MinHeap* min_heap) 
{ 
  
    int n = min_heap->size - 1; 
    int i; 
  
    for (i = (n - 1) / 2; i >= 0; --i) 
        minHeapify(min_heap, i); 
} 
  
// A utility function to print an array of size n 
void printArr(int arr[], int n) 
{ 
    int i; 
    for (i = 0; i < n; ++i) 
        printf("%d", arr[i]); 
  
    printf("\n"); 
} 
  
// Utility function to check if this node is leaf 
int isLeaf(struct MinHeapNode* root) 
  
{ 
  
    return !(root->left) && !(root->right); 
} 
  
// Creates a min heap of capacity 
// equal to size and inserts all character of 
// data[] in min heap. Initially size of 
// min heap is equal to capacity 
struct MinHeap* createAndBuildMinHeap(char* data[], 
                                      int freq[], int size) 
  
{ 
  
    struct MinHeap* min_heap = createMinHeap(size); 
  
    for (int i = 0; i < size; ++i) 
        min_heap->array[i] = newNode(data[i], freq[i]); 
  
    min_heap->size = size; 
    buildMinHeap(min_heap); 
  
    return min_heap; 
} 
  
// The main function that builds Huffman tree 
struct MinHeapNode* buildHuffmanTree(char* data[], 
                                     int freq[], int size) 
  
{ 
    struct MinHeapNode *left, *right, *top; 
  
    // Step 1: Create a min heap of capacity 
    // equal to size.  Initially, there are 
    // modes equal to size. 
    struct MinHeap* min_heap 
        = createAndBuildMinHeap(data, freq, size); 
  
    // Iterate while size of heap doesn't become 1 
    while (!isSizeOne(min_heap)) { 
  
        // Step 2: Extract the two minimum 
        // freq items from min heap 
        left = extractMin(min_heap); 
        right = extractMin(min_heap); 
  
        // Step 3:  Create a new internal 
        // node with frequency equal to the 
        // sum of the two nodes frequencies. 
        // Make the two extracted node as 
        // left and right children of this new node. 
        // Add this node to the min heap 
        // '$' is a special value for internal nodes, not 
        // used 
        top = newNode("$", left->freq + right->freq); 
  
        top->left = left; 
        top->right = right; 
  
        insertMinHeap(min_heap, top); 
    } 
  
    // Step 4: The remaining node is the 
    // root node and the tree is complete. 
    return extractMin(min_heap); 
} 
  
// Prints huffman codes from the root of Huffman Tree. 
// It uses arr[] to store codes 
void printCodes(struct MinHeapNode* root, int arr[], 
                int top) 
  
{ 
  
    // Assign 0 to left edge and recur 
    if (root->left) { 
  
        arr[top] = 0; 
        printCodes(root->left, arr, top + 1); 
    } 
  
    // Assign 1 to right edge and recur 
    if (root->right) { 
  
        arr[top] = 1; 
        printCodes(root->right, arr, top + 1); 
    } 
  
    // If this is a leaf node, then 
    // it contains one of the input 
    // characters, print the character 
    // and its code from arr[] 
    if (isLeaf(root)) { 
  
        printf("%s: ", root->data); 
        printArr(arr, top); 
    } 
} 
  
// The main function that builds a 
// Huffman Tree and print codes by traversing 
// the built Huffman Tree 
void HuffmanCodes(char* data[], int freq[], int size) 
  
{ 
    // Construct Huffman Tree 
    struct MinHeapNode* root 
        = buildHuffmanTree(data, freq, size); 
  
    // Print Huffman codes using 
    // the Huffman tree built above 
    int arr[MAX_TREE_HT], top = 0; 
  
    printCodes(root, arr, top); 
} 
