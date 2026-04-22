cmake_minimum_required(VERSION 3.10)

project(AbyssEngine VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

file(GLOB IMGUI_SRCS CONFIGURE_DEPENDS 
    ${PROJECT_SOURCE_DIR}/3rd/imgui-1.92.2b/*.cpp
    ${PROJECT_SOURCE_DIR}/3rd/imgui-1.92.2b/backends/*.cpp
)

file(GLOB SRCS CONFIGURE_DEPENDS 
    ${PROJECT_SOURCE_DIR}/src/Function/*.cpp
    ${PROJECT_SOURCE_DIR}/src/GUI/*.cpp
    ${PROJECT_SOURCE_DIR}/src/Material/*.cpp
    ${PROJECT_SOURCE_DIR}/src/Object/*.cpp
    ${PROJECT_SOURCE_DIR}/src/Physics/*.cpp
    ${PROJECT_SOURCE_DIR}/src/Render/*.cpp
    ${PROJECT_SOURCE_DIR}/src/World/*.cpp
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

if(OpenGL_INCLUDE_DIRS AND OpenGL_LIBRARY_DIRS)    
    add_library(OpenGL STATIC IMPORTED)
    include_directories(${OpenGL_INCLUDE_DIRS})
    if(WIN32)
        set_target_properties(OpenGL PROPERTIES IMPORTED_LOCATION ${OpenGL_LIBRARY_DIRS}/opengl32.lib) 
    else()
    endif()
else()
    find_package(OpenGL REQUIRED)
endif()

if(GLEW_INCLUDE_DIRS AND GLEW_LIBRARY_DIRS)
    add_library(GLEW STATIC IMPORTED)
    include_directories(${GLEW_INCLUDE_DIRS})
    if(WIN32)
        set_target_properties(GLEW PROPERTIES IMPORTED_LOCATION ${GLEW_LIBRARY_DIRS}/glew32.lib) 
    else()
    endif()
else()
    find_package(GLEW REQUIRED)
endif()

if(glfw3_INCLUDE_DIRS AND glfw3_LIBRARY_DIRS)
    add_library(glfw3 STATIC IMPORTED) 
    include_directories(${glfw3_INCLUDE_DIRS})
    if(WIN32)
        set_target_properties(glfw3 PROPERTIES IMPORTED_LOCATION ${glfw3_LIBRARY_DIRS}/glfw3dll.lib) 
    else()
    endif()
else()
    find_package(glfw3 REQUIRED)
endif()

if(BULLET_INCLUDE_DIRS AND BULLET_LIBRARY_DIRS)
    add_library(Bullet STATIC IMPORTED)
    include_directories(${BULLET_INCLUDE_DIRS})
    if(WIN32)
        set_target_properties(Bullet PROPERTIES IMPORTED_LOCATION ${BULLET_LIBRARY_DIRS}/*.lib) 
    else()
    endif()
elseif(Bullet_DIR)
    find_package(Bullet CONFIG REQUIRED)
    include_directories(${BULLET_ROOT_DIR}/${BULLET_INCLUDE_DIRS})
else()
    find_package(Bullet REQUIRED)
endif()

#message("BULLET_INCLUDE_DIRS = ${BULLET_INCLUDE_DIRS}")

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/3rd/eigen-3.4.0)
include_directories(${PROJECT_SOURCE_DIR}/3rd/imgui-1.92.2b)
include_directories(${PROJECT_SOURCE_DIR}/3rd/stb)

add_library(imgui STATIC ${IMGUI_SRCS})
target_link_libraries(imgui PUBLIC OpenGL::GL GLEW glfw)

add_executable(AbyssEngine ${SRCS})
target_compile_definitions(AbyssEngine PUBLIC PROGRAM_DIR=${PROJECT_SOURCE_DIR})
target_link_libraries(AbyssEngine 
    LinearMath
    Bulletd
    imgui)
if(WIN32)
    add_custom_command(TARGET AbyssEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${PROJECT_SOURCE_DIR}/3rd/glew-2.2.0/lib/glew32.dll"
            $<TARGET_FILE_DIR:AbyssEngine>)    
    add_custom_command(TARGET AbyssEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${PROJECT_SOURCE_DIR}/3rd/glfw-3.3.8/glfw3.dll"
            $<TARGET_FILE_DIR:AbyssEngine>)   
else()
endif()
