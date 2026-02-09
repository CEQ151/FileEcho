cmake_minimum_required(VERSION 3.15)
project(FileEcho LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 强制定义 Windows 10 版本，解决 httplib 兼容性
if(WIN32)
    add_compile_definitions(_WIN32_WINNT=0x0A00)
endif()

# 包含头文件目录
include_directories(include)
include_directories(include/external)

# 查找所有源文件
file(GLOB SOURCES "src/backend/*.cpp")

# 【关键修复】定义资源文件变量
set(RESOURCE_FILES "")
if(WIN32)
    # 将资源文件加入列表
    set(RESOURCE_FILES "resources/FileEcho.rc")
endif()

# 查找 Python 解释器并执行资源生成脚本
find_package(Python3 COMPONENTS Interpreter REQUIRED)
execute_process(COMMAND ${Python3_EXECUTABLE} "${CMAKE_SOURCE_DIR}/generate_resources.py")

# 创建可执行文件 (WIN32 隐藏黑窗口)
add_executable(FileEcho WIN32 ${SOURCES} ${RESOURCE_FILES})

if(WIN32)
    # 链接所有必要的 Windows 系统库
    target_link_libraries(FileEcho PRIVATE 
        ole32
        uuid 
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

# 自动拷贝前端资源到 build 目录 (已嵌入到 exe 中，不再需要)
# add_custom_command(TARGET FileEcho POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     "${CMAKE_SOURCE_DIR}/src/frontend"
#     "${CMAKE_BINARY_DIR}/frontend"
#     COMMENT "Copying frontend resources to build directory..."
# )