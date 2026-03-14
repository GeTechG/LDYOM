# EmbedFile.cmake
# Usage: cmake -DINPUT_FILE=<path> -DOUTPUT_FILE=<path> -DVAR_NAME=<name> -P EmbedFile.cmake
#
# Reads INPUT_FILE and writes a header with its content as a constexpr char array.
# The content is stored as individual byte values to avoid any string escaping issues.

file(READ "${INPUT_FILE}" content HEX)

# Split hex string into comma-separated byte values: "41424344" -> "0x41,0x42,0x43,0x44"
string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," bytes "${content}")
# Remove trailing comma
string(REGEX REPLACE ",$" "" bytes "${bytes}")

file(WRITE "${OUTPUT_FILE}" "// Auto-generated from ${INPUT_FILE} — do not edit manually.\n")
file(APPEND "${OUTPUT_FILE}" "#pragma once\n\n")
file(APPEND "${OUTPUT_FILE}" "namespace embedded {\n")
file(APPEND "${OUTPUT_FILE}" "    inline constexpr char ${VAR_NAME}[] = {\n        ${bytes}, 0x00\n    };\n")
file(APPEND "${OUTPUT_FILE}" "} // namespace embedded\n")
