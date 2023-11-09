#include "../../include/data/constants.h"

#include "../../include/data/node.h"
#include "../../include/data/property.h"
#include "../../include/data/relationship.h"
#include "data/function_helper.h"
#include "data/iterator.h"
#include <stdint.h>

const uint32_t BLOCK_SIZE = 4096;
const uint32_t START_PAGE = 1;
const int ERROR_EXIT_CODE = -1;
const uint64_t UINT32_T_SIZE = sizeof(uint32_t);
const uint64_t UINT64_T_SIZE = sizeof(uint64_t);
const uint64_t CHAR_SIZE = sizeof(char);
const uint64_t PAGE_SIZE = sizeof(Page);
const uint64_t NODE_SIZE = sizeof(Node);
const uint64_t PROPERTY_SIZE = sizeof(Property);
const uint64_t RELATIONSHIP_SIZE = sizeof(Relationship);
const uint64_t ENTITY_SIZE = sizeof(Entity);
const uint64_t PAGE_HEADER_SIZE = sizeof(PageHeader);
const uint64_t VALUE_TYPE_SIZE = sizeof(ValueType);
const uint64_t TYPE_OF_ELEMENT_SIZE = sizeof(TypeOfElement);
const uint64_t FUNCTION_HELPER = sizeof(FunctionHelper);
const uint64_t ITERATOR_SIZE = sizeof(Iterator);
const uint64_t EMTITY_ITERATOR_SIZE = sizeof(EntityIterator);
const uint64_t PAGE_BODY_SIZE = (BLOCK_SIZE - PAGE_HEADER_SIZE);
const uint32_t NAME_TYPE_WITH_TERM_LENGTH = (NAME_TYPE_LENGTH + 1);
const uint32_t NAME_TYPE_SIZE = NAME_TYPE_WITH_TERM_LENGTH * CHAR_SIZE;
