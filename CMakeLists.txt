cmake_minimum_required(VERSION 3.10)

project(FileEcho LANGUAGES CXX RC)

# 1. Configuration
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

# 2. Include Directories
include_directories(include)
include_directories(include/external)
# Add src/backend to include path if needed for assets.hpp
include_directories(src/backend)

# 3. Source Files
set(SOURCES
    src/backend/main.cpp
    src/backend/webserver.cpp
    src/backend/filesystem.cpp
    src/backend/ai_handler.cpp
    src/backend/pdf_extractor.cpp
    src/backend/doc_extractor.cpp
    resources/FileEcho.rc
)

# Check for zlib (PDF text extraction support)
include(CheckLibraryExists)
check_library_exists(z inflate "" HAVE_ZLIB)

# 4. Executable
# Using WIN32 to ensure it is treated as a GUI application (linked with -mwindows automatically by CMake if WIN32 is present)
# However, if main() is used instead of WinMain(), MinGW handles it but MSVC needs /ENTRY:mainCRTStartup
if(MSVC)
    add_executable(FileEcho WIN32 ${SOURCES})
    set_target_properties(FileEcho PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
else()
    # For MinGW, we can just use add_executable and link with -mwindows manually if needed, or use WIN32
    # Using WIN32 with MinGW automatically adds -mwindows
    add_executable(FileEcho WIN32 ${SOURCES})
endif()

# 5. Libraries
# Standard Windows libraries required for WebView2, Socket, and Windows API
# - ws2_32: Winsock (httplib)
# - shlwapi, ole32, shell32, version, gdi32, user32, uuid: WebView2 / Windows API
target_link_libraries(FileEcho PRIVATE 
    ws2_32 
    shlwapi 
    ole32 
    shell32 
    version 
    gdi32 
    user32
    uuid
    comdlg32
    winhttp
)

if(HAVE_ZLIB)
    target_compile_definitions(FileEcho PRIVATE HAS_ZLIB)
    target_link_libraries(FileEcho PRIVATE z)
    message(STATUS "zlib found - PDF FlateDecode decompression enabled")
else()
    message(STATUS "zlib not found - PDF extraction limited to uncompressed streams")
endif()

# 6. Static linking & stripping (reduce AV false positives, eliminate DLL dependencies)
if(MINGW)
    target_link_options(FileEcho PRIVATE
        -static                # Statically link libstdc++, libgcc, libwinpthread into EXE
        -s                     # Strip all symbols (smaller binary, fewer AV triggers)
    )
    message(STATUS "MinGW: static linking + strip enabled")
endif()

# 7. Embed UAC manifest (via .rc file: "1 24 FileEcho.manifest")
