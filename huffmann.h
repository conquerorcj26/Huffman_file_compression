#include <stdlib.h>
#include <stdbool.h> 
#define MAX_SIZE 256

// Function to read input file contents and count character frequencies
void countFrequencies(FILE* input_file, int freq[]) {
    int ch = 0;
    while ((ch = fgetc(input_file)) != EOF) {
        unsigned char uc = (unsigned char)ch;
        freq[uc]++;
        //printf("Character '%c' (ASCII %d) Frequency: %d\n", uc, uc, freq[uc]);
    }
    rewind(input_file);
}

//Matching frequency and character arrays
int fillArrays(int freq1[], char characters[], int freq[]) {
    int character_count = 0;

    // Fill the characters array with the characters having non-zero frequencies
    for (int i = 0; i < MAX_SIZE; i++) {
        if (freq1[i] > 0) {
            freq[character_count] = freq1[i];
            characters[character_count] = (char)i;
            character_count++;
        }
    }
    return character_count;
}

// Structure for tree nodes
struct Node {
    char character;
    int freq;
    struct Node *l, *r;
};
 
// Structure for min heap
struct Min_Heap {
    int size;
    struct Node** array;
};

//Function for a new node
struct Node* newNode(char character, int freq) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->character = character;
    node->freq = freq;
    node->l = NULL;
    node->r = NULL;
    return node;
}

//Function to heapify the min_heap
void Heapify(struct Min_Heap* min_heap, int i) {
    int smallest = i;             // Initialize smallest as root
    int l = 2 * i + 1;         // l child index
    int r = 2 * i + 2;        // r child index

    // If l child is smaller than root
    if (l < min_heap->size && min_heap->array[l]->freq < min_heap->array[smallest]->freq)
        smallest = l;

    // If r child is smaller than smallest so far
    if (r < min_heap->size && min_heap->array[r]->freq < min_heap->array[smallest]->freq)
        smallest = r;

    // If smallest is not root
    if (smallest != i) {
        // Swap the pointers in heap array
        struct Node* temp = min_heap->array[i];
        min_heap->array[i] = min_heap->array[smallest];
        min_heap->array[smallest] = temp;

        // Recursively heapify the affected sub-tree
        Heapify(min_heap, smallest);
    }
}

// Function to create min heap
struct Min_Heap* createAndBuildMin_Heap(char arr[],int freq[],int unique_size)
{
	int i;

	// Initializing heap
	struct Min_Heap* Min_Heap = (struct Min_Heap*)malloc(sizeof(struct Min_Heap));
	Min_Heap->size = unique_size;
	Min_Heap->array = (struct Node**)malloc(Min_Heap->size * sizeof(struct Node*));

	// Initializing the array of pointers in minheap.
	// Pointers pointing to new nodes of character
	// and their frequency
	for (i = 0; i < unique_size; ++i) {

		// newNode is a function
		// to initialize new node
		Min_Heap->array[i] = newNode(arr[i], freq[i]);
	}

	int n = Min_Heap->size - 1;
	for (i = (n - 1) / 2; i >= 0; --i) {

		// Standard function for Heap creation
		Heapify(Min_Heap, i);
	}

	return Min_Heap;
}

// Function to insert a node into the min heap
void insertIntoMin_Heap(struct Min_Heap* minHeap, struct Node* node) {
    ++minHeap->size;
    int i = minHeap->size - 1;
    // Adjust the position of the new node to maintain min heap property
    while (i && node->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = node;
}

// Function to extract the minimum element from the min heap
struct Node* extractMinFromMin_Heap(struct Min_Heap* minHeap) {
    struct Node* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    // Call heapify to maintain min heap property
    Heapify(minHeap, 0);
    return temp;
}

// Function to check if the size of the min heap is one
bool isSizeOne(struct Min_Heap* minHeap) {
    return (minHeap->size == 1);
}

// Function to build Huffman Tree
struct Node* buildHuffmanTree(struct Min_Heap* Min_Heap)
{
    struct Node *l, *r, *top;
    while (!isSizeOne(Min_Heap)) {
        l = extractMinFromMin_Heap(Min_Heap);
        r = extractMinFromMin_Heap(Min_Heap);
        top = newNode('$', l->freq + r->freq);
        top->l = l;
        top->r = r;
        insertIntoMin_Heap(Min_Heap, top);
    }
    return extractMinFromMin_Heap(Min_Heap);
}

//Function to traverse and generate code
void generateCodes(struct Node* root, char* code, char codes[][MAX_SIZE]) {
    if (root == NULL)
        return;

    // If leaf node is reached, store the code
    if (!root->l && !root->r) {
        codes[(int)(root->character)][0] = '\0';
        strcat(codes[(int)(root->character)], code);
        return;
    }

    // Traverse left
    if (root->l) {
        int len = strlen(code);
        code[len] = '0';
        code[len + 1] = '\0';
        generateCodes(root->l, code, codes);
        code[len] = '\0'; // Backtrack
    }

    // Traverse right
    if (root->r) {
        int len = strlen(code);
        code[len] = '1';
        code[len + 1] = '\0';
        generateCodes(root->r, code, codes);
        code[len] = '\0'; // Backtrack
    }
}

//Function to print contents of a tree
void printTree(struct Node* root) {
    if (root == NULL) {
        return;
    }

    // Print the contents of the current node
    printf("Character: %c, Frequency: %d\n", root->character, root->freq);

    // Recursively print the left subtree
    printTree(root->l);

    // Recursively print the right subtree
    printTree(root->r);
}

//Function to print contents of minheap
void printMinHeap(struct Min_Heap* minHeap) {
    printf("Min Heap Contents:\n");
    for (int i = 0; i < minHeap->size; ++i) {
        printf("Index: %d, Character: %c, Frequency: %d\n", i, minHeap->array[i]->character, minHeap->array[i]->freq);
    }
}

//Function to decompress a file
void decompress(FILE* output_file, FILE* decompressed_file, struct Node* root) {
    struct Node* current = root;
    int bit;

    while ((bit = fgetc(output_file)) != EOF) {
        // Traverse the Huffman tree based on the encoded bit stream
        if (bit == '0') {
            current = current->l;
        } else if (bit == '1') {
            current = current->r;
        } else {
            // Invalid bit, handle error
            fprintf(stderr, "Invalid bit encountered during decompression.\n");
            return;
        }

        // If leaf node is reached, output the character and reset to root
        if (!current->l && !current->r) {
            fputc(current->character, decompressed_file);
            current = root; // Reset to root for next character
        }
    }
}