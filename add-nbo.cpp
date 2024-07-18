#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>    
#include <iostream>
#include <memory>
#include <netinet/in.h>
#include <arpa/inet.h>

#define READ_SIZE 4

using namespace std;

struct FileDeleter {
    void operator()(FILE* file) const {
        if (file) fclose(file);
    }
};

using FilePtr = unique_ptr<FILE, FileDeleter>;

uint32_t GetFileSize(FILE* file){
    fseek(file, 0, SEEK_END);
    uint32_t size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}

FilePtr fopen_(const char* filename, const char* mode){
    FILE* file = fopen(filename, mode);
    if(!file){
        fprintf(stderr, "Can not open File: %s\n", filename);
        exit(-1);
    }
 
    uint32_t filesize = GetFileSize(file);
    if(filesize != READ_SIZE){
        fprintf(stderr, "File size is not %d byte: %s(FileSize: %d)\n", READ_SIZE, filename, filesize);
        exit(-1);
    }

    return FilePtr(file) ;
}

uint32_t getFileData(FILE* file, size_t size) {
    uint32_t num;
    if(fread(&num, sizeof(int8_t), READ_SIZE, file) < READ_SIZE) {
        fprintf(stderr, "Error reading file\n");
        exit(-1);
    }

    num = ntohl(num);
    return num;
}

int main(int argc, char **argv){
    if (argc !=  3){
        printf("Usage: add-nbo <file1> <file2>\n");
        exit(-1);
    }
    
    FilePtr file1 = fopen_(argv[1], "rb");
    FilePtr file2 = fopen_(argv[2], "rb");
    
    uint32_t num1 = getFileData(file1.get(), READ_SIZE);
    uint32_t num2 = getFileData(file2.get(), READ_SIZE);

    printf("%d(0x%x) + %d(0x%x) = %d(0x%x)\n", num1, num1, num2, num2, num1 + num2, num1 + num2);
    
    return 0;
}   