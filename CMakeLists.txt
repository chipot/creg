cmake_minimum_required(VERSION 3.1)

project(registry)

# everyone need dl.so
link_libraries(${CMAKE_DL_LIBS})

# create static and shared versions of i.c
add_library(i-shared SHARED i.c)
add_library(i-static STATIC i.c)

# create an executable with no plugins, but which will load any SO passed as param
add_executable(reg-dlopen reg.c)

# create an executable that dynamicaly link with i-shared
add_executable(reg-shared reg.c)
target_link_libraries(reg-shared i-shared)

# create an executable that staticaly link with i-static
add_executable(reg-static reg.c)
target_link_libraries(reg-static -Wl,--whole-archive i-static -Wl,--no-whole-archive)

# create an executable where i is built-in.
add_executable(reg-builtin reg.c i.c)
