set(This Utils)

set(Sources
    include/Utils/Thread.hpp)

add_library(${This} INTERFACE)

target_include_directories(${This} INTERFACE include/)
