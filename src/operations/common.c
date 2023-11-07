#include "../../include/operations/common.h"

#include "../../include/utils/io_utils.h"
#include "utils/checker.h"
#include "utils/logger.h"
#include "utils/page_utils.h"

#include <string.h>
//FIXME: good
bool create_element(
    Cursor *cursor, const void *element,
    TypeOfElement element_type, const char *type,
    const FunctionHelper *function_helper,
    const uint32_t *id
) {
    if (check_is_null_arg(cursor, "cursor") ||
        check_is_null_arg(type, "type") ||
        check_is_null_arg(function_helper, "function_helper")) {
            return false;
    }
    uint64_t pointer = 0;
    
    Entity *entity = get_entity(cursor, element_type, type, &(pointer));
    if (check_is_null_returned(entity, "get_entity")) {
        free(entity);
        LOG_INFO("Error to find Entity with name %s", type);
        return false;
    }

    uint32_t page_num = entity->last_page;
    Page *page = read_page_from_file(cursor, page_num);

    uint64_t page_offset = page->page_header->offset;
    uint64_t global_offset = page_num * BLOCK_SIZE + page_offset + PAGE_HEADER_SIZE;

    // если недостаточно места на странице, то создаем новую,
    // для элементов, которые больше чем страница, не нужно создавать новую страницу, если текущая пустая
    if (page_offset + function_helper->get_size_of_element(element) > PAGE_BODY_SIZE && page_offset != 0) {
        Page *n_page = get_free_page(cursor, page);
        memcpy_page(page, n_page);
        free_page(n_page);
        
        entity->last_page = page->page_header->block_number;
        global_offset = page->page_header->block_number * BLOCK_SIZE + PAGE_HEADER_SIZE;
    }

    set_pointer_offset_file(cursor->file, global_offset);
    function_helper->write_element_to_file(cursor, page, entity, element, id);

    // на странице с большим элементом нет других элементов
    if (function_helper->get_size_of_element(element) > PAGE_BODY_SIZE) {
        Page *n_page = page;
        n_page = get_free_page(cursor, page);
        memcpy_page(page, n_page);
        free_page(n_page);
        entity->last_page = page->page_header->block_number;
    }

    set_pointer_offset_file(cursor->file, pointer + TYPE_OF_ELEMENT_SIZE + VALUE_TYPE_SIZE + UINT32_T_SIZE);
    write_uint_32_to_file(cursor->file, entity->last_page);
    write_uint_32_to_file(cursor->file, entity->next_id);

    flush(cursor->file);

    free(entity);
    free_page(page);

    return true;
}

Iterator *select_element(
    const Cursor *cursor,
    TypeOfElement element_type, const char *type,
    uint64_t size_of_element_malloc, 
    const void *helper, 
    const FunctionHelper *function_helper
) {
    if (check_is_null_arg(cursor, "cursor") ||
        check_is_null_arg(type, "type") ||
        check_is_null_arg(function_helper, "function_helper") ||
        check_is_null_arg(helper, "helper")) {
            return NULL;
    }

    uint64_t pointer = 0;
    Entity *entity = get_entity(cursor, element_type, type, &(pointer));
    if (entity == NULL) {
        free(entity);
        LOG_INFO("Error to find Entity with name %s", type);
        return NULL;
    }

    void *element = (void*) malloc(size_of_element_malloc);    
    
    uint64_t *offset_ = malloc(UINT64_T_SIZE);
    *offset_ = 0;
    uint32_t *read_block_ = malloc(UINT32_T_SIZE);
    *read_block_ = entity->first_page;;

    Iterator *iterator = (Iterator*) malloc(ITERATOR_SIZE);

    iterator->cursor = cursor;
    iterator->entity = entity;
    iterator->read_block_ = read_block_;
    iterator->offset_ = offset_;
    iterator->element = element;
    iterator->helper = helper;
    iterator->function_helper = function_helper;
    
    return iterator;
}

/*
    offset указывает на начало элемента
    page хранит первый блок с элементом
*/
void *find_element(
    const Cursor *cursor, const Page *page, 
    uint64_t size_of_element_malloc, 
    const void *find_elem, uint64_t *offset_,
    const FunctionHelper *function_helper
) {
    if (check_is_null_arg(cursor, "cursor") ||
        check_is_null_arg(page, "page") ||
        check_is_null_arg(function_helper, "function_helper")) {
            return NULL;
    }
    uint32_t read_block = page->page_header->block_number;
    void *element = (void*) malloc(size_of_element_malloc);

    Page *curr_page = NULL;
    Page *prev_page = new_page();

    while (read_block != 0) {
        curr_page = read_page_from_file(cursor, read_block);
        memcpy_page(prev_page, curr_page);

        uint64_t offset = 0;
        while (offset < curr_page->page_header->offset) {
            element = function_helper->read_element_from_file(cursor, curr_page, &(offset));

            if (function_helper->condition(element, find_elem)) {
                memcpy_page(page, prev_page);
                *offset_ = offset;
                free_page(curr_page);
                free_page(prev_page);
                return element;
            }
            offset = (offset + function_helper->get_size_of_element(element)) % PAGE_BODY_SIZE;
        }
        read_block = curr_page->page_header->next_block;
        free_page(curr_page);
    }
    free(element);
    // здесь page - последняя страница в списке
    memcpy_page(page, prev_page);
    free_page(prev_page);
    return NULL;
}

