cmake_minimum_required(VERSION 3.10)
project(fom_farm_optimizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
  HIGHS
  GIT_REPOSITORY https://github.com/ERGO-Code/HiGHS
  GIT_TAG v1.15.1
)
FetchContent_MakeAvailable(HIGHS)
