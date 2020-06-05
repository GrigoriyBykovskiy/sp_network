#include <windows.h>
#ifndef SP_NETWORK_SPN_H
#define SP_NETWORK_SPN_H
#pragma pack(push, 1)//padding (1 byte)
typedef struct {
    DWORD biSize;  //specifies the number of bytes required by the struct
    LONG biWidth;  //specifies width in pixels
    LONG biHeight;  //species height in pixels
    WORD biPlanes; //specifies the number of color planes, must be 1
    WORD biBitCount; //specifies the number of bit per pixel
    DWORD biCompression;//specifies the type of compression
    DWORD biSizeImage;  //size of image in bytes
    LONG biXPelsPerMeter;  //number of pixels per meter in x axis
    LONG biYPelsPerMeter;  //number of pixels per meter in y axis
    DWORD biClrUsed;  //number of colors used by th ebitmap
    DWORD biClrImportant;  //number of colors that are important
} TBITMAPINFOHEADER;

typedef struct {
    WORD bfType;  //specifies the file type.If the file is BMP then it must be 0x4D42 .
    DWORD bfSize;  //specifies the size in bytes of the bitmap file
    WORD bfReserved1;  //reserved; must be 0
    WORD bfReserved2;  //reserved; must be 0
    DWORD bfOffBits;  //species the offset in bytes from the bitmapfileheader to the bitmap bits
} TBITMAPFILEHEADER;
typedef struct  {
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
} TRGBTRIPLE;
#pragma pack(pop)

typedef struct {
    unsigned char *data;
    unsigned int data_count;
} TKEY;

typedef struct {
    unsigned char *data;
    unsigned int data_count;
} TPBOX;

typedef struct {
    unsigned char *data;
    unsigned int data_count;
} TSBOX;

typedef struct {
    TSBOX **sboxes;
    TPBOX **pboxes;
    unsigned int sboxes_count;
    unsigned int pboxes_count;
} TBOXES;

void init_pbox(TPBOX* pbox, unsigned int data_count);
void init_sbox(TSBOX* sbox, unsigned int data_count);
void print_sbox(TSBOX* sbox);
void print_pbox(TPBOX* pbox);
void init_boxes(TBOXES* box);
void b_add_sbox(TBOXES* box, unsigned int data);
void b_add_pbox(TBOXES* box, unsigned int data);
void print_boxes(TBOXES* box);
unsigned char *load_image_data(char *image_file_name);
void *init_key(TKEY* key, unsigned int length);
void print_key(TKEY* key);
unsigned char get_substitution(TSBOX* sbox, unsigned char element);
unsigned char get_reverse_substitution(TSBOX* sbox, unsigned char element);
unsigned char get_bit(unsigned char element, unsigned char position);
unsigned char *get_permutation(TPBOX* pbox, unsigned char *data, unsigned int length);
unsigned char* get_reverse_permutation(TPBOX* pbox, unsigned char *data, unsigned int length);
int encrypt_8(char *input_image_filename, char *output_image_filename, TBOXES* boxes, TKEY* key, unsigned int round_count, unsigned int data_length);
int decrypt_8(char *input_image_filename, char *output_image_filename, TBOXES* boxes, TKEY* key, unsigned int round_count, unsigned int data_length);
int analyze_8(char *input_image_filename, char *output_csv_filename);
#endif //SP_NETWORK_SPN_H
