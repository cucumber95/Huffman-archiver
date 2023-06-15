find_library(archiver_library ../library)

add_executable(test test.cpp)
target_link_libraries(test archiver_library)