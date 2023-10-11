cmake_minimum_required(VERSION 3.10)

project(lab_1_llp VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}   -Wall -Wextra -pedantic -Wmissing-prototypes -Wstrict-prototypes -fsanitize-address-use-after-scope -g")

add_executable(main src/main.c)

target_link_libraries(main ${PTHREAD_LIBRARY} ${PCRE_LIBRARY})