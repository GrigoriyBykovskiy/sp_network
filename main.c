#define OBMPIMAGENAME "bmp_encrypt.bmp"
#define OOBMPIMAGENAME "bmp_decrypt.bmp"
#define CSVNAME "output.csv"
#define BYTE_MAX_VALUE 256
#define BIT_BYTE 8
#include <stdio.h>
#include <time.h>
#include "SPN.h"
int user_input(char* filename, unsigned int data_length, unsigned int rounds_count){
        /*some exception for user input must be here*/
    srand(time(NULL));
    TKEY key;
    init_key(&key, data_length * rounds_count);
    print_key(&key);
    TBOXES boxes;
    init_boxes(&boxes);
    int count = 1;
    for (int i = 0; i < data_length * rounds_count; i++) {
        if ( i % data_length == 0){
            printf("\nRound %d\n", count);
            count++;
        }
        b_add_sbox(&boxes, BYTE_MAX_VALUE);
        print_sbox(boxes.sboxes[i]);
    }
    for (int i = 0; i < rounds_count; i++) {
        printf("\nRound %d\n", i+1);
        b_add_pbox(&boxes, data_length * BIT_BYTE);
        print_pbox(boxes.pboxes[i]);
    }
    encrypt_8(filename, OBMPIMAGENAME, &boxes, &key, rounds_count, data_length);
    decrypt_8(OBMPIMAGENAME, OOBMPIMAGENAME, &boxes, &key, rounds_count, data_length);
    analyze_8(OBMPIMAGENAME, CSVNAME);
    return 1;
};

int main(int argc, char **argv) {
    if (argc == 4) {
        int result = user_input(argv[1], atoi(argv[2]), atoi(argv[3]));
        if ( result == -1){
            printf("Error! Something was wrong ...\n");
            return 0;
        }
    }
    else printf("Error! Only 3 parameters are requirted!\n");
    return 0;
}

