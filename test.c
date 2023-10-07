#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    uint32_t block_number;
    uint32_t next_block;
    uint32_t offset;
} PageHeader;

int main() {
    PageHeader* page = (PageHeader*) malloc(sizeof(PageHeader));
    printf("%i\n", page->block_number);
    ++(page->block_number);
    printf("%i\n", page->block_number);
    int num;
    printf("%i", num);
}