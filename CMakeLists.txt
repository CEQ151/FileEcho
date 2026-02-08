cmake_minimum_required(VERSION 3.15)
project(FileEcho LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 强制定义 Windows 10 版本，解决 httplib 兼容性
if(WIN32)
    add_compile_definitions(_WIN32_WINNT=0x0A00)
endif()

# 包含头文件目录
include_directories(include src/backend)

# 查找所有源文件
file(GLOB_RECURSE SOURCES "src/backend/*.cpp")

# 【新增】定义资源文件变量
set(RESOURCE_FILES "")
if(WIN32)
    # 【修改】将资源文件指向 resources 目录
    set(RESOURCE_FILES "resources/FileEcho.rc")
endif()

# 创建可执行文件 (WIN32 隐藏黑窗口)
add_executable(FileEcho WIN32 ${SOURCES} ${RESOURCE_FILES})

# 【关键新增】将 resources 目录加入包含路径，确保 rc 编译器能找到 logo.ico
if(WIN32)
    target_include_directories(FileEcho PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/resources")
endif()

if(WIN32)
    # 链接所有必要的 Windows 系统库
    target_link_libraries(FileEcho PRIVATE 
        ole32 
        shell32 
        shlwapi 
        user32 
        advapi32
        ws2_32   # httplib 需要
        version  # 【关键修复】webview 需要这个库来获取版本信息
    )
    
    # 设置入口点
    if(MSVC)
        set_target_properties(FileEcho PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
    endif()
endif()

# 自动拷贝前端资源到 build 目录 (已改为单文件部署，此处不再需要)
# add_custom_command(TARGET FileEcho POST_BUILD
#      COMMAND ${CMAKE_COMMAND} -E copy_directory
#      "${CMAKE_SOURCE_DIR}/src/frontend"
#      "${CMAKE_BINARY_DIR}/frontend"
#      COMMENT "Copying frontend resources to build directory..."
# )