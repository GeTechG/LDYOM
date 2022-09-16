from email.policy import strict
from pathlib import Path
from parse_header import parseHeader

import glob

file_template = """---@meta
{}
"""

class_template = """---@class {0}
{0} = {{
    {1}
}}
"""

function_template = """---{0}({1}) -> {2}
    ---@return {2}
    [\"{0}\"] = function({3}) end"""

field_template = """---{}
    [\"{}\"] = nil"""

root_dir = "input"
for filepath in glob.iglob(root_dir + '/**/*.h', recursive=True):
    classes = parseHeader(filepath)

    out = [];

    for clazz in classes:
        items = [];
        for field in clazz["fields"]:
            items.append(field_template.format(field["valueType"], field["name"]))
        for method in clazz["methods"]:
            params = [];

            if not method["static"]:
                params.append("self");

            if method["return_type"] == "char*":
                method["return_type"] = "string";
            
            if str(method["return_type"]).endswith("*"):
                method["return_type"] = method["return_type"][:-1]

            params_desc = [];
            for param in method["params"]:
                params.append(param["name"]);
                params_desc.append("{}: {}".format(param["name"], param["type"]));
            

            items.append(function_template.format(method["name"], ", ".join(params_desc), method["return_type"], ', '.join(params)))
        
        out.append(class_template.format(clazz["name"], ',\n\t'.join(items)));
    
    with open("out/{}.lua".format(Path(filepath).stem), "w") as out_file:
        out_file.write(file_template.format('\n'.join(out)));