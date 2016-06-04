
project(ctrl)

add_library(ctrl SHARED src/writeBuffer.cpp
                src/polymorphicSerializer.cpp
                src/polymorphicFactory.cpp
                src/readBuffer.cpp
                src/readRawPointerRepository.cpp
                src/writePointerRepository.cpp)

target_include_directories(ctrl PUBLIC include)
set_property(TARGET ctrl PROPERTY CXX_STANDARD 11)

add_executable(test-ctrl test/test.cpp)

target_include_directories(test-ctrl PUBLIC include)
target_link_libraries(test-ctrl ctrl)
set_property(TARGET test-ctrl PROPERTY CXX_STANDARD 11)

install(TARGETS ctrl DESTINATION lib)

install(DIRECTORY include/ctrl DESTINATION include)