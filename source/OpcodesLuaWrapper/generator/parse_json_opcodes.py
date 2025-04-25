import json
import stringcase

def parseOpcodes(only_required: bool = False):
    classes: dict[str, list[object]] = {}
    data: any
    with open("sa.json", "r") as read_file:
        data = json.load(read_file)
    for extension in data["extensions"]:
        if only_required:
            if extension["name"] not in ["default", "CLEO"]:
                continue
        for command in extension["commands"]:
            if "platforms" in command and not "pc" in command["platforms"]:
                continue

            if "attrs" in command:
                if "is_nop" in command["attrs"]:
                    continue
                if "is_keyword" in command["attrs"]:
                    command["class"] = "Keywords"
                    command["member"] = stringcase.camelcase(str(command["name"]).lower())
            
            command["extension"] = extension["name"]
            if "member" in command:
                if command["member"] == "":
                    command["member"] = stringcase.camelcase(str(command["name"]).lower())
                if command["class"] == "":
                    command["class"] = "Other"
                if command["class"] in classes:
                    classes[command["class"]].append(command)
                else:
                    classes[command["class"]] = [command]
    return classes