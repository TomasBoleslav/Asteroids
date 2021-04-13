# Project definition
cmake_minimum_required(VERSION 3.12)
project(SpaceGame)

# Set output directories
set(BIN_DIR ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

# Source files
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(SOURCE_FILES
	"main.cpp"
	"Shader.cpp"
	"Debug.cpp"
	"Window.cpp"
	"Game.cpp"
	"Texture2D.cpp"
	"ResourceManager.cpp"
	"Renderer.cpp"
	"GameObject.cpp"
	"Player.cpp"
	"Input.cpp"
	"Asteroid.cpp"
	"Geometry.cpp"
	"Bullet.cpp"
	"Random.cpp"
	"Timer.cpp"
	)
list(TRANSFORM SOURCE_FILES PREPEND "${SRC_DIR}/")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")

# Executable definition and properties
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE "${INCLUDE_DIR}")
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

# Copy resources
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/res/ ${BIN_DIR}/res/)

# Disable console on Windows
#set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS")

# GLFW
set(GLFW_DIR "${LIB_DIR}/glfw")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory("${GLFW_DIR}")
target_link_libraries(${PROJECT_NAME} "glfw" "${GLFW_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLFW_DIR}/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")

# glad
set(GLAD_DIR "${LIB_DIR}/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")

# stb
set(STB_DIR "${LIB_DIR}/stb")
add_library("stb" "${STB_DIR}/src/stb_image.cpp")
target_include_directories("stb" PRIVATE "${STB_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${STB_DIR}/include")
target_link_libraries(${PROJECT_NAME} "stb")

# FreeType
set(FT_DIR "${LIB_DIR}/freetype")
add_subdirectory(${FT_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE "${FT_DIR}/include")
target_link_libraries(${PROJECT_NAME} "freetype")