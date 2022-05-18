cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(XES_SQ3_Converter)

include_directories("${PROJECT_SOURCE_DIR}")

add_executable(
    XES_SQ3_Converter
    includes/exceptions.hpp
    includes/XesToSq3Handler.hpp
    includes/XesSq3Converter.hpp
    includes/PrepareDatabase.hpp
    includes/XesToSq3Converter.hpp
    includes/utils.hpp
    includes/XesElements.hpp
    includes/XesWriter.hpp
    includes/Sq3ToXesConverter.hpp

    src/exceptions.cpp
    src/XesToSq3Handler.cpp
    src/XesSq3Converter.cpp
    src/PrepareDatabase.cpp
    src/XesToSq3Converter.cpp
    src/XesElements.cpp
    src/utils.cpp
    src/XesWriter.cpp
)


find_package(XercesC 3.2.0 REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(Boost REQUIRED)
target_link_libraries(XES_SQ3_Converter PRIVATE XercesC::XercesC SQLite::SQLite3 ${Boost_LIBRARIES})
