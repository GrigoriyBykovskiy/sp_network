#define SETCOLORblack "0"
#include <stdio.h>
#include "SPN.h"

void init_pbox(TPBOX* pbox, unsigned int data_count){
    unsigned char *new_data;
    new_data = (unsigned char *)malloc(data_count * sizeof(unsigned char));
    if (new_data != NULL){
        // fill an array
        for (int i = 0; i < data_count; i++){
            new_data[i] = i;
        }
        // mix an array
        for (int i = data_count - 1; i >= 1; i--){
            //for (int i = 0; i < data_count; i++){
            //int r = rand() % data_count;
            int r = rand() % (i + 1);
            unsigned int buf = new_data[i];
            new_data[i] = new_data[r];
            new_data[r] = buf;
        }
        pbox->data = new_data;
        pbox->data_count = data_count;
    }
    else{
        printf("Error! Not enough memory\n");
        free(new_data);
    }
};

void init_sbox(TSBOX* sbox, unsigned int data_count){
    unsigned char *data_new;
    data_new = (unsigned char *)malloc(data_count * sizeof(unsigned char));
    if (data_new != NULL){
        // fill an array
        for (int i = 0; i < data_count; i ++){
            data_new[i] = i;
        }
        // mix an array
        for (int i = data_count - 1; i >= 1; i--){
            //for (int i = 0; i < data_count; i++){
            //int r = rand() % data_count;
            int r = rand() % (i + 1);
            unsigned char buf = data_new[i];
            data_new[i] = data_new[r];
            data_new[r] = buf;
        }
        sbox->data = data_new;
        sbox->data_count = data_count;
    }
    else{
        printf("Error! Not enough memory\n");
        free(data_new);
    }
};
void print_sbox(TSBOX* sbox){
    printf("Table of substitution:\n");
    unsigned int count = 0;
    printf("byte value in the source line");
    for (int i = 1; i <= sbox->data_count; i++){
        printf(" %3d ", i - 1);
        if (i % 8 == 0) {
            printf("\nbyte value in in the new line");
            for (int k = count; k < i; k++){
                printf(" %3d ", sbox->data[k]);
            }
            if (i+1 < sbox->data_count) {
                printf("\nbyte value in the source line");
                count = i;
            }
        }
    }
    printf("\n");
};
void print_pbox(TPBOX* pbox){
    printf("Table of permutation:\n");
    printf("bit position in a new line\n");
    for (int i = 0; i < pbox->data_count; i++){
        printf(" %2d ", i);
    }
    printf("\n");
    printf("bit number in the source line\n");
    for (int i = 0; i < pbox->data_count; i++){
        printf(" %2d ", pbox->data[i]);
    }
    printf("\n");
};
void bit_check(char *symbol) {/*little-endian format*/
    int array_of_bits[8];
    printf("%c : ", *symbol);
    for (int i = 0; i < 8; i++) {
        if ((*symbol & (1 << i)) == 0) {
            array_of_bits[i] = 0;
        }
        else {
            array_of_bits[i] = 1;
        }
        printf("%d", array_of_bits[i]);
    }
    printf("\n");
};
unsigned char *load_image_data(char *image_file_name) {
    FILE *input_image;
    TBITMAPFILEHEADER image_file_header;
    TBITMAPINFOHEADER image_info_header;
    unsigned char *image_data;
    if ((input_image = fopen(image_file_name, "rb")) != NULL) {
        fread(&image_file_header, sizeof(TBITMAPFILEHEADER), 1, input_image);
        fread(&image_info_header, sizeof(TBITMAPINFOHEADER), 1, input_image);
        //Is it BMP?
        if ((image_file_header.bfType != 0x4D42) && (image_info_header.biBitCount != 24)) {
            printf("Unsupported format of file!\n");
            fclose(input_image);
            return NULL;
        }
        //Size of data is empty?
        if (image_info_header.biSizeImage == 0) {
            image_info_header.biSizeImage = image_info_header.biWidth * sizeof(TRGBTRIPLE) + image_info_header.biWidth % 4;
            image_info_header.biSizeImage *= abs(image_info_header.biHeight);
        }
        //move file point to the begging of bitmap data
        fseek(input_image, image_file_header.bfOffBits, SEEK_SET);
        image_data = (unsigned char*)malloc(image_info_header.biSizeImage);
        if (!image_data) {
            printf("Error! Not enough memory\n");
            free(image_data);
            fclose(input_image);
            return NULL;
        }
        //read in the bitmap image data
        fread(image_data, 1, (image_info_header.biSizeImage), input_image);
        if (image_data == NULL) {
            printf("Can not read image data!\n");
            fclose(input_image);
            return NULL;
        }
        fclose(input_image);
        return image_data;
    }
    else {
        printf("Can not open image!\n");
        return NULL;
    }
};
TRGBTRIPLE *get_pixel(TBITMAPINFOHEADER *image_info_header, unsigned char *image_data, int x, int y) {
    TRGBTRIPLE *pixel;
    int padding = image_info_header->biWidth % 4;
    int offset = sizeof(RGBTRIPLE)*x + (image_info_header->biWidth) * sizeof(TRGBTRIPLE)*y + y*padding;
    /*printf("%d",offset);
    printf("%c%c%c", image_data[offset], image_data[offset+1], image_data[offset+2]);*/
    if ((pixel = (TRGBTRIPLE *)malloc(sizeof(TRGBTRIPLE))) != NULL) {
        pixel->rgbBlue = image_data[offset];
        pixel->rgbGreen = image_data[offset + 1];
        pixel->rgbRed = image_data[offset + 2];
        return pixel;
    }
    else {
        printf("Error! Not enough memory\n");
        free(pixel);
        return NULL;
    }
};
void *init_key(TKEY* key, unsigned int length){ // in bytes
    unsigned char *data = (unsigned char *) malloc(length * sizeof(unsigned char));
    if (data != NULL){
        for (int i = 0; i < length; i++){
            unsigned char tmp = rand() % 255;
            data[i] = tmp;
        }
        key->data = data;
        key->data_count = length;
    }
    else {
        printf("Error! Not enough memory\n");
        free(data);
        return NULL;
    }
};
void init_boxes(TBOXES* box){
    box->sboxes = NULL;
    box->pboxes = NULL;
    box->pboxes_count = 0;
    box->sboxes_count = 0;
};
void b_add_sbox(TBOXES* box, unsigned int data){
    TSBOX *new_box;
    TSBOX **new_data;
    new_box = (TSBOX *)malloc(sizeof(TPBOX));
    new_data = (TSBOX **)realloc(box->sboxes, (box->sboxes_count + 1) * sizeof(unsigned));
    if (new_data != NULL && new_box != NULL){
        init_sbox(new_box, data);
        box->sboxes = new_data;
        box->sboxes[box->sboxes_count] = new_box;
        box->sboxes_count += 1;
    }
    else{
        printf("Error! Not enough memory\n");
        free(new_data);
    }
};
void b_add_pbox(TBOXES* box, unsigned int data){
    TPBOX *new_box;
    TPBOX **new_data;
    new_box = (TPBOX *)malloc(sizeof(TPBOX));
    new_data = (TPBOX **)realloc(box->pboxes, (box->pboxes_count + 1) * sizeof(unsigned));
    if (new_data != NULL && new_box != NULL){
        init_pbox(new_box, data);
        box->pboxes = new_data;
        box->pboxes[box->pboxes_count] = new_box;
        box->pboxes_count += 1;
    }
    else{
        printf("Error! Not enough memory\n");
        free(new_data);
    }
};
void print_boxes(TBOXES* box){
    printf("\n----BOXES----\n");
    for (int i = 0; i < box->sboxes_count; i++){
        print_sbox(box->sboxes[i]);
    }
    for (int i = 0; i < box->pboxes_count; i++){
        print_pbox(box->pboxes[i]);
    }
};
void print_key(TKEY* key){
    printf("Key is ");
    for (int i = 0; i < key->data_count; i++){
        printf("%c", key->data[i]);
    }
    printf("\n");
    printf("Key value is ");
    for (int i = 0; i < key->data_count; i++){
        printf(" %d ", key->data[i]);
    }
    printf("\n");
};
unsigned char get_substitution(TSBOX* sbox, unsigned char element){
    return sbox->data[element];
};
unsigned char get_reverse_substitution(TSBOX* sbox, unsigned char element){
    for (int i = 0; i < sbox->data_count; i++){
        if (sbox->data[i] == element){
            return i;
            break;
        }
    }
};
unsigned char get_bit(unsigned char element, unsigned char position){//return value of bit
    return ((1<<position) & element)>>position;
};
//unsigned char *get_permutation(TPBOX* pbox, unsigned char *data, unsigned int length){ // in bytes
//    unsigned char *new_data = (unsigned char*)malloc(length*sizeof(unsigned char));
//    if (new_data != NULL) {
//        // fill the array of new data zero values
//        for (int i = 0; i < length; i++){
//            new_data[i] = 0;
//        }
//        for (int i = 0; i <= pbox->data_count; i++) {
//            unsigned char buf = pbox->data[i];
//            if (buf >= 0 && buf <= 7) {
//                unsigned char tmp = get_bit(data[0], buf);
//                if (tmp == 1) new_data[0] |= (1 << i);
//                if (tmp == 0) new_data[0] &= ~(1 << i);
//            }
//            if (buf >= 8 && buf <= 15) {
//                unsigned char tmp = get_bit(data[1], buf);
//                if (tmp == 1) new_data[1] |= (1 << i);
//                if (tmp == 0) new_data[1] &= ~(1 << i);
//            }
//            if (buf >= 16 && buf <= 23) {
//                unsigned char tmp = get_bit(&data[2], (buf % 8));
//                if (tmp == 1) new_data[2] |= (1 << (i % 8));
//                if (tmp == 0) new_data[2] &= ~(1 << (i % 8));
//            }
//        }
//        return new_data;
//    }
//    else{
//        printf("Error! Not enough memory\n");
//        free(new_data);
//        return NULL;
//    }
//};
unsigned char *get_permutation(TPBOX* pbox, unsigned char *data, unsigned int length){ // in bytes
    unsigned char *new_data = (unsigned char*)malloc(length*sizeof(unsigned char));
    if (new_data != NULL) {
        // fill the array of new data zero values
        for (int i = 0; i < length; i++){
            new_data[i] = 0;
        }
        for (int i = 0; i <= pbox->data_count; i++) {
            unsigned char buf = pbox->data[i];
            unsigned char tmp = 0;
            if (i >= 0 && i <= 7) {// filter for new line
                if (buf >= 0 && buf <= 7) tmp = get_bit(data[0], buf); // filter for source line
                if (buf >= 8 && buf <= 15) tmp = get_bit(data[1], buf%8);
                if (buf >= 16 && buf <= 23) tmp = get_bit(data[2], buf%8);
                if (tmp == 1) new_data[0] |= (1 << i);
                if (tmp == 0) new_data[0] &= ~(1 << i);
            }
            if (i >= 8 && i <= 15) {
                if (buf >= 0 && buf <= 7) tmp = get_bit(data[0], buf);
                if (buf >= 8 && buf <= 15) tmp = get_bit(data[1], buf%8);
                if (buf >= 16 && buf <= 23) tmp = get_bit(data[2], buf%8);
                if (tmp == 1) new_data[1] |= (1 << (i%8));
                if (tmp == 0) new_data[1] &= ~(1 << (i%8));
            }
            if (i >= 16 && i <= 23) {
                if (buf >= 0 && buf <= 7) tmp = get_bit(data[0], buf);
                if (buf >= 8 && buf <= 15) tmp = get_bit(data[1], buf%8);
                if (buf >= 16 && buf <= 23) tmp = get_bit(data[2], buf%8);
                if (tmp == 1) new_data[2] |= (1 << (i % 8));
                if (tmp == 0) new_data[2] &= ~(1 << (i % 8));
            }
        }
        return new_data;
    }
    else{
        printf("Error! Not enough memory\n");
        free(new_data);
        return NULL;
    }
};
unsigned char* get_reverse_permutation(TPBOX* pbox, unsigned char *data, unsigned int length){ // in bytes
    TPBOX reverse_pbox;
    init_pbox(&reverse_pbox, pbox->data_count);
    for (int i = 0; i < reverse_pbox.data_count; i++){
        reverse_pbox.data[pbox->data[i]] = i;
    }
    //print_pbox(&reverse_pbox);
    return get_permutation(&reverse_pbox, data, length);
};
int encrypt(char *input_image_filename, char *output_image_filename, TBOXES* boxes, TKEY* key, unsigned int round_count){
    FILE *input_image, *output_image;
    TBITMAPFILEHEADER image_file_header;
    TBITMAPINFOHEADER image_info_header;
    unsigned char *image_data;
    output_image = fopen(output_image_filename, "wb+");
    if ((input_image = fopen(input_image_filename, "rb")) != NULL) {
        fread(&image_file_header, sizeof(TBITMAPFILEHEADER), 1, input_image);
        fwrite(&image_file_header, 1, sizeof(TBITMAPFILEHEADER), output_image);
        fread(&image_info_header, sizeof(TBITMAPINFOHEADER), 1, input_image);
        fwrite(&image_info_header, 1, sizeof(TBITMAPINFOHEADER), output_image);
        fclose(input_image);
        image_data = load_image_data(input_image_filename);
        int padding = (image_info_header.biWidth) % 4;
        for (int y = 0; y < image_info_header.biHeight; y++) {
            for (int x = 0; x < image_info_header.biWidth; x++) {
                printf("Pixel is %d %d\n", y, x);
                TRGBTRIPLE *pixel;
                pixel = get_pixel(&image_info_header, &image_data[0], x, y);
                printf("Value of pixel is %3d %3d %3d \n", pixel->rgbRed, pixel->rgbGreen, pixel->rgbBlue);
                if (pixel != NULL) {
                    for (int i = 0; i < round_count; i++){
                        unsigned char source_buf [3] = {pixel->rgbRed, pixel->rgbGreen, pixel->rgbBlue};
                        unsigned  char xor_buf [3] = {0, 0, 0};
                        unsigned char sub_buf [3] = {0, 0, 0};
                        unsigned char *perm_buf;
                        printf("Source buf is %3d %3d %3d \n", source_buf [0], source_buf [1], source_buf [2]);
                        for (int k = 0; k < 3; k++){
                            xor_buf[k] = (source_buf[k])^(key->data[k]);
                            printf("%d byte of xor is %3d\n", k, xor_buf[k]);
                            sub_buf[k] = get_substitution(boxes->sboxes[k], xor_buf[k]);
                            printf("%d byte of sub_buf is %3d\n", k, sub_buf[k]);
                        }
                        perm_buf = get_permutation(boxes->pboxes[i], &sub_buf[0], 3);
                        printf("Perm buf is %3d %3d %3d \n", perm_buf[0], perm_buf[1], perm_buf[2]);
                        pixel->rgbRed = perm_buf[0];
                        pixel->rgbGreen = perm_buf[1];
                        pixel->rgbBlue = perm_buf[2];
                    }
                    printf("Final Value of pixel is %3d %3d %3d \n", pixel->rgbRed, pixel->rgbGreen, pixel->rgbBlue);
                    fwrite(pixel, sizeof(BYTE), sizeof(TRGBTRIPLE), output_image);
                }
                else {
                    printf("Something was wrong...\n");
                    return -1;
                }
            }
            for (int i = 0; i < padding; i++) {
                fwrite(SETCOLORblack, 1, 1, output_image);
            }
        }
        fclose(output_image);
        printf("File encrypted! Check the working directory!\n");
    }
    else {
        printf("Can not open image file!\n");
        return -1;
    }
};
int decrypt(char *input_image_filename, char *output_image_filename, TBOXES* boxes, TKEY* key, unsigned int round_count){
    FILE *input_image, *output_image;
    TBITMAPFILEHEADER image_file_header;
    TBITMAPINFOHEADER image_info_header;
    unsigned char *image_data;
    output_image = fopen(output_image_filename, "wb+");
    if ((input_image = fopen(input_image_filename, "rb")) != NULL) {
        fread(&image_file_header, sizeof(TBITMAPFILEHEADER), 1, input_image);
        fwrite(&image_file_header, 1, sizeof(TBITMAPFILEHEADER), output_image);
        fread(&image_info_header, sizeof(TBITMAPINFOHEADER), 1, input_image);
        fwrite(&image_info_header, 1, sizeof(TBITMAPINFOHEADER), output_image);
        fclose(input_image);
        image_data = load_image_data(input_image_filename);
        int padding = (image_info_header.biWidth) % 4;
        for (int y = 0; y < image_info_header.biHeight; y++) {
            for (int x = 0; x < image_info_header.biWidth; x++) {
                printf("Pixel is %d %d\n", y, x);
                TRGBTRIPLE *pixel;
                pixel = get_pixel(&image_info_header, &image_data[0], x, y);
                printf("Value of pixel is %3d %3d %3d \n", pixel->rgbRed, pixel->rgbGreen, pixel->rgbBlue);
                if (pixel != NULL) {
                    for (int i = 0; i < round_count; i++){
                        unsigned char source_buf [3] = {pixel->rgbRed, pixel->rgbGreen, pixel->rgbBlue};
                        unsigned  char xor_buf [3] = {0, 0, 0};
                        unsigned char sub_buf [3] = {0, 0, 0};
                        unsigned char *perm_buf;
                        printf("Source buf is %3d %3d %3d \n", source_buf [0], source_buf [1], source_buf [2]);
                        perm_buf = get_reverse_permutation(boxes->pboxes[i], &source_buf[0], 3);
                        printf("Perm buf is %3d %3d %3d \n", perm_buf[0], perm_buf[1], perm_buf[2]);
                        for (int k = 0; k < 3; k++){
                            sub_buf[k] = get_reverse_substitution(boxes->sboxes[k], perm_buf[k]);
                            printf("%d byte of sub_buf is %3d\n", k, sub_buf[k]);
                            xor_buf[k] = (sub_buf[k])^(key->data[k]);
                            printf("%d byte of xor is %3d\n", k, xor_buf[k]);
                        }
                        pixel->rgbRed = xor_buf[0];
                        pixel->rgbGreen = xor_buf[1];
                        pixel->rgbBlue = xor_buf[2];
                    }
                    printf("Final Value of pixel is %3d %3d %3d \n", pixel->rgbRed, pixel->rgbGreen, pixel->rgbBlue);
                    fwrite(pixel, sizeof(BYTE), sizeof(TRGBTRIPLE), output_image);
                }
                else {
                    printf("Something was wrong...\n");
                    return -1;
                }
            }
            for (int i = 0; i < padding; i++) {
                fwrite(SETCOLORblack, 1, 1, output_image);
            }
        }
        printf("File encrypted! Check the working directory!\n");
    }
    else {
        printf("Can not open image file!\n");
        return -1;
    }
};