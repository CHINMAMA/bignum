add_library(BigNum bignum.cpp)
target_include_directories(
    BigNum
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)