/* 
    page указывает на следующую страницу
*/
void remove_bid_element(
    const Cursor *cursor, const Page *page, 
    Entity *entity, uint64_t size_of_element,
    const uint64_t *pointer
) {
    if (check_is_null_arg(cursor, "cursor") ||
        check_is_null_arg(page, "page") ||
        check_is_null_arg(entity, "entity")) {
            return;
    }

    uint32_t page_numbers = size_of_element / PAGE_BODY_SIZE;
    if (size_of_element % PAGE_BODY_SIZE) {
        page_numbers++;
    }

    Page *n_page = new_page();
    memcpy_page(n_page, page);
    char *empty_body = (char *) calloc(1, PAGE_BODY_SIZE);

    //блок с которого начинаем удаление
    uint32_t start_page_number = n_page->page_header->block_number;
    //блок перед ним, (если start_page_number == entity->first_page, то они равны)
    uint32_t prev_page_number = find_page_before(cursor, n_page->page_header->block_number, entity->first_page);
    //блок за ни, (если start_page_number == entity->last_page, то они равны)
    uint32_t next_page_number = start_page_number;

    push(cursor->empty_pages, start_page_number);
    for (uint32_t i = 0; i < page_numbers; i++) {
        n_page->page_header->offset = 0;
        memcpy(n_page->page_body, empty_body, PAGE_BODY_SIZE);
        write_page_to_file_flush(cursor, n_page);

        // для последнего блока нужно занулить next_block, чтобы отсоединить от общего списка
        if (i == page_numbers - 1) {
            uint32_t curr_page_number = n_page->page_header->block_number;
            set_pointer_offset_file(cursor->file, curr_page_number * BLOCK_SIZE + UINT32_T_SIZE);
            write_uint_32_to_file(cursor->file, 0);
        }

        next_page_number = n_page->page_header->next_block;
        free_page(n_page);
        n_page = read_page_from_file(cursor, next_page_number);
    }

    //если блоки находится где-то посередине, то нужно добавить в стек верхний и связать предыдущий блок со следующим
    if (start_page_number != entity->first_page) {
        //связать пердыдущий со следущим
        set_pointer_offset_file(cursor->file, prev_page_number * BLOCK_SIZE + UINT32_T_SIZE);
        write_uint_32_to_file(cursor->file, next_page_number);
    } else {
        set_pointer_offset_file(cursor->file, *pointer + TYPE_OF_ELEMENT_SIZE + VALUE_TYPE_SIZE);
        write_uint_32_to_file(cursor->file, next_page_number);
        entity->first_page = next_page_number;
    }

    memcpy_page(page, n_page);
}

void remove_small_element(
    const Cursor *cursor, const Page *page,
    Entity *entity, uint32_t offset, 
    uint64_t size_of_element, const uint64_t *pointer
) {
    if (check_is_null_arg(cursor, "cursor") ||
        check_is_null_arg(page, "page") ||
        check_is_null_arg(entity, "entity")) {
            return;
    }
    Page *n_page = read_page_from_file(cursor, page->page_header->block_number);

    char *new_body = (char *) calloc(1, PAGE_BODY_SIZE);
    memcpy(new_body, n_page->page_body, offset);
    memcpy(new_body + offset, n_page->page_body + offset + size_of_element, PAGE_BODY_SIZE - offset - size_of_element);

    uint64_t new_offset = n_page->page_header->offset - size_of_element;
    n_page->page_header->offset = new_offset;
    memcpy(n_page->page_body, new_body, PAGE_BODY_SIZE);

    write_page_to_file_flush(cursor, n_page);
    
    if (new_offset == 0) {
        uint32_t delete_page_number = n_page->page_header->block_number;
        //Если delete_page_number - первый блок, то prev_page_number = delete_page_number
        uint32_t prev_page_number = find_page_before(cursor, delete_page_number, entity->first_page);
        uint32_t next_page_number = n_page->page_header->next_block;

        push(cursor->empty_pages, delete_page_number);

        //если блок является первым блоком в цепочке и не является последним, то нужно изменить начальный блок для entity
        if (delete_page_number == entity->first_page && delete_page_number != entity->last_page) {
            set_pointer_offset_file(cursor->file, *pointer + TYPE_OF_ELEMENT_SIZE + VALUE_TYPE_SIZE);
            write_uint_32_to_file(cursor->file, next_page_number);
            entity->first_page = next_page_number;
        }
        
        //если блок является последним, что нужно изменить начальный блок для entity
        if (delete_page_number == entity->last_page) {
            set_pointer_offset_file(cursor->file, *pointer + TYPE_OF_ELEMENT_SIZE + VALUE_TYPE_SIZE + UINT32_T_SIZE);
            write_uint_32_to_file(cursor->file, prev_page_number);
            entity->first_page = prev_page_number;
        }

        //если блок является и первым и последним, то его не нужно помечать удаленным
        if (delete_page_number == entity->first_page && delete_page_number == entity->last_page) {
            pop(cursor->empty_pages);
        }

        set_pointer_offset_file(cursor->file, prev_page_number * BLOCK_SIZE + UINT32_T_SIZE);
        write_uint_32_to_file(cursor->file, next_page_number);
        
        //отделяем удаленный блок от основной цепочки
        set_pointer_offset_file(cursor->file, delete_page_number * BLOCK_SIZE + UINT32_T_SIZE);
        write_uint_32_to_file(cursor->file, 0);
    }

    if (n_page->page_header->next_block != 0 && new_offset == 0) {
        Page *next_page = read_page_from_file(cursor, n_page->page_header->next_block);
        memcpy_page(page, next_page);
    } else if (new_offset == 0) {
        page->page_header->block_number = 0;
    } else {
        memcpy_page(page, n_page);    
    }
    
    free_page(n_page);
    free(new_body);
}

