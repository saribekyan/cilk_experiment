cmake_minimum_required(VERSION 2.8)

set(CILKFLAGS -fcilkplus -lm)
set(CILK_LIBRARIES -lcilkrts)
set(GCC_FLAGS -std=c++0x -Wall -Wextra -W -g -gdwarf-2 -O3)

set(Boost_LIBRARY_DIRS /usr/lib64)
set(Boost_LIBRARIES /usr/lib64/libboost_filesystem.so /usr/lib64/libboost_system.so /usr/lib64/libboost_program_options.so)

project( Main )

#find_package( Boost 1.54 COMPONENTS filesystem system program_options REQUIRED )

include_directories(${Boost_INCLUDE_DIR})

add_definitions(${CILKFLAGS} ${GCC_FLAGS})

add_executable(Main test.cpp)

link_directories(${Boost_LIBRARY_DIRS})

target_link_libraries(Main ${CILK_LIBRARIES} ${Boost_LIBRARIES})

