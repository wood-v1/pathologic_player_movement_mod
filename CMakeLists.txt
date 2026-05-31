cmake_minimum_required(VERSION 3.10.0)
project(PPMM VERSION 0.1.0 LANGUAGES C CXX)

if(CMAKE_SIZEOF_VOID_P AND NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR "PPMM must be built as Win32/x86 to hook Pathologic Game.exe.")
endif()

set(_default_oynontools_root "${CMAKE_CURRENT_SOURCE_DIR}/../OynonTools")
set(OYNONTOOLS_ROOT "${_default_oynontools_root}" CACHE PATH "Path to the OynonTools repository root")
set(OYNONTOOLS_INCLUDE_DIR "${OYNONTOOLS_ROOT}/include")
set(OYNONTOOLS_LIBRARY_DIR "${OYNONTOOLS_ROOT}/lib/Win32")

if(NOT EXISTS "${OYNONTOOLS_INCLUDE_DIR}/OynonToolsApi.h")
    message(FATAL_ERROR "OynonToolsApi.h not found. Set OYNONTOOLS_ROOT to the OynonTools repository.")
endif()

add_library(PPMM SHARED
    bootstrap.cpp
    config.cpp
    debug.cpp
    dllmain.cpp
    module_context.cpp
    PPMM.def
    ppmm_api.cpp
    tiredness_effect.cpp
)

target_compile_definitions(PPMM PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX _CRT_SECURE_NO_WARNINGS PPMM_EXPORTS)
target_include_directories(PPMM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(PPMM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${OYNONTOOLS_INCLUDE_DIR})
target_link_directories(PPMM PRIVATE
    "$<$<CONFIG:Debug>:${OYNONTOOLS_LIBRARY_DIR}/Debug>"
    "$<$<CONFIG:Release>:${OYNONTOOLS_LIBRARY_DIR}/Release>"
    "$<$<CONFIG:MinSizeRel>:${OYNONTOOLS_LIBRARY_DIR}/MinSizeRel>"
    "$<$<CONFIG:RelWithDebInfo>:${OYNONTOOLS_LIBRARY_DIR}/RelWithDebInfo>")
target_link_libraries(PPMM PRIVATE OynonTools.lib)

set_target_properties(PPMM PROPERTIES PREFIX "" OUTPUT_NAME "PPMM" SUFFIX ".dll")
set_target_properties(PPMM PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

if(MSVC)
    target_compile_options(PPMM PRIVATE /W4 /EHsc)
    target_link_options(PPMM PRIVATE /SUBSYSTEM:WINDOWS)
endif()

add_custom_command(
    TARGET PPMM POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/PPMM.ini"
        "$<TARGET_FILE_DIR:PPMM>/PPMM.ini")
