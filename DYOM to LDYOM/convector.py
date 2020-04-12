import io
import binascii
import struct
import random

def totable(str,reverse = False):
    arr = []
    size = int(len(str)/2)
    for i in range(size):
        if reverse:
            arr.insert(0, str[i*2:i*2+2])
        else:
            arr.append(str[i*2:i*2+2])
    return arr

def tostr(arr):
    str = ""
    for i in arr:
        str += i
    return  str

def hex2float(s):
    bins = ''.join(chr(int(s[x:x+2], 16)) for x in range(0, len(s), 2))
    return struct.unpack('>f', bins)[0]

def copyArrRange(arr,start,end):
    new_arr = []
    for i in range(len(arr)):
        if i >= start and i < end:
            new_arr.append(arr[i])
    return new_arr

def reverseArr(arr):
    new_arr = []
    for i in arr:
        new_arr.insert(0, i)
    return  new_arr

path = "DYOM6.dat"  # input("Enter DYOMX.dat: ")
with open(path, 'rb') as f:
    hexdata = totable(f.read().hex())

#print(hexdata)
MPACK = {
    "Name": "Пак миссий #1",
    "Missions": [
        {
            "Prename": "",
            "Name": "",
            "Mission_Data": {
                "Particle": [],
                "Actors": [],
                "Cars": [],
                "Targets": [],
                "Audio": [],
                "Explosion": [],
                "Miss_data": {
                    "Riot": False,
                    "enex_markers": False,
                    "Player": {
                        "Pos": [884, -1221, 16],
                        "ModelId": 0,
                        "ped_type": True,
                        "Interior_id": 0,
                        "Weap_ammo": 0,
                        "Angle": 0,
                        "Weapon": 1
                    },
                    "Time": [0,0],
                    "Name": "Миссия #1",
                    "Weather": 1
                },
                "Group_relationships": [
                    "NULL",
                    [
                        3,
                        "NULL",
                        3,
                        3,
                        3,
                        3,
                        3,
                        3,
                        3
                    ],
                    [
                        5,
                        3,
                        "NULL",
                        5,
                        3,
                        3,
                        3,
                        3,
                        3
                    ],
                    [
                        5,
                        3,
                        5,
                        "NULL",
                        3,
                        3,
                        3,
                        3,
                        3
                    ],
                    [
                        3,
                        3,
                        3,
                        3,
                        "NULL",
                        3,
                        3,
                        3,
                        3
                      ],
                    [
                        3,
                        3,
                        3,
                        3,
                        3,
                        "NULL",
                        3,
                        3,
                        3
                      ],
                    [
                        3,
                        3,
                        3,
                        3,
                        3,
                        3,
                        "NULL",
                        3,
                        3
                      ],
                    [
                        3,
                        3,
                        3,
                        3,
                        3,
                        3,
                        3,
                        "NULL",
                        3
                      ],
                    [
                        3,
                        3,
                        3,
                        3,
                        3,
                        3,
                        3,
                        3,
                        "NULL"
                      ]
                ],
                "Pickup": [],
                "Objects": []
            }
        }
    ]
}

def findEnd(arr,start):
    endAdress = start
    while arr[endAdress] != "00":
        endAdress += 1
    return endAdress

offset = 0
MPACK["Missions"][0]["Name"] = bytes.fromhex(tostr(copyArrRange(hexdata, 4 + offset, findEnd(hexdata, 4+offset)))).decode('utf-8')
offset += findEnd(hexdata, 4+offset) - (4 + offset)
offset += findEnd(hexdata, 5+offset) - (5 + offset)
if tostr(copyArrRange(hexdata, 5+offset, 9+offset)) != "00000000":
    offset += findEnd(hexdata, 6+offset) - (6 + offset)
    offset += findEnd(hexdata, 8+offset) - (8 + offset)
    offset += findEnd(hexdata, 10+offset) - (10 + offset)

MPACK["Missions"][0]["Mission_Data"]["Miss_data"]["Time"] = [int(tostr(reverseArr(copyArrRange(hexdata, 49+offset, 50+offset))),16), 0]
MPACK["Missions"][0]["Mission_Data"]["Miss_data"]["Weather"] = int(tostr(reverseArr(copyArrRange(hexdata, 53+offset, 54+offset))),16)+1
MPACK["Missions"][0]["Mission_Data"]["Miss_data"]["Riot"] = bool(int(tostr(reverseArr(copyArrRange(hexdata, 65+offset, 66+offset))),16))


