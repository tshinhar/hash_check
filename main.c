#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

int main(int argc, char *argv[]){
    if(argc != 3){
        fprintf(stderr, "Usage %s <filename> <hash>\n", argv[0]);
        return 1;
    }

    printf("opening file\n");
    FILE *fp = fopen(argv[1], "rb");
    if(fp == NULL){
        perror("failed to open file");
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    long file_size  = ftell(fp);
    rewind(fp);

    unsigned char *file_data = malloc(file_size);
    if(file_data == NULL){
        perror("malloc filed");
        fclose(fp);
        return 1;
    }

    printf("reading file\n");
    size_t bytes_read = fread(file_data, 1, file_size, fp);
    if(bytes_read != file_size){
        fprintf(stderr, "failed to read full file");
        fclose(fp);
        free(file_data);
        return 1;
    }

    printf("calculating hash\n");
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(file_data, file_size, hash);
    char hex_hash[SHA256_DIGEST_LENGTH * 2 + 1];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(&hex_hash[i*2], "%02x", hash[i]);
    }
    hex_hash[SHA256_DIGEST_LENGTH*2] = '\0';

    printf("calculated hash is:\n %s\n", hex_hash);
    printf("given hash is:\n %s\n", argv[2]);

    if(strcmp(hex_hash, argv[2]) == 0){
        printf("Hash is matching!!!\n");
    }
    else{
        printf("Hash is not matching\n");
    }

    free(file_data);
    fclose(fp);
    return 0;
    
}