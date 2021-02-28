f = open('parse.txt', 'r')
raw_text = f.read();
f.close;


strings = [""];
string_num = 0;
for char in raw_text:
	if char == '\n':
		string_num += 1;
		strings.append("");
	else:
		strings[string_num] += char;

temp = 't_imgui.set_function("{}", ImGui::{});'
temp2 = 't_imgui.set_function("{}", sol::resolve<{}>(ImGui::{}));'
from collections import Counter

params = [];

for str_i in range(len(strings)):
	strings[str_i] = strings[str_i][28:];
	strParams = strings[str_i][strings[str_i].find("(")+1:]
	strParams = strParams[:strParams.find(";")-1];
	print(strParams)
	strParams = strParams.replace(", ",",");
	intr = 0;
	startt = 0;
	rawArrStr = []
	for it in range(len(strParams)):
		if strParams[it] == "(":
			intr += 1;
		if strParams[it] == ")":
			intr -= 1;
		if strParams[it] == "," and intr == 0:
			rawArrStr.append(strParams[startt:it])
			startt = it+1;
	rawArrStr.append(strParams[startt:])
	for j in range(len(rawArrStr)):
		if rawArrStr[j].find("=") == -1:
			rawArrStr[j] = ' '.join(rawArrStr[j].split(" ")[:-1]);
		else:
			rawArrStr[j] = ' '.join(rawArrStr[j].split(" ")[:-3]);
	params.append(rawArrStr);
	find_br = strings[str_i].find("(");
	if find_br != -1:
		strings[str_i] = strings[str_i][:find_br];


countPowtor = Counter(strings);
for str_i in range(len(strings)):
	if countPowtor[strings[str_i]] > 1:
		strings[str_i] = temp2.format(strings[str_i],', '.join(params[str_i]),strings[str_i])
	else:
		strings[str_i] = temp.format(strings[str_i],strings[str_i])


parsed = '\n'.join(strings)

f = open('parse.txt', 'w')
f.write(parsed);
f.close();