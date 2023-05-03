#project name
project (SE_Kernel)

file(GLOB SOURCES "src/*/*.c" "src/*.c")

target_include_directories(target
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include/*
)

add_executable(hello_age main.cpp age.cpp)