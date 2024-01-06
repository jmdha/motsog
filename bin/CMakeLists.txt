add_executable(jarlo ${CMAKE_CURRENT_LIST_DIR}/uci.c ${sources})
target_include_directories(jarlo PRIVATE src)
