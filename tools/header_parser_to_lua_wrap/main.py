import json
import subprocess

classes = dict()

# python -m cxxheaderparser --mode=json myheader.h
process = subprocess.run(["python", "-m", "cxxheaderparser", "--mode=json", "input/Train.h"],capture_output=True)
json_data = json.loads(process.stdout)

segments = json_data["namespace"]["classes"][0]["class_decl"]["typename"]["segments"];
segments = list(map(lambda x: x["name"], segments))
class_name = "::".join(segments)

methods_ = json_data["namespace"]["classes"][0]["methods"]
methods_ = list(filter(lambda x: x["access"] == "public", methods_))
methods_ = list(filter(lambda x: not x["constructor"], methods_))
methods_ = list(filter(lambda x: not x["destructor"], methods_))
methods_ = list(filter(lambda x: x["operator"] is None, methods_))
for method in methods_:
    method["name"] = "::".join(list(map(lambda x: x["name"], method["name"]["segments"])))
    if "typename" in method["return_type"]:
        method["return_type"] = "::".join(list(map(lambda x: x["name"], method["return_type"]["typename"]["segments"])))
    elif "ref_to" in method["return_type"]:
        method["return_type"] = "::".join(list(map(lambda x: x["name"], method["return_type"]["ref_to"]["typename"]["segments"]))) + "&"
    elif "ptr_to" in method["return_type"]:
        method["return_type"] = "::".join(list(map(lambda x: x["name"], method["return_type"]["ptr_to"]["typename"]["segments"]))) + "*"

classes[class_name] = methods_;

with open("output/Train.json", "w") as f:
    json.dump(classes, f, indent=4)
