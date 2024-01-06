############################## CRITERION SETUP ##############################
find_package(PkgConfig)

find_path(CRITERION_INCLUDE_DIR criterion/criterion.h
          PATH_SUFFIXES criterion)

find_library(CRITERION_LIBRARY NAMES criterion libcriterion)

set(CRITERION_LIBRARIES ${CRITERION_LIBRARY})
set(CRITERION_INCLUDE_DIRS ${CRITERION_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
# handle the QUIET and REQUIRED arguments and set CRITERION_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(Criterion DEFAULT_MSG
                                  CRITERION_LIBRARY CRITERION_INCLUDE_DIR)

mark_as_advanced(CRITERION_INCLUDE_DIR CRITERION_LIBRARY)

#############################################################################

add_executable(test_runner ${CMAKE_CURRENT_LIST_DIR}/test_runner.c ${sources}
    ${CMAKE_CURRENT_LIST_DIR}/test_attacks.c
    ${CMAKE_CURRENT_LIST_DIR}/test_perft.c
)
target_include_directories(test_runner PRIVATE src)
target_link_libraries(test_runner PRIVATE criterion)
