cmake_minimum_required(VERSION 2.8)

add_compile_options(-std=c++11)

set(CMAKE_BUILD_TYPE "Release")

project(Lancelot)

find_package(OpenMP REQUIRED)
if(OPENMP_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
endif(OPENMP_FOUND)

find_package(OpenCV REQUIRED)
find_package(serial REQUIRED)

add_subdirectory(./dsst)

include_directories(
    ${PROJECT_BINARY_DIR}/install/include
    ${serial_INCLUDE_DIRS}
)
link_directories(
    ${PROJECT_BINARY_DIR}/install/lib
    /usr/lib
)

aux_source_directory(${PROJECT_SOURCE_DIR}/src SOURCE_FILES)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}
    ${OpenCV_LIBS} 
    ${serial_LIBRARIES} 
    CorrelationTracker 
    tesseract
)

set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/install)