import os

# Configuration
# Mapping: Variable Name -> Source File
files = {
    "index_html": "src/frontend/index.html",
    "script_js": "src/frontend/script.js",
    "style_css": "src/frontend/style.css",
    "ai_addon_js": "src/frontend/ai_addon.js",
    "ai_addon_css": "src/frontend/ai_addon.css",
    "logo_ico": "resources/logo.ico"
}

# Output files
# The user requested src/backend/assets.hpp
# The existing code requires include/FileEcho/frontend_resources.hpp
# We generate BOTH to satisfy requirements and ensure the build works without code changes.
output_files = [
    "src/backend/assets.hpp",
    "include/FileEcho/frontend_resources.hpp"
]

def generate_header_content(files):
    content = []
    content.append("#pragma once")
    content.append("#include <string>")
    content.append("")

    for name, path in files.items():
        if not os.path.exists(path):
            print(f"Error: File not found: {path}")
            return None
            
        with open(path, "rb") as f:
            data = f.read()
            
        # Write array definition
        content.append(f"const unsigned char {name}[] = {{")
        
        # Hex formatting
        hex_lines = []
        current_line = []
        for i, byte in enumerate(data):
            current_line.append(f"0x{byte:02x}")
            if len(current_line) == 16:
                hex_lines.append(", ".join(current_line) + ",")
                current_line = []
        if current_line:
            hex_lines.append(", ".join(current_line))
            
        content.extend(hex_lines)
        content.append("};")
        content.append(f"const size_t {name}_len = {len(data)};")
        content.append("")

    # Add GetMimeType function (required by existing code compatibility)
    content.append("inline std::string GetMimeType(const std::string& path) {")
    content.append('    if (path.find(".html") != std::string::npos) return "text/html";')
    content.append('    if (path.find(".js") != std::string::npos) return "application/javascript";')
    content.append('    if (path.find(".css") != std::string::npos) return "text/css";')
    content.append('    if (path.find(".ico") != std::string::npos) return "image/x-icon";')
    content.append('    return "text/plain";')
    content.append("}")
    content.append("")
    
    return "\n".join(content)

def main():
    print("Generating asset headers...")
    header_content = generate_header_content(files)
    
    if header_content is None:
        exit(1)

    for output_file in output_files:
        # Ensure directory exists
        os.makedirs(os.path.dirname(output_file), exist_ok=True)
        
        with open(output_file, "w", encoding="utf-8") as f:
            f.write(header_content)
        
        print(f"Generated: {output_file}")

if __name__ == "__main__":
    main()
