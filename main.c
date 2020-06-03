#define IBMPIMAGENAME "linux.bmp"
#define OBMPIMAGENAME "linux_sp.bmp"
#define OOBMPIMAGENAME "linux_sp_decrypt.bmp"
#include <stdio.h>
#include <time.h>
#include "SPN.h"

int main(int argc, char **argv)
{
    srand(1);
    //key test
    TKEY key;
    init_key(&key, 3);
    print_key(&key);
    //end key test
    // sub test
//    TSBOX sbox;
//    init_sbox(&sbox, 32);
//    print_sbox(&sbox);
//    unsigned char buf = 30;
//    printf("\nvalue of buf is %c   %d\n", buf, buf);
//    unsigned char sub = get_substitution(&sbox, buf);
//    printf("\nvalue of sub is %c   %d\n", sub, sub);
//    unsigned char reverse_sub = get_reverse_substitution(&sbox, sub);
//    printf("\nvalue of rsub is %c   %d\n", reverse_sub, reverse_sub);
//    bit_check(&buf);
//    bit_check(&sub);
//    bit_check(&reverse_sub);
    //end sub test
//permutation test
//    TPBOX pbox;
//    init_pbox(&pbox, 24);
//    print_pbox(&pbox);
//    unsigned char source_data [3] = {'P','B', 'A'};
//    for (int i = 0; i < 3; i++){
//        bit_check(&source_data[i]);
//        for (int k =0 ; k < 8; k++){
//            printf("bit %d is %d\n", k,get_bit(source_data[i],k));
//        }
//    }
//    unsigned char *perm_data;
//    perm_data = get_permutation(&pbox, &source_data[0], 3);
//    for (int i = 0; i < 3; i++){
//        bit_check(&perm_data[i]);
//        for (int k =0 ; k < 8; k++){
//            printf("bit %d is %d\n", k,get_bit(perm_data[i],k));
//        }
//    }
//    unsigned char *reverse_perm_data;
//    reverse_perm_data = get_reverse_permutation(&pbox, &perm_data[0], 3);
//    for (int i = 0; i < 3; i++){
//        bit_check(&reverse_perm_data[i]);
//        for (int k =0 ; k < 8; k++){
//            printf("bit %d is %d\n", k,get_bit(reverse_perm_data[i],k));
//        }
//    }
//end permutation test
// box test
    TBOXES boxes;
    init_boxes(&boxes);
    for (int i = 0; i < 3; i++){
        b_add_sbox(&boxes, 256);
    }
    printf("1\n");
    b_add_pbox(&boxes, 24);
    print_boxes(&boxes);
    encrypt(IBMPIMAGENAME, OBMPIMAGENAME, &boxes, &key, 1);
    printf("2\n");
    decrypt(OBMPIMAGENAME, OOBMPIMAGENAME, &boxes, &key, 1);
//    print_sbox(boxes.sboxes[0]);
//    print_sbox(boxes.sboxes[1]);
//    print_pbox(boxes.pboxes[0]);
// end box test
//    FILE *input_image, *output_image;
//    TBITMAPFILEHEADER image_file_header;
//    TBITMAPINFOHEADER image_info_header;
//    unsigned char *image_data;
//    if ((input_image = fopen(BMPIMAGENAME, "rb")) != NULL) {
//        fread(&image_file_header, sizeof(TBITMAPFILEHEADER), 1, input_image);
//        fread(&image_info_header, sizeof(TBITMAPINFOHEADER), 1, input_image);
//        fclose(input_image);
//        image_data = load_image_data(BMPIMAGENAME);
//        int padding = (image_info_header.biWidth) % 4;
//        for (int y = 0; y < image_info_header.biHeight; y++) {
//            for (int x = 0; x < image_info_header.biWidth; x++) {
//                TRGBTRIPLE *pixel;
//                pixel = get_pixel(&image_info_header, &image_data[0], x, y);
//                printf("pixel %d %d \n", y, x);
//                if (pixel != NULL) {
//                    printf("R is %d G is %d B is %d \n", pixel->rgbRed, pixel->rgbGreen, pixel->rgbBlue);
//                }
//                else {
//                    printf("Something was wrong...\n");
//                    return NULL;
//                }
//            }
//            for (int i = 0; i < padding; i++) {
//                printf("Write padding\n");
//            }
//        }
//        printf("File encrypted! Check the working directory!\n");
//    }
//    else {
//        printf("Can not open image file!\n");
//        return NULL;
//    }
    return 0;
}

