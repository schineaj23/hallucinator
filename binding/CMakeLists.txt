cmake_minimum_required(VERSION 3.15)
project(link_wrapper)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find Python
set(Python3_FIND_VIRTUALENV FIRST)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED) 

# Add pybind11 subdirectory (assuming it's in extern/pybind11)
add_subdirectory(extern/pybind11)
pybind11_add_module(link_wrapper src/Link.cpp)

include(extern/link/AbletonLinkConfig.cmake)
target_link_libraries(link_wrapper PUBLIC Ableton::Link)

install(TARGETS link_wrapper LIBRARY DESTINATION .)

# # Include directories for Link (assuming it's in extern/link)
# target_include_directories(link_wrapper PRIVATE
#     extern/link/include
#     extern/link/modules/asio-standalone/asio/include
# )

# # Set platform-specific compile definitions and libraries
# if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
#     find_package(Threads REQUIRED)
#     target_link_libraries(link_wrapper PRIVATE Threads::Threads)
#     target_compile_definitions(link_wrapper PRIVATE LINK_PLATFORM_LINUX=1)
# elseif(APPLE)
#     target_compile_definitions(link_wrapper PRIVATE LINK_PLATFORM_MACOSX=1)
#     find_library(CORE_FOUNDATION CoreFoundation)
#     find_library(CORE_AUDIO CoreAudio)
#     target_link_libraries(link_wrapper PRIVATE ${CORE_FOUNDATION} ${CORE_AUDIO})
# elseif(WIN32)
#     target_compile_definitions(link_wrapper PRIVATE LINK_PLATFORM_WINDOWS=1 _WIN32_WINNT=0x0601)
#     target_link_libraries(link_wrapper PRIVATE iphlpapi winmm)
# endif()

# Set output directory
set_target_properties(link_wrapper PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

# Configure setup.py for pip installation
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in
    ${CMAKE_CURRENT_BINARY_DIR}/setup.py
    @ONLY
)

# Add custom target for pip installation
add_custom_target(pip-install
    COMMAND ${Python3_EXECUTABLE} -m pip install .
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS link_wrapper
    COMMENT "Installing package with pip..."
)

