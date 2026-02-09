# 构建说明 (Build Instructions)

本项目已配置为将前端资源（HTML/JS/CSS）打包到可执行文件中，生成单一的 `.exe` 文件。

## 如何更新前端

如果你修改了 `src/frontend` 目录下的任何文件，请按照以下步骤更新程序：

1. **重新生成资源头文件**：
   运行项目根目录下的 Python 脚本：
   ```bash
   python generate_resources.py
   ```
   这会更新 `src/backend/frontend_resources.hpp` 文件。

2. **重新编译项目**：
   在 `build` 目录中运行：
   ```bash
   cmake --build build
   ```

3. **运行程序**：
   生成的独立可执行文件位于 `build/FileEcho.exe`。

## 注意事项

- `FileEcho.exe` 现在可以独立运行，不需要 `frontend` 文件夹。
- 如果添加了新的静态资源（如图片），需要修改 `generate_resources.py` 和 `src/backend/webserver.cpp` 来包含并服务这些新文件。
