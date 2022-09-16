from pathlib import Path
from parse_header import parseHeader

import glob

root_dir = "input"
for filepath in glob.iglob(root_dir + '/**/*.h', recursive=True):
    classes = parseHeader(filepath)

    class_template = "auto clazz = state.new_usertype<{0}>(\"{0}\");";
    class_template_with_constr = "sol::usertype<{0}> clazz = state.new_usertype<player>(\"{0}\", sol::constructors<{}>());";


    item_template = "clazz[\"{1}\"] = &{0}::{1};"

    out_file_template = """#include <sol.hpp>
    
void {0}LuaWrap(sol::state& state) {{
    {1}
}}"""

    out_list = [];
    for clazz in classes:
        out_list.append(class_template.format(clazz["name"]));
        for field in clazz["fields"]:
            out_list.append(item_template.format(clazz["name"], field["name"]))
        for method in clazz["methods"]:
            out_list.append(item_template.format(clazz["name"], method["name"]))

        out_file_code = out_file_template.format(clazz["name"], '\n\t'.join(out_list))
        out_file_code += "\nextern void {0}LuaWrap(sol::state& state);".format(clazz["name"])

        with open("out/{}.cpp".format(clazz["name"]), "w") as out_file:
            out_file.write(out_file_code);
