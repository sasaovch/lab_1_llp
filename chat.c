#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    uint32_t number;
    uint32_t string_length;
    char *string;
} Data;

void write_to_binary_file(const char *filename, Data *data, int count) {
    FILE *file = fopen(filename, "ab");
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fwrite(&data[i].number, sizeof(uint32_t), 1, file);
        fwrite(&data[i].string_length, sizeof(uint32_t), 1, file);
        fwrite(data[i].string, sizeof(char), data[i].string_length, file);
    }

    fclose(file);
}

int read_from_binary_file(const char *filename, Data **data) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file\n");
        return 0;
    }

    int count = 0;
    while (1) {
        uint32_t number;
        uint32_t string_length;
        if (fread(&number, sizeof(uint32_t), 1, file) != 1) {
            break;
        }
        fread(&string_length, sizeof(uint32_t), 1, file);

        char *string = (char *)malloc(sizeof(char) * (string_length + 1));
        fread(string, sizeof(char), string_length, file);
        string[string_length] = '\0';

        *data = (Data *)realloc(*data, sizeof(Data) * (count + 1));
        (*data)[count].number = number;
        (*data)[count].string_length = string_length;
        (*data)[count].string = string;
        count++;
    }

    fclose(file);
    return count;
}

int main() {
    const char *filename = "data.bin";

    Data data_to_write[] = {
        {25, 4, "John"},
        {42, 5, "Alice"},
        {33, 3, "Bob"},
    };
    int count_to_write = sizeof(data_to_write) / sizeof(Data);

    printf("Writing data to binary file...\n");
    write_to_binary_file(filename, data_to_write, count_to_write);

    printf("Reading data from binary file...\n");
    Data *read_data = NULL;
    int count_read = read_from_binary_file(filename, &read_data);

    for (int i = 0; i < count_read; i++) {
        printf("Number: %u, String: %s\n", read_data[i].number, read_data[i].string);
        free(read_data[i].string);
    }
    free(read_data);

    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    uint32_t number;
    uint32_t string_length;
    char *string;
    float real_number;
    uint8_t boolean;
    
} Data;

void write_to_binary_file(const char *filename, Data *data, int count) {
    FILE *file = fopen(filename, "ab");
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fwrite(&data[i].number, sizeof(uint32_t), 1, file);
        fwrite(&data[i].string_length, sizeof(uint32_t), 1, file);
        fwrite(data[i].string, sizeof(char), data[i].string_length, file);
        fwrite(&data[i].real_number, sizeof(float), 1, file);
        fwrite(&data[i].boolean, sizeof(uint8_t), 1, file);
    }

    fclose(file);
}

int read_from_binary_file(const char *filename, Data **data) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file\n");
        return 0;
    }

    int count = 0;
    while (1) {
        uint32_t number;
        uint32_t string_length;
        float real_number;
        uint8_t boolean;
        
        if (fread(&number, sizeof(uint32_t), 1, file) != 1) {
            break;
        }
        fread(&string_length, sizeof(uint32_t), 1, file);

        char *string = (char *)malloc(sizeof(char) * (string_length + 1));
        fread(string, sizeof(char), string_length, file);
        string[string_length] = '\0';

        fread(&real_number, sizeof(float), 1, file);
        fread(&boolean, sizeof(uint8_t), 1, file);
        
        *data = (Data *)realloc(*data, sizeof(Data) * (count + 1));
        (*data)[count].number = number;
        (*data)[count].string_length = string_length;
        (*data)[count].string = string;
        (*data)[count].real_number = real_number;
        (*data)[count].boolean = boolean;
        
        count++;
    }

    fclose(file);
    return count;
}

int main() {
    const char *filename = "data.bin";

    Data data_to_write[] = {
        {25, 4, "John", 3.1415, 1},
        {42, 5, "Alice", 2.7182, 0},
        {33, 3, "Bob", 1.6180, 1},
    };1
    int count_to_write = sizeof(data_to_write) / sizeof(Data);

    printf("Writing data to binary file...\n");
    write_to_binary_file(filename, data_to_write, count_to_write);

    printf("Reading data from binary file...\n");
    Data *read_data = NULL;
    int count_read = read_from_binary_file(filename, &read_data);

    for (int i = 0; i < count_read; i++) {
        printf("Number: %u, String: %s, Real Number: %f, Boolean: %u\n", read_data[i].number, read_data[i].string, read_data[i].real_number, read_data[i].boolean);
        free(read_data[i].string);
    }
    free(read_data);

    return 0;
}

