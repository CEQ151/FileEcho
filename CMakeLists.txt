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

# 创建可执行文件 (WIN32 隐藏黑窗口)
add_executable(FileEcho WIN32 ${SOURCES})

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