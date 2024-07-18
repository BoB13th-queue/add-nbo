#include <stddef.h> // for size_t
#include <stdint.h> // for uint8
#include <stdlib.h> // for printf    
#include <iostream>

#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

// 파일에서 숫자을 읽기 위해서는 fopen, fread 함수 등을 사용한다. fopen하고 나서 fclose 함수도 호출해 주도록 한다. 관련 함수들의 사용 방법은 검색을 통하여 익힌다.
// 정수 덧셈에서 발생하는 overflow는 무시한다.
// 파일이 제대로 열리지 않는 경우, 파일의 크기가 4바이트보다 많거나 작아서 제대로 읽지 못했다고 판단되는 경우 등에 대해서 에러 처리를 할 수 있도록 한다.

uint32_t GetFileSize(FILE* file){
    fseek(file, 0, SEEK_END);
    uint32_t size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}

FILE* fopen_(const char* filename, const char* mode){
    FILE* file = fopen(filename, mode);
    if(file == NULL){
        printf("Can not open File: %s\n", filename);
        exit(-1);
    }
    uint32_t filesize = GetFileSize(file);
    if(filesize != 4){
        printf("File size is not 4 byte: %s(FileSize: %d)\n", filename, filesize);
        exit(-1);
    }

    return file;
}

uint32_t getFileData(FILE* file, size_t size) {
    uint32_t num;
    fread(&num, sizeof(uint32_t), 1, file);
    num = ntohl(num);
    return num;
}

int main(int argc, char **argv){
    if (argc !=  3){
        printf("Usage: add-nbo <file1> <file2>\n");
        return -1;
    }
    
    FILE* file1 = fopen_(argv[1], "rb");
    FILE* file2 = fopen_(argv[2], "rb");
    
    uint32_t num1= getFileData(file1, 4);
    uint32_t num2= getFileData(file2, 4);

    printf("%d(0x%x) + %d(0x%x) = %d(0x%x)\n", num1, num1, num2, num2, num1 + num2, num1 + num2);
    
    fclose(file1);
    fclose(file2);
    return 0;

}