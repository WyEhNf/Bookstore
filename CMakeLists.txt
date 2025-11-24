cmake_minimum_required(VERSION 3.15)
project(BookStore)

set(CMAKE_CXX_STANDARD 20)

add_executable(
    code
    include/Chars.hpp
    include/MemoryRiver.hpp
    include/FileStore.hpp
    include/Finance.hpp
    include/Financesys.hpp
    include/read.hpp
    include/user.hpp
    include/UserStore.hpp
    include/UserStack.hpp
    include/Book.hpp
    include/Manager.hpp
    src/main.cpp
)
