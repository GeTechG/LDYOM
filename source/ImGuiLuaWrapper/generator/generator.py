from pathlib import Path

import enums_generator
import structs_and_definitions

enums_generator.generate()

lua_declarations = ["---@meta"]
lua_declarations += enums_generator.generate_lua()

Path(f"./output/lua").mkdir(parents=True, exist_ok=True)
with open(f"./output/lua/enums.lua", 'w') as f:
    f.write("\n".join(lua_declarations))

teal_declarations = []
teal_declarations += enums_generator.generate_teal()

Path(f"./output/teal").mkdir(parents=True, exist_ok=True)
with open(f"./output/teal/enums.d.tl", 'w') as f:
    f.write("\n".join(teal_declarations))

structs_and_definitions.generate()

lua_declarations = ["""---@diagnostic disable: duplicate-index
---@meta"""]
lua_declarations += structs_and_definitions.generate_lua()

Path(f"./output/lua").mkdir(parents=True, exist_ok=True)
with open(f"./output/lua/structs.lua", 'w') as f:
    f.write("\n".join(lua_declarations))

teal_declarations = []
teal_declarations += structs_and_definitions.generate_teal()

Path(f"./output/teal").mkdir(parents=True, exist_ok=True)
with open(f"./output/teal/struct.d.tl", 'w') as f:
    f.write("\n".join(teal_declarations))
