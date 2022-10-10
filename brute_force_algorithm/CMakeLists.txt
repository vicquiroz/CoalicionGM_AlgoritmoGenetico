cmake_minimum_required (VERSION 3.8)

project ("brute_force_algorithm")

include_directories(
	${PROJECT_SOURCE_DIR}/include
)

file(GLOB all_SRCS
	"${PROJECT_SOURCE_DIR}/src/*.cpp"
	"${PROJECT_SOURCE_DIR}/src/*.c"
	"${PROJECT_SOURCE_DIR}/src/*.h"
)
file(GLOB all_DATA
	"${PROJECT_SOURCE_DIR}/data/*.json"
)
# Agregue un origen al ejecutable de este proyecto.
add_executable (brute_force_algorithm ${all_SRCS} ${all_DATA})
set_target_properties(brute_force_algorithm PROPERTIES LINKER_LANGUAGE CXX)
file(COPY ${all_DATA} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries(brute_force_algorithm PRIVATE nlohmann_json::nlohmann_json)