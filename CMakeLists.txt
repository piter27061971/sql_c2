cmake_minimum_required(VERSION 3.20)
project(MyProject1)
add_executable(MyProject1 main.cpp)
add_subdirectory(C:\\Users\\Дом\\source\\repos\\libpqxx-7.9.1   libpqxx-build) # Укажите путь к libpq++
target_compile_features(MyProject1 PRIVATE cxx_std_17) # Включите C++17
target_link_libraries(MyProject1 pqxx) # Слинкуйте libpq++ к проекту