# # cmake_minimum_required(VERSION 3.22.1)
# # project(llp C)


# # set(CMAKE_C_STANDARD 11)

# # set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}   -Wall -Wextra -pedantic -Wmissing-prototypes -Wstrict-prototypes -fsanitize-address-use-after-scope -g")

# # file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/**.c)
# # file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_LIST_DIR}/include/**.h)

# # include_directories(include)

# # add_executable(${PROJECT_NAME})

# # target_sources(${PROJECT_NAME} PRIVATE ${SOURCES} PUBLIC ${HEADERS})

# # add_library(${PROJECT_NAME}_lib ${SOURCES})

# # Minimum required version of CMake
# cmake_minimum_required(VERSION 3.10)

# # Set the project's name and version
# project(MyProgram VERSION 1.0 LANGUAGES C)

# # Set the C standard
# set(CMAKE_C_STANDARD 11)
# set(CMAKE_C_STANDARD_REQUIRED True)

# # Find all the source files automatically
# file(GLOB_RECURSE SOURCE_FILES "src/**.c")

# # Add the main executable and its source files
# add_executable(MyProgram ${SOURCE_FILES})

# # Set the linker language for the "MyProgram" target explicitly
# set_target_properties(MyProgram PROPERTIES LINKER_LANGUAGE C)

# # Include the 'include' directory
# target_include_directories(MyProgram PRIVATE "include")

# # Add a custom command to automatically run the program after building it
# add_custom_command(TARGET MyProgram POST_BUILD COMMAND ${CMAKE_COMMAND} -E echo "Running program:" && ./MyProgram DEPENDS MyProgram)


cmake_minimum_required(VERSION 3.22.1)
project(llp C)


set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra  -Wpedantic -Werror -Waddress -g")

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/**.c)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_LIST_DIR}/include/**.h)

include_directories(include)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES} PUBLIC ${HEADERS})

add_library(${PROJECT_NAME}_lib ${SOURCES})