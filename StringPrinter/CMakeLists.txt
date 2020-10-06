set(This StringPrinter)

set(Sources 
    src/PrinterThread.hpp
    src/StringPrinter.cpp)

add_executable(${This} ${Sources})

target_link_libraries(${This}
            Utils
            pthread)