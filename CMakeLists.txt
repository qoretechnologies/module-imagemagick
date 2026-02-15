cmake_minimum_required(VERSION 3.5.0)

project(qore-imagemagick-module)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

set(PROJECT_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

if (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.12.0")
    cmake_policy(SET CMP0074 NEW)

    if (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.18.0")
        cmake_policy(SET CMP0053 NEW)
    endif()
endif()

cmake_policy(SET CMP0042 NEW)

if (UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

find_package(Qore 2.0 REQUIRED)

# Find ImageMagick 7 MagickWand library via pkg-config
include(FindPkgConfig)

# Macro to search for MagickWand 7 .pc files
macro(_find_magickwand)
    pkg_check_modules(MAGICKWAND QUIET MagickWand-7.Q16HDRI)
    if (NOT MAGICKWAND_FOUND)
        pkg_check_modules(MAGICKWAND QUIET MagickWand-7.Q16)
    endif()
    if (NOT MAGICKWAND_FOUND)
        # Try generic name but verify it's IM7
        pkg_check_modules(MAGICKWAND QUIET MagickWand)
        if (MAGICKWAND_FOUND AND MAGICKWAND_VERSION VERSION_LESS "7.0")
            set(MAGICKWAND_FOUND FALSE)
        endif()
    endif()
endmacro()

_find_magickwand()

# If not found, search common non-standard ImageMagick 7 installation paths
if (NOT MAGICKWAND_FOUND)
    set(_IM7_EXTRA_PC_PATHS
        /opt/local/lib/ImageMagick7/lib/pkgconfig   # MacPorts
        /opt/homebrew/opt/imagemagick/lib/pkgconfig  # Homebrew (Apple Silicon)
        /usr/local/opt/imagemagick/lib/pkgconfig     # Homebrew (Intel)
    )
    foreach(_pc_path ${_IM7_EXTRA_PC_PATHS})
        if (IS_DIRECTORY "${_pc_path}")
            file(GLOB _im7_pc_files "${_pc_path}/MagickWand-7*.pc" "${_pc_path}/MagickWand.pc")
            if (_im7_pc_files)
                set(ENV{PKG_CONFIG_PATH} "${_pc_path}:$ENV{PKG_CONFIG_PATH}")
                message(STATUS "Added ${_pc_path} to PKG_CONFIG_PATH for ImageMagick 7 detection")
                _find_magickwand()
                if (MAGICKWAND_FOUND)
                    break()
                endif()
            endif()
        endif()
    endforeach()
endif()

if (NOT MAGICKWAND_FOUND)
    message(FATAL_ERROR "MagickWand 7+ library not found. Install ImageMagick 7 development packages.\n"
        "If ImageMagick 7 is installed in a non-standard location, set PKG_CONFIG_PATH to include\n"
        "the directory containing MagickWand-7.Q16HDRI.pc or MagickWand-7.Q16.pc.")
endif()
if (MAGICKWAND_FOUND AND MAGICKWAND_VERSION VERSION_LESS "7.0")
    message(FATAL_ERROR "ImageMagick 7+ required, found version ${MAGICKWAND_VERSION}. "
        "Install ImageMagick 7 development packages.")
endif()
message(STATUS "Found MagickWand: ${MAGICKWAND_LIBRARIES} (version ${MAGICKWAND_VERSION})")

# Check for deprecated/removed APIs
include(CheckSymbolExists)
set(CMAKE_REQUIRED_INCLUDES ${MAGICKWAND_INCLUDE_DIRS})
set(CMAKE_REQUIRED_LIBRARIES ${MAGICKWAND_LDFLAGS})
set(CMAKE_REQUIRED_FLAGS "${MAGICKWAND_CFLAGS_OTHER}")
check_symbol_exists(MagickReduceNoiseImage "MagickWand/MagickWand.h" HAVE_MAGICK_REDUCE_NOISE_IMAGE)
unset(CMAKE_REQUIRED_FLAGS)
unset(CMAKE_REQUIRED_INCLUDES)
unset(CMAKE_REQUIRED_LIBRARIES)
if (HAVE_MAGICK_REDUCE_NOISE_IMAGE)
    message(STATUS "MagickReduceNoiseImage: found")
else()
    message(STATUS "MagickReduceNoiseImage: not found (removed in IM 7.1+), reduceNoise() will use MagickStatisticImage")
endif()

# Check for C++11.
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

set(QPP_SRC
    src/QC_MagickImage.qpp
    src/QC_MagickDrawing.qpp
)

set(CPP_SRC
    src/imagemagick-module.cpp
    src/QoreMagickImage.cpp
    src/QoreMagickDrawing.cpp
    src/ImageMagickHelper.cpp
)

qore_wrap_qpp_value(QPP_SOURCES ${QPP_SRC})

foreach (it ${QPP_SOURCES})
    GET_FILENAME_COMPONENT(_outfile ${it} NAME_WE)
    set(QPP_DOX ${QPP_DOX} ${CMAKE_CURRENT_BINARY_DIR}/${_outfile}.dox.h)
endforeach()

set(module_name "imagemagick")

set(QMOD
    qlib/ImageMagickDataProvider
)

set(QORE_DOX_TMPL_SRC
  docs/mainpage.dox.tmpl
)

add_library(${module_name} MODULE ${CPP_SRC} ${QPP_SOURCES})

include_directories(${CMAKE_SOURCE_DIR}/src)
target_include_directories(${module_name} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    ${MAGICKWAND_INCLUDE_DIRS}
)
target_compile_options(${module_name} PRIVATE ${MAGICKWAND_CFLAGS_OTHER})
if (HAVE_MAGICK_REDUCE_NOISE_IMAGE)
    target_compile_definitions(${module_name} PRIVATE HAVE_MAGICK_REDUCE_NOISE_IMAGE)
endif()

add_custom_target(QORE_INC_FILES DEPENDS ${QORE_INC_SRC})
add_dependencies(${module_name} QORE_INC_FILES)

if (MAGICKWAND_LIBRARY_DIRS)
    target_link_directories(${module_name} PRIVATE ${MAGICKWAND_LIBRARY_DIRS})
endif()
target_link_libraries(${module_name} ${MAGICKWAND_LIBRARIES} ${QORE_LIBRARY})

set(MODULE_DOX_INPUT ${CMAKE_CURRENT_BINARY_DIR}/mainpage.dox ${QPP_DOX})
string(REPLACE ";" " " MODULE_DOX_INPUT "${MODULE_DOX_INPUT}")

if (DEFINED ENV{DOXYGEN_EXECUTABLE})
    set(DOXYGEN_EXECUTABLE $ENV{DOXYGEN_EXECUTABLE})
endif()

set(DOXYGEN_IMAGE_PATH "${CMAKE_SOURCE_DIR}/docs/images" CACHE STRING "Doxygen image path" FORCE)
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/images")
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/doxygen/qlib/ImageMagickDataProvider")
file(WRITE
    "${CMAKE_BINARY_DIR}/doxygen/qlib/ImageMagickDataProvider/imagemagick-logo.svg.dox.h"
    "/** @file imagemagick-logo.svg.dox.h\n    @brief ImageMagick DataProvider logo file.\n*/\n"
)
set(MODULE_DOX_INPUT ${CMAKE_BINARY_DIR})
qore_external_binary_module(${module_name} ${PROJECT_VERSION})

qore_dist(${PROJECT_VERSION})

qore_user_modules("${QMOD}")
qore_external_user_module("qlib/ImageMagickDataProvider" "")

qore_config_info()

if (DOXYGEN_FOUND)
    qore_wrap_dox(QORE_DOX_SRC ${QORE_DOX_TMPL_SRC})
    add_custom_target(QORE_MOD_DOX_FILES DEPENDS ${QORE_DOX_SRC})
    add_dependencies(docs-module QORE_MOD_DOX_FILES)
endif()
