cmake_minimum_required(VERSION 3.20)
project(refinery
    VERSION 0.1.0
    DESCRIPTION "C++26 Refinement Types Library"
    LANGUAGES CXX
)

# Require C++26
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options
option(REFINERY_BUILD_TESTS "Build test suite" ON)
option(REFINERY_BUILD_EXAMPLES "Build assembly comparison examples" OFF)
option(REFINERY_INSTALL "Generate install target" ON)

# Create header-only library
add_library(refinery INTERFACE)
add_library(refinery::refinery ALIAS refinery)

target_include_directories(refinery INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Add reflection flag (required, GCC only)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(refinery INTERFACE -freflection)
endif()

# Tests
if(REFINERY_BUILD_TESTS)
    enable_testing()

    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
    )
    FetchContent_MakeAvailable(googletest)

    add_subdirectory(tests)
endif()

# Examples
if(REFINERY_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Installation
if(REFINERY_INSTALL)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    # Install headers
    install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    # Install targets
    install(TARGETS refinery
        EXPORT refineryTargets
    )

    # Generate and install config files
    install(EXPORT refineryTargets
        FILE refineryTargets.cmake
        NAMESPACE refinery::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/refinery
    )

    configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/refineryConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/refineryConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/refinery
    )

    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/refineryConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/refineryConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/refineryConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/refinery
    )
endif()
