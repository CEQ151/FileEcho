import os

files = {
    "index_html": "src/frontend/index.html",
    "script_js": "src/frontend/script.js",
    "style_css": "src/frontend/style.css"
}

output_file = "include/FileEcho/frontend_resources.hpp"

with open(output_file, "w", encoding="utf-8") as out:
    out.write("#pragma once\n")
    out.write("#include <string>\n\n")
    
    for name, path in files.items():
        with open(path, "rb") as f:
            content = f.read()
            
        out.write(f"const unsigned char {name}[] = {{\n")
        
        # Write bytes in hex format
        for i, byte in enumerate(content):
            out.write(f"0x{byte:02x}")
            if i < len(content) - 1:
                out.write(", ")
            if (i + 1) % 16 == 0:
                out.write("\n")
                
        out.write("\n};\n")
        out.write(f"const size_t {name}_len = {len(content)};\n\n")

    out.write("""
inline std::string GetMimeType(const std::string& path) {
    if (path.find(".html") != std::string::npos) return "text/html";
    if (path.find(".js") != std::string::npos) return "application/javascript";
    if (path.find(".css") != std::string::npos) return "text/css";
    return "text/plain";
}
""")

print(f"Generated {output_file}")
