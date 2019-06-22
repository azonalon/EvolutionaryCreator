add_executable(EvolutionaryCreator Main.cpp)
add_executable(FluidTest test/FluidTest.cpp)
add_library(OpenGLContext render/OpenGLContext.cpp)
target_link_libraries(EvolutionaryCreator OpenGLContext imgui glfw GL GLEW Evolutionary)
target_link_libraries(FluidTest OpenGLContext imgui glfw GL GLEW Evolutionary)
set(source "${PROJECT_ROOT}/resources")
set(destination "${BUILD_ROOT}/resources")
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${source}" "${destination}"
    DEPENDS ${destination}
    COMMENT "symbolic link resources folder from ${source} => ${destination}"
)
