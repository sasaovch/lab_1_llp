#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char name[20];
} Record;

void delete_element(const char *filename, int index_to_remove) {
    FILE *input = fopen(filename, "rb");
    if (input == NULL) {
        perror("Error opening file for reading");
        return;
    }

    // Calculate the number of records in the file
    fseek(input, 0, SEEK_END);
    int num_records = ftell(input) / sizeof(Record);
    fseek(input, 0, SEEK_SET);

    if (index_to_remove < 0 || index_to_remove >= num_records) {
        fprintf(stderr, "Invalid index to remove\n");
        fclose(input);
        return;
    }

    // Read the records and remove the desired element
    Record *records = (Record *)malloc(sizeof(Record) * (num_records - 1));
    int j = 0;
    for (int i = 0; i < num_records; i++) {
        Record temp;
        fread(&temp, sizeof(Record), 1, input);
        if (i != index_to_remove) {
            records[j++] = temp;
        }
    }
    fclose(input);

    // Rewrite the updated records to the file
    FILE *output = fopen(filename, "wb");
    if (output == NULL) {
        perror("Error opening file for writing");
        free(records);
        return;
    }
    fwrite(records, sizeof(Record), num_records - 1, output);
    fclose(output);

    free(records);

    if (ftruncate(fileno(file), (num_records - 1) * sizeof(Record)) != 0) {
        perror("Error truncating file");
    }

}

int main() {
    const char *filename = "binary_data.bin";

    // Perform element deletion at desired index
    int index_to_remove = 2;
    delete_element(filename, index_to_remove);

    return 0;
}