MPACK["Missions"][0]["Mission_Data"]["Miss_data"]["Player"]["Pos"][0] = struct.unpack('!f', bytes.fromhex(tostr(reverseArr(copyArrRange(hexdata, 69+offset, 73+offset)))))[0]
MPACK["Missions"][0]["Mission_Data"]["Miss_data"]["Player"]["Pos"][1] = struct.unpack('!f', bytes.fromhex(tostr(reverseArr(copyArrRange(hexdata, 73+offset, 77+offset)))))[0]
MPACK["Missions"][0]["Mission_Data"]["Miss_data"]["Player"]["Pos"][2] = struct.unpack('!f', bytes.fromhex(tostr(reverseArr(copyArrRange(hexdata, 77+offset, 81+offset)))))[0]
MPACK["Missions"][0]["Mission_Data"]["Miss_data"]["Player"]["Angle"] = struct.unpack('!f', bytes.fromhex(tostr(reverseArr(copyArrRange(hexdata, 81+offset, 85+offset)))))[0]
model_Player = int(tostr(reverseArr(copyArrRange(hexdata, 89+offset, 91+offset))),16)
Weapon_Player = int(tostr(reverseArr(copyArrRange(hexdata, 93+offset, 94+offset))),16)
if model_Player > 288:
    MPACK["Missions"][0]["Mission_Data"]["Miss_data"]["Player"]['ped_type'] = False
    model_Player = model_Player-1000+1
if Weapon_Player == 48:
    Weapon_Player = random.randint(0, 44)
MPACK["Missions"][0]["Mission_Data"]["Miss_data"]["Player"]['ModelId'] = model_Player
MPACK["Missions"][0]["Mission_Data"]["Miss_data"]["Player"]['Weapon'] = Weapon_Player+1-2
MPACK["Missions"][0]["Mission_Data"]["Miss_data"]["Player"]['Weap_ammo'] = int(tostr(reverseArr(copyArrRange(hexdata, 97+offset, 99+offset))), 16)
MPACK["Missions"][0]["Mission_Data"]["Miss_data"]["Player"]['Heath'] = int(tostr(reverseArr(copyArrRange(hexdata, 101+offset, 102+offset))), 16)

typeTargOffs = 2105
PosXTargOffs = 105
PosYTargOffs = 505
PosZTargOffs = 905
for t in range(int(tostr(reverseArr(copyArrRange(hexdata, 25+offset, 26+offset))), 16)):
    type = int(tostr(reverseArr(copyArrRange(hexdata, typeTargOffs+((t*4))+offset, typeTargOffs+((t*4))+offset+1))), 16)
    if type == 2:
        type = 1
    elif type == 1:
        type = 2
    MPACK["Missions"][0]["Mission_Data"]["Targets"].append({
        "Name": 'Target #'+str(t+1),
        "Type": type,
        "Target_Data": {}
    })
    xx = struct.unpack('!f', bytes.fromhex(tostr(reverseArr(copyArrRange(hexdata, PosXTargOffs + (t * 4) + offset, PosXTargOffs + (t * 4) + offset + 4)))))[0]
    xy = struct.unpack('!f', bytes.fromhex(tostr(reverseArr(copyArrRange(hexdata, PosYTargOffs + (t * 4) + offset, PosYTargOffs + (t * 4) + offset + 4)))))[0]
    xz = struct.unpack('!f', bytes.fromhex(tostr(reverseArr(copyArrRange(hexdata, PosZTargOffs + (t * 4) + offset, PosZTargOffs + (t * 4) + offset + 4)))))[0]
    if type == 1:
        MPACK["Missions"][0]["Mission_Data"]["Targets"][t]["Target_Data"] = {
            'Pos': {xx, xy, xz},
            'Radius': 2,
            'Text': '',
            'Text_time': 2,
            'Color_blip': 1
        }
    print(MPACK["Missions"][0]["Mission_Data"]["Targets"][t])


print(MPACK["Missions"][0])

input()
