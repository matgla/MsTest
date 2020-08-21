add_library(mstest)

set(include_dir ${PROJECT_SOURCE_DIR}/include/mstest)

target_sources(mstest
    PUBLIC
        ${include_dir}/mstest.hpp
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/runner.cpp
)

target_include_directories(mstest
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(mstest
    PUBLIC
    $<$<COMPILE_LANGUAGE:CXX>:-std=c++2a ${MSTEST_CXX_COMPILER_FLAGS}>
    $<$<CONFIG:DEBUG>:-Og -g>
    $<$<CONFIG:RELEASE>:-Os>
)

target_link_options(mstest
    PUBLIC
        ${MSTEST_LINKER_FLAGS}
)
