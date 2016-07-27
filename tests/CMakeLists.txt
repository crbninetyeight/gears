find_package (Boost COMPONENTS system chrono REQUIRED)
find_package (SDL2 REQUIRED)
find_package (GLEW REQUIRED)
find_package (OpenGL REQUIRED)

include_directories (../include/)

add_executable (window_test window_test.cpp)
target_link_libraries (window_test 
    gearsengine
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_CHRONO_LIBRARY}
    ${SDL2_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${OPENGL_LIBRARIES}
)

add_executable (sandbox sandbox.cpp)
target_link_libraries (sandbox
    gearsengine
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_CHRONO_LIBRARY}
    ${SDL2_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${OPENGL_LIBRARIES}
)
