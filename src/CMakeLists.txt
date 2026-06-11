# This is hacky :(
file(GLOB_RECURSE simple_sources ${PROJECT_SOURCE_DIR}/src/*/*.cc ${PROJECT_SOURCE_DIR}/src/*/*/*.cc)
add_library(simple_shared SHARED ${simple_sources})
#target_compile_options(simple_shared PRIVATE -std=c++17)
target_compile_options(simple_shared PRIVATE ${SRC_CXX_FLAGS})
target_link_libraries(simple_shared Threads::Threads)
target_link_options(simple_shared PRIVATE 
                        -lredis++ 
                        -lhiredis
                        -lre2
                        )
    
######################################################################################################################
# THIRD-PARTY SOURCES
######################################################################################################################
    
macro(add_thirdparty_lib LIB_NAME)
file(GLOB_RECURSE ${LIB_NAME}_sources
        ${PROJECT_SOURCE_DIR}/third_party/${LIB_NAME}/*.cpp
        ${PROJECT_SOURCE_DIR}/third_party/${LIB_NAME}/*.cc
        ${PROJECT_SOURCE_DIR}/third_party/${LIB_NAME}/*.hpp
        ${PROJECT_SOURCE_DIR}/third_party/${LIB_NAME}/*.h
        )
add_library(thirdparty_${LIB_NAME} SHARED ${${LIB_NAME}_sources} ) 
target_link_libraries(simple_shared thirdparty_${LIB_NAME})
endmacro()

add_thirdparty_lib(tinyxml2)

# add_subdirectory(reactor)