bool delete_element(
    const Cursor *cursor, const void *element, 
    uint64_t size_of_sturcture, 
    const void *type, TypeOfElement element_type, 
    const FunctionHelper *function_helper
) {
    if (check_is_null_arg(cursor, "cursor") ||
        check_is_null_arg(type, "type") ||
        check_is_null_arg(function_helper, "function_helper") ||
        check_is_null_arg(element, "element")) {
            return false;
    }

    uint64_t pointer = 0;
    uint64_t offset = 0;

    Entity *entity = get_entity(cursor, element_type, type, &(pointer));
    if (entity == NULL) {
        LOG_INFO("Error to find Node with name %s", type);
        free(entity);
        return false;
    }

    Page *page = read_page_from_file(cursor, entity->first_page);
    void *find_el = find_element(cursor, page, size_of_sturcture, element, &(offset), function_helper);
    if (find_el == NULL) {
        free_page(page);
        free(entity);
        return false;
    }

    while (find_el != NULL) {
        uint64_t size_of_element = function_helper->get_size_of_element(find_el);
    
        if (size_of_element > PAGE_BODY_SIZE) {
            remove_bid_element(cursor, page, entity, size_of_element, &(pointer));
        } else {
            remove_small_element(cursor, page, entity, offset, size_of_element, &(pointer));
        }

        flush(cursor->file);
        free(find_el);

        if (page->page_header->block_number == 0) break;
        find_el = find_element(cursor, page, size_of_sturcture, element, &(offset), function_helper);
    }

    free_page(page);
    free(entity);
    return true;
}

uint32_t *delete_and_create_element(
    Cursor *cursor, Page *page, 
    Entity *entity, uint64_t pointer,
    uint64_t* offset, 
    const void *old_element, const void *new_element, 
    const FunctionHelper *function_helper
) {
    uint64_t size_of_element = function_helper->get_size_of_element(old_element);
    
    if (size_of_element > PAGE_BODY_SIZE) {
        remove_bid_element(cursor, page, entity, size_of_element, &(pointer));
    } else {
        remove_small_element(cursor, page, entity, *offset, size_of_element, &(pointer));
    }

    flush(cursor->file);
    
    uint32_t *id = (uint32_t *) malloc(UINT32_T_SIZE);
    *id = function_helper->get_id(old_element);

    if (!create_element(cursor, new_element, entity->element_type, entity->type, function_helper, id)) {
        LOG_DEBUG("Error to create elemnt for update", "");
        return NULL;
    }
    return id;
}

bool update_element(
    Cursor *cursor, const void *old_element, 
    const void *new_element, uint64_t size_of_sturcture, 
    const void *type, TypeOfElement element_type, 
    const FunctionHelper *function_helper
) {
    if (check_is_null_arg(cursor, "cursor") ||
        check_is_null_arg(type, "type") ||
        check_is_null_arg(function_helper, "function_helper") ||
        check_is_null_arg(new_element, "new_element") ||
        check_is_null_arg(old_element, "old_element")) {
            return false;
    }

    uint64_t pointer = 0;
    uint64_t offset = 0;
    
    Entity *entity = get_entity(cursor, element_type, type, &(pointer));
    if (entity == NULL) {
        LOG_INFO("Error to find Node with name %s", type);
        free(entity);
        return false;
    }

    Page *page = read_page_from_file(cursor, entity->first_page);
    void *find_el = find_element(cursor, page, size_of_sturcture, old_element, &(offset),  function_helper);
    if (find_el == NULL) {
        free_page(page);
        free(entity);
        return false;
    }

    Stack *stack = new_stack(256);
    while (find_el != NULL && 
        !stack_contains(stack, function_helper->get_id(find_el))
    ) {
        uint32_t *id = delete_and_create_element(cursor, page, entity, pointer, &(offset), find_el, new_element, function_helper);
        push(stack, *id);
        
        free(find_el);
        free(id);
        
        if (page->page_header->block_number == 0) break;
        find_el = find_element(cursor, page, size_of_sturcture, old_element, &(offset),  function_helper);
    }

    free_page(page);
    free(entity);
    destroy(stack);
    return true;
}
