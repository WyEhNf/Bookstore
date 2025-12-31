cmake_minimum_required(VERSION 3.22)

project(BookStore)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(code
        src/Finance.cpp
        src/Financesys.cpp
        src/Manager.cpp
        src/MemoryRiver.cpp
        src/UserStack.cpp
        src/UserStore.cpp
        src/main.cpp
)
