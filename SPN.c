#define BYTE_MAX_VALUE 256
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
    printf("------------------------Table of substitution:------------------------\n");
    unsigned int count = 0;
    printf("|byte value in the source line");
    for (int i = 1; i <= sbox->data_count; i++){
        printf("|%3d|", i - 1);
        if (i % 8 == 0) {
            printf("\n|byte value in in the new line");
            for (int k = count; k < i; k++){
                printf("|%3d|", sbox->data[k]);
            }
            if (i+1 < sbox->data_count) {
                printf("\n|byte value in the source line");
                count = i;
            }
        }
    }
    printf("\n----------------------end table of substitution:----------------------\n");
};
void print_pbox(TPBOX* pbox){
    printf("\n--------------------Table of permutation:---------------------\n");
    printf("|bit position in the new line ");
    int count = 0;
    for (int i = 1; i <=pbox->data_count; i++){
        printf("|%2d|", i - 1);
        if (i % 8 == 0) {
            printf("\n|bit number in the source line");
            for (int k = count; k < i; k++){
                printf("|%2d|", pbox->data[k]);
            }
            if (i+1 < pbox->data_count) {
                printf("\n|bit position in the new line ");
                count = i;
            }
        }
    }
    printf("\n------------------end table of permutation:-------------------\n");
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
        if (image_file_header.bfType != 0x4D42){
            printf("Unsupported format of file!\n");
            fclose(input_image);
            return NULL;
        }
        if ((image_info_header.biBitCount != 24) && (image_info_header.biBitCount != 1)){
            printf("Unsupported format of file!\n");
            fclose(input_image);
            return NULL;
        }
        //Size of data is empty?
        if (image_info_header.biSizeImage == 0) {
            if (image_info_header.biBitCount == 1) {
                image_info_header.biSizeImage =
                        image_info_header.biWidth * sizeof(unsigned char) + image_info_header.biWidth % 4;
                image_info_header.biSizeImage *= abs(image_info_header.biHeight);
                image_info_header.biSizeImage /= 8;
            }
            if (image_info_header.biBitCount == 24){
                image_info_header.biSizeImage =
                        image_info_header.biWidth * sizeof(TRGBTRIPLE) + image_info_header.biWidth % 4;
                image_info_header.biSizeImage *= abs(image_info_header.biHeight);
            }
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
    new_box = (TSBOX*)malloc(sizeof(TSBOX));
    new_data = (TSBOX **)realloc(box->sboxes, (box->sboxes_count + 1) * sizeof(TSBOX *));
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
    new_data = (TPBOX **)realloc(box->pboxes, (box->pboxes_count + 1) * sizeof(TPBOX*));
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
    printf("\n----Key----\n");
    printf("|Key data  ");
    for (int i = 0; i < key->data_count; i++){
        printf("|%3c|", key->data[i]);
    }
    printf("\n");
    printf("|Key value ");
    for (int i = 0; i < key->data_count; i++){
        printf("|%3d|", key->data[i]);
    }
    printf("\n--end key--\n");
};
unsigned char get_substitution(TSBOX* sbox, unsigned char element){
    return sbox->data[element];
};
unsigned char get_reverse_substitution(TSBOX* sbox, unsigned char element){
    for (int i = 0; i < sbox->data_count; i++){
        if (sbox->data[i] == element){
            return i;
        }
    }
};
unsigned char get_bit(unsigned char element, unsigned char position){//return value of bit
    return ((1<<position) & element)>>position;
};
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
                if (buf >= 24 && buf <= 31) tmp = get_bit(data[3], buf%8);
                if (tmp == 1) new_data[0] |= (1 << i);
                if (tmp == 0) new_data[0] &= ~(1 << i);
            }
            if (i >= 8 && i <= 15) {
                if (buf >= 0 && buf <= 7) tmp = get_bit(data[0], buf);
                if (buf >= 8 && buf <= 15) tmp = get_bit(data[1], buf%8);
                if (buf >= 16 && buf <= 23) tmp = get_bit(data[2], buf%8);
                if (buf >= 24 && buf <= 31) tmp = get_bit(data[3], buf%8);
                if (tmp == 1) new_data[1] |= (1 << (i%8));
                if (tmp == 0) new_data[1] &= ~(1 << (i%8));
            }
            if (i >= 16 && i <= 23) {
                if (buf >= 0 && buf <= 7) tmp = get_bit(data[0], buf);
                if (buf >= 8 && buf <= 15) tmp = get_bit(data[1], buf%8);
                if (buf >= 16 && buf <= 23) tmp = get_bit(data[2], buf%8);
                if (buf >= 24 && buf <= 31) tmp = get_bit(data[3], buf%8);
                if (tmp == 1) new_data[2] |= (1 << (i % 8));
                if (tmp == 0) new_data[2] &= ~(1 << (i % 8));
            }
            if (i >= 24 && i <= 31) {
                if (buf >= 0 && buf <= 7) tmp = get_bit(data[0], buf);
                if (buf >= 8 && buf <= 15) tmp = get_bit(data[1], buf%8);
                if (buf >= 16 && buf <= 23) tmp = get_bit(data[2], buf%8);
                if (buf >= 24 && buf <= 31) tmp = get_bit(data[3], buf%8);
                if (tmp == 1) new_data[3] |= (1 << (i % 8));
                if (tmp == 0) new_data[3] &= ~(1 << (i % 8));
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
    return get_permutation(&reverse_pbox, data, length);
};
int encrypt_8(char *input_image_filename, char *output_image_filename, TBOXES* boxes, TKEY* key, unsigned int round_count, unsigned int data_length){
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
        // Is palette exist?
        if (image_file_header.bfOffBits != sizeof(TBITMAPFILEHEADER) + sizeof(TBITMAPINFOHEADER)){
            unsigned int pallete_buf_count = image_file_header.bfOffBits - sizeof(TBITMAPFILEHEADER) - sizeof(TBITMAPINFOHEADER);
            unsigned char pallete_buf[pallete_buf_count];
            fread(&pallete_buf[0], sizeof(unsigned char), pallete_buf_count, input_image);
            fwrite(pallete_buf, sizeof(unsigned char), pallete_buf_count, output_image);
        }
        fclose(input_image);
        image_data = load_image_data(input_image_filename);
        int padding = (image_info_header.biWidth) % 4;
        if (padding != 0){
            printf("Unsupported format of file. Will be added in the new version!\n");
            return -1;
        }
        if (image_info_header.biSizeImage == 0) {
            if (image_info_header.biBitCount == 1) {
                image_info_header.biSizeImage =
                        image_info_header.biWidth * sizeof(unsigned char) + image_info_header.biWidth % 4;
                image_info_header.biSizeImage *= abs(image_info_header.biHeight);
                image_info_header.biSizeImage /= 8;
            }
        }
        if (image_info_header.biSizeImage % data_length != 0){
            printf("ANTA BAKA ?! Image size is not a multiple of the data block size\n");
            return -1;
        }
        unsigned int iterations = image_info_header.biSizeImage / data_length;
        for (int i = 0; i < iterations; i++){
            unsigned char source_data_buf[data_length];
            // fill the array
            for (int l = 0; l < data_length; l++){
                source_data_buf[l]= image_data[l+i*data_length];
            }
            for (int round = 0; round < round_count; round++){
                    unsigned char *perm_buf;
                    unsigned  char xor_buf[data_length];
                    unsigned char sub_buf[data_length];
                    for (int k = 0; k < data_length; k++){
                        int offset = k+round*data_length;
                        xor_buf[k] = (source_data_buf[k])^(key->data[offset]);
                        sub_buf[k] = get_substitution(boxes->sboxes[offset], xor_buf[k]);
                }
                perm_buf = get_permutation(boxes->pboxes[round], &sub_buf[0], data_length);
                // fill the array
                for (int count = 0; count < data_length; count++){
                    source_data_buf[count] = perm_buf[count];
                }
            }
            fwrite(&source_data_buf[0], sizeof(unsigned char), data_length, output_image);
        }
        fclose(output_image);
        printf("File encrypted! Check the working directory!\n");
    }
    else {
        printf("Can not open image file!\n");
        return -1;
    }
};
int decrypt_8(char *input_image_filename, char *output_image_filename, TBOXES* boxes, TKEY* key, unsigned int round_count, unsigned int data_length){
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
        // Is palette exist?
        if (image_file_header.bfOffBits != sizeof(TBITMAPFILEHEADER) + sizeof(TBITMAPINFOHEADER)){
            unsigned int pallete_buf_count = image_file_header.bfOffBits - sizeof(TBITMAPFILEHEADER) - sizeof(TBITMAPINFOHEADER);
            unsigned char pallete_buf[pallete_buf_count];
            fread(&pallete_buf[0], sizeof(unsigned char), pallete_buf_count, input_image);
            fwrite(pallete_buf, sizeof(unsigned char), pallete_buf_count, output_image);
        }
        fclose(input_image);
        image_data = load_image_data(input_image_filename);
        int padding = (image_info_header.biWidth) % 4;
        if (padding != 0){
            printf("Unsupported format of file. Will be added in the new version!\n");
            return -1;
        }
        if (image_info_header.biSizeImage == 0) {
            if (image_info_header.biBitCount == 1) {
                image_info_header.biSizeImage =
                        image_info_header.biWidth * sizeof(unsigned char) + image_info_header.biWidth % 4;
                image_info_header.biSizeImage *= abs(image_info_header.biHeight);
                image_info_header.biSizeImage /= 8;
            }
        }
        if (image_info_header.biSizeImage % data_length != 0){
            printf("ANTA BAKA ?! Image size is not a multiple of the data block size\n");
            return -1;
        }
        unsigned int iterations = image_info_header.biSizeImage / data_length;
        for (int i = 0; i < iterations; i++){
            unsigned char source_data_buf[data_length];
            // fill the array
            for (int l = 0; l < data_length; l++){
                source_data_buf[l]= image_data[l+i*data_length];
            }
            for (int round = round_count; round > 0; round--){
                unsigned char *perm_buf;
                unsigned  char xor_buf[data_length];
                unsigned char sub_buf[data_length];
                perm_buf = get_reverse_permutation(boxes->pboxes[round-1], &source_data_buf[0], data_length);
                for (int k = 0; k < data_length; k++){
                    int offset = k+(round-1)*data_length;
                    sub_buf[k] = get_reverse_substitution(boxes->sboxes[offset], perm_buf[k]);
                    xor_buf[k] = (sub_buf[k])^(key->data[offset]);
                }
                for (int count = 0; count < data_length; count++){
                    source_data_buf[count] = xor_buf[count];
                }
            }
            fwrite(&source_data_buf[0], sizeof(unsigned char), data_length, output_image);
        }
        fclose(output_image);
        printf("File decrypted! Check the working directory!\n");
    }
    else {
        printf("Can not open image file!\n");
        return -1;
    }
};
int analyze_8(char *input_image_filename, char *output_csv_filename){
    FILE *input_image, *output_csv;
    TBITMAPFILEHEADER image_file_header;
    TBITMAPINFOHEADER image_info_header;
    unsigned char *image_data;
    output_csv = fopen(output_csv_filename, "w");
    if ((input_image = fopen(input_image_filename, "rb")) != NULL) {
        fread(&image_file_header, sizeof(TBITMAPFILEHEADER), 1, input_image);
        fread(&image_info_header, sizeof(TBITMAPINFOHEADER), 1, input_image);
        // Is palette exist?
        if (image_file_header.bfOffBits != sizeof(TBITMAPFILEHEADER) + sizeof(TBITMAPINFOHEADER)){
            unsigned int pallete_buf_count = image_file_header.bfOffBits - sizeof(TBITMAPFILEHEADER) - sizeof(TBITMAPINFOHEADER);
            unsigned char pallete_buf[pallete_buf_count];
            fread(&pallete_buf[0], sizeof(unsigned char), pallete_buf_count, input_image);
        }
        fclose(input_image);
        image_data = load_image_data(input_image_filename);
        int padding = (image_info_header.biWidth) % 4;
        if (padding != 0){
            printf("Unsupported format of file. Will be added in the new version!\n");
            return -1;
        }
        if (image_info_header.biSizeImage == 0) {
            if (image_info_header.biBitCount == 1) {
                image_info_header.biSizeImage =
                        image_info_header.biWidth * sizeof(unsigned char) + image_info_header.biWidth % 4;
                image_info_header.biSizeImage *= abs(image_info_header.biHeight);
                image_info_header.biSizeImage /= 8;
            }
        }
        unsigned int buf[BYTE_MAX_VALUE];
        unsigned int output_data[BYTE_MAX_VALUE];
        // fill the arrays
        for (int i = 0; i < BYTE_MAX_VALUE; i++){
            buf[i] = i;
            output_data[i] = 0;
        }
        for (int i = 0; i < image_info_header.biSizeImage; i ++){
            for (int k = 0; k < BYTE_MAX_VALUE; k++){
                if (buf[k]==image_data[i]){
                    output_data[k] = output_data[k] + 1;
                }
            }
        }
        fprintf(output_csv,"frequency;value\n");
        for (int i = 0; i < BYTE_MAX_VALUE; i++){
            fprintf(output_csv,"%d",i);
            fprintf(output_csv,";");
            fprintf(output_csv,"%d",output_data[i]);
            fprintf(output_csv,"\n");
        }
        fclose(output_csv);
        printf("Analysis done! Check the working directory!\n");
    }
    else {
        printf("Can not open image file!\n");
        return -1;
    }
};