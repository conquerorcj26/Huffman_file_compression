#include <stdio.h>    //standard input output
#include <stdlib.h>   //standard library
#include <string.h>   //string opertions
#include "huffmann.h" //huffmann functions as header file
#define MAX_SIZE 256

int main(){
    printf("START\n");

    // Read input file contents and count character frequencies
    FILE *input_file = fopen("input.txt", "r");
    if (input_file == NULL) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }
    printf("OPENED IN FILE\n");

    int freq1[MAX_SIZE] = {0}; // for easy count of frequency then transferred to an array of frequencies 'freq'
    countFrequencies(input_file, freq1);
    printf("COUNTED FREQ\n");
    
    // Create arrays to store characters and frequencies
    char characters[MAX_SIZE];
    int freq[MAX_SIZE] = {0};

    // Fill characters and freq arrays using the fillArrays function ,it returns character count
    int character_count = fillArrays(freq1, characters,freq);
    /*for (int i=0;i<character_count;i++){
        printf("ASCII [%d]; Charcter [%c]: Freuency [%d]\n",i,characters[i],freq[i]);
    }*/
    printf("MATCHED ARRAYS\n");

    struct Min_Heap* min_heap = createAndBuildMin_Heap(characters,freq,character_count);
    //printMinHeap(min_heap);
    printf("MIN_HEAP FORMED\n");

    struct Node* tree = buildHuffmanTree(min_heap);
    //printf("Tree contents : \n");
    //printTree(tree);
    printf("TREE FORMED\n");

    char code[MAX_SIZE];
    char codes[MAX_SIZE][MAX_SIZE];
    code[0] = '\0'; // Initialize code string
    generateCodes(tree,code,codes);
    printf("CODES FORMED\n");

    //opening output file
    FILE *output_file = fopen("output.txt", "w");
    if (output_file == NULL) {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }
    printf("OPENED OUT FILE\n");

    // Write compressed data to the output file
    rewind(input_file);
    int ch;
    while ((ch = fgetc(input_file)) != EOF) {
        //printf("%c : %s\n",ch,codes[ch]);
        if (fprintf(output_file, "%s", codes[ch]) < 0) {
            perror("Error writing to output file");
            exit(EXIT_FAILURE);
        }
    }

    fclose(output_file);
    fclose(input_file);

    printf("COMPRESSED\n");
    printf("START DECOMPRESSION\n");

    // Read output file contents 
    FILE *output_file_again = fopen("output.txt", "r");
    if (output_file_again == NULL) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }
    printf("OPENED OUT FILE\n");

    //opening decompression file
    FILE *decompress_file = fopen("decompress_file.txt", "w");
    if (decompress_file == NULL) {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }
    printf("OPENED DECOMPRESSION FILE\n");

    decompress(output_file_again, decompress_file, tree);
    printf("DECOMPRESSED");

    fclose(decompress_file);
    fclose(output_file_again);
    return 0;
}