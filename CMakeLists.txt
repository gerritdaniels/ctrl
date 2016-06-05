
project(ctrl)

add_library(ctrl SHARED
                src/polymorphicSerializer.cpp
                src/polymorphicFactory.cpp
                src/readRawPointerRepository.cpp
                src/abstractWriteBuffer.cpp
                src/binaryWriteBuffer.cpp
                src/xmlWriteBuffer.cpp
                src/writePointerRepository.cpp
                src/abstractReadBuffer.cpp
                src/binaryReadBuffer.cpp
                src/xmlReadBuffer.cpp)
target_include_directories(ctrl PUBLIC include)
set_property(TARGET ctrl PROPERTY CXX_STANDARD 11)

add_executable(test-ctrl test/test.cpp)
target_include_directories(test-ctrl PUBLIC include)
set_property(TARGET test-ctrl PROPERTY CXX_STANDARD 11)
target_compile_options(test-ctrl PRIVATE -finput-charset=UTF-8)
target_link_libraries(test-ctrl ctrl)

install(TARGETS ctrl DESTINATION lib)
install(DIRECTORY include/ctrl DESTINATION include)
