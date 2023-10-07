#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

const uint32_t PAGE_SIZE = 4096;
const uint32_t NAME_TYPE_SIZE = 64;

typedef struct {
    int file_descriptor;
    uint32_t file_length;
} File;

typedef struct {
    char* buffer;
    size_t buffer_length;
    ssize_t input_length;
} InputBuffer;

typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
  RELATIONSHIP,
  NODE,
  PROPERTY
} TypeOfElement;

typedef struct {
    TypeOfElement type;
    char* name;
} MetaTable;

typedef struct {
    uint32_t next_block;
    TypeOfElement type;
    uint32_t number;
    float real_number;
    uint8_t boolean;
    uint32_t string_length;
    char* string;
} MetaPage;

File* db_open(const char* filename) {
    int fd = open(filename,
                O_RDWR |      // Read/Write mode
                    O_CREAT,  // Create file if it does not exist
                S_IWUSR |     // User write permission
                    S_IRUSR   // User read permission
    );

    if (fd == -1) {
        printf("Unabled to open file\n");
        exit(EXIT_FAILURE);
    }

    off_t file_length = lseek(fd, 0, SEEK_END); // find end of file

    if (file_length == -1) {
        printf("Failed to find end of file\n");
        exit(EXIT_FAILURE);
    }

    File* file = (File*) malloc(sizeof(File));
    file->file_descriptor = fd;
    file->file_length = file_length;

    return file;
}

InputBuffer* new_input_buffer() {
    InputBuffer* input_buffer = (InputBuffer*) malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;

    return input_buffer;
}

void print_prompt() { printf("db > "); }

void read_input(InputBuffer* input_buffer) {
    ssize_t bytes_read =
      getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

    if (bytes_read <= 0) {
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }

    // Ignore trailing newline
    input_buffer->input_length = bytes_read - 1;
    input_buffer->buffer[bytes_read - 1] = 0;
}

MetaTable* prepare_meta_command(InputBuffer* input_buffer) {
    printf("Preparing\n");
    if (strcmp(input_buffer->buffer, ".exit") == 0) {
        //FIXME: is bad to exit and prepare
        // db_close(file);
        exit(EXIT_SUCCESS);
    } else if (strncmp(input_buffer->buffer, ".create", 6) == 0) {
        printf("Create\n");

        char* command = strtok(input_buffer->buffer, " ");
        char* type_of_element = strtok(NULL, " ");
        char* name_of_element = strtok(NULL, " ");
        printf("%s",type_of_element);
        printf("\n");
        printf("%s",name_of_element);
        printf("\n");

        printf("Node\n");
        
            MetaTable* meta_table = (MetaTable*) malloc(sizeof(MetaTable));
            meta_table->type = NODE;
            int length = strlen(name_of_element);
            printf("%i", length);
            printf("\n");
            meta_table->name = malloc(sizeof(char) * length);
            strcpy(meta_table->name, name_of_element); 
            return meta_table;
        
        if (strcmp(type_of_element, "node")) {
            printf("Node\n");
        
            MetaTable* meta_table = (MetaTable*) malloc(sizeof(MetaTable));
            meta_table->type = NODE;
            int length = strlen(name_of_element);
            printf("%i", length);
            printf("\n");
            meta_table->name = malloc(sizeof(char) * length);
            strcpy(meta_table->name, name_of_element); 
            return meta_table;
        
        } else if (strcmp(type_of_element, "rel")) {
        
            MetaTable* meta_table = (MetaTable*) malloc(sizeof(MetaTable));
            meta_table->type = RELATIONSHIP;
            strcpy(meta_table->name, name_of_element); 
            return meta_table;
        
        } else if (strcmp(type_of_element, "prop")) {
            MetaTable* meta_table = (MetaTable*) malloc(sizeof(MetaTable));
            meta_table->type = PROPERTY;
            strcpy(meta_table->name, name_of_element); 
            return meta_table;
        } else {
            //FIXME: handle error
            return NULL;
        }
    } else {
        //FIXME: handle error
        return NULL;
    }
}

void close_input_buffer(InputBuffer* input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}

void serialize_row(MetaPage* source, void* destination) {
    // printf("%i", source->type);
    // FILE* file = fopen("data", "rb+");

    // fwrite(&(source->type), sizeof (uint32_t), 1, file);
    // fclose(file);


//   memcpy(destination, &(source->type), sizeof(TypeOfElement));
  // memcpy(destination + USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
  // memcpy(destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
  // Чтобы все биты были 0, инициализированными
//   memcpy(destination + sizeof(TypeOfElement), &(source->first_block), sizeof(uint32_t));
//   memcpy(destination + sizeof(TypeOfElement) + sizeof(uint32_t), &(source->next_block), sizeof(uint32_t));
memcpy(destination, "sasaovch", sizeof(char) * 8);
}

void write_to_file(MetaPage* source, File* file) {
    lseek(file->file_descriptor, 0, SEEK_SET);
    void* page = malloc(sizeof(MetaPage));
    // ssize_t bytes_read = read(file->file_descriptor, page, sizeof(MetaPage));
    // if (bytes_read == -1) {
    //     printf("Error reading file: %d\n", 1);
    //     exit(EXIT_FAILURE);
    // }
    serialize_row(source, page);
    off_t offset = lseek(file->file_descriptor, 1, SEEK_SET);
    if (offset == -1) {
    printf("Error seeking: %d\n", 1);
    exit(EXIT_FAILURE);
    }

      ssize_t bytes_written =
      write(file->file_descriptor, page, sizeof(char) * 6);

  if (bytes_written == -1) {
    printf("Error writing: %d\n", 1);
    exit(EXIT_FAILURE);
  }

//   memcpy(&(file->file_descriptor), &(source->type), sizeof(TypeOfElement));
//   memcpy(&(file->file_descriptor) + sizeof(TypeOfElement), &(source->first_block), sizeof(uint32_t));
//   memcpy(&(file->file_descriptor) + sizeof(TypeOfElement) + sizeof(uint32_t), &(source->next_block), sizeof(uint32_t));
//   memcpy(destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
  // Чтобы все биты были 0, инициализированными
//   strncpy(destination + USERNAME_OFFSET, source->username, USERNAME_SIZE);
//   strncpy(destination + EMAIL_OFFSET, source->email, EMAIL_SIZE);
}

// void execute_operation(MetaTable* meta_table, File* file) {
//     printf("%s", meta_table->name);
//     printf("\n");
//     printf("%i", meta_table->type);
//     printf("\n");
//     MetaPage* meta_page = (MetaPage*) malloc(sizeof(MetaPage));
//     meta_page->type = meta_table->type;
//     meta_page->first_block = 0;
//     meta_page->last_block = 0;
//     meta_page->next_block = 0;
//     printf("Writing");

//     write_to_file(meta_page, file);
//     printf("Writed");
// }

int main(int argc, char* argv[]) {
    MetaPage data[] = {
        {0, NODE, 111, 1.1f, 1, 8, "sasaovchвавыа"},
        {0, RELATIONSHIP, 2222, 15.12f, 0, 6, "andrey"},
        {0, PROPERTY, 2727, 27.13f, 1, 4, "lett"},
    };

    // // if (argc < 2) {
    // //     printf("Not enough parameters\n");
    // //     exit(EXIT_FAILURE);
    // // }

    // // char* filename = argv[1];
    char* filename = "data";
    File* file = db_open(filename);
    void* page = malloc(PAGE_SIZE);
    int count_to_write = sizeof(data) / sizeof(MetaPage);
    uint32_t next_block = 0;
    for (int i = 0; i < count_to_write; i++) {
        MetaPage data_to_write = data[i];
        data_to_write.next_block = next_block + sizeof(MetaPage) + data_to_write.string_length;
        memcpy(page + next_block, &(data_to_write), sizeof(MetaPage));
        memcpy(page + next_block + sizeof(MetaPage), data_to_write.string, sizeof(char) * data_to_write.string_length);
        next_block = data_to_write.next_block;
    }
    off_t offset = lseek(file->file_descriptor, 0, SEEK_SET);
    if (offset == -1) {
        printf("Error seeking: %d\n", 1);
        exit(EXIT_FAILURE);
    }
    ssize_t bytes_written =
        write(file->file_descriptor, page, PAGE_SIZE);

    if (bytes_written == -1) {
        printf("Error writing: %d\n", 1);
        exit(EXIT_FAILURE);
    }

    free(page);

    void* read_page = malloc(PAGE_SIZE);
    lseek(file->file_descriptor, 0, SEEK_SET);
    ssize_t bytes_read = read(file->file_descriptor, read_page, PAGE_SIZE);
    if (bytes_read == -1) {
        printf("Error reading file");
        exit(EXIT_FAILURE);
    }

    next_block = 0;
    for (int i = 0; i < 3; i++) {
        MetaPage read_meta;
        memcpy(&(read_meta), read_page + next_block, sizeof(MetaPage));
        read_meta.string = malloc(sizeof(char) * read_meta.string_length);
        memcpy(read_meta.string, read_page + next_block + sizeof(MetaPage), sizeof(char) * read_meta.string_length);
        printf("Print\n");
        printf("%i\n", read_meta.next_block);
        printf("%i\n", read_meta.type);
        printf("%i\n", read_meta.number);
        printf("%f\n", read_meta.real_number);
        printf("%i\n", read_meta.boolean);
        printf("%i\n", read_meta.string_length);
        printf("%s\n", read_meta.string);
        next_block += sizeof(MetaPage) + read_meta.string_length;
    }

    // InputBuffer* input_buffer = new_input_buffer();
    // while (true) {
        // print_prompt();
        // read_input(input_buffer);

        // if (input_buffer->buffer[0] == '.') {
            // MetaTable* meta_table = prepare_meta_command(input_buffer);
            // printf("Prepared\n");
            // if (meta_table != NULL) {
                // execute_operation(meta_table, file);
            // }
        // }
    // }
    // close_input_buffer(input_buffer);
    return 0;
}

//     const char *filename = "new.bin";
//     file = fopen(filename, "wb");

//     if (file == NULL) {
//         printf("The file is not opened. The program will "
//                "exit now");
//         exit(0);
//     }
//     else {
//         unsigned char buffer[3] = {'1', '2', '3'};
//         fwrite(buffer, sizeof(buffer), 1, file); 
//         // fprintf(file, "%s %s %s %d", "We", "are", "in", 2012);
//         // fputc('a', file);

//         printf("The file is created Successfully.");
//     }
   
//     return 0;
// }