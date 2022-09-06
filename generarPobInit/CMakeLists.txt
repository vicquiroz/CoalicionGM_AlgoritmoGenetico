# CMakeList.txt: proyecto de CMake para algoritmoc++, incluya el origen y defina
# la lógica específica del proyecto aquí.
#
cmake_minimum_required (VERSION 3.8)

project ("generarPobInit")

include_directories(
	${PROJECT_SOURCE_DIR}/include
)

file(GLOB all_SRCS
	"${PROJECT_SOURCE_DIR}/src/*.cpp"
	"${PROJECT_SOURCE_DIR}/src/*.c"
)

# Agregue un origen al ejecutable de este proyecto.
add_executable (generarPobInit ${all_SRCS} ejemplo2.json ingles.json "src/main.cpp")
set_target_properties(generarPobInit PROPERTIES LINKER_LANGUAGE CXX)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/ejemplo2.json
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/ingles.json
	${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
# TODO: Agregue pruebas y destinos de instalación si es necesario.

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries(generarPobInit PRIVATE nlohmann_json::nlohmann_json)
