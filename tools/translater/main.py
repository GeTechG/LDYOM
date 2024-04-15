import json
from itertools import batched
from dotenv import load_dotenv
import rtoml
import time
from os import walk
from g4f.client import Client

load_dotenv()

client = Client()
instruction = """"You are an advanced artificial intelligence translator. In your request, you will receive a JSON 
object with the value being the text in {} for translation. This text is intended for the interface of a mod for the 
GTA SA game (Lua Design Your Own Mission), focused on creating custom missions. Please consider this context when 
translating. Your response should include a JSON object with the same structure, but with the translated text as the 
value. The language of translation is {}."""

default_lang_name = 'Russian'


def translate(fields: dict, current_lang: str) -> dict:
    a = instruction.format(default_lang_name, current_lang)
    b = json.dumps(fields, ensure_ascii=False)
    completion = client.chat.completions.create(
        model="gpt-3.5-turbo",
        response_format={"type": "json_object"},
        messages=[
            {"role": "system", "content": a},
            {"role": "user", "content": b}
        ]
    )

    return json.loads(completion.choices[0].message.content)


langs = {}

folder_path = 'langs'
for root, dirs, files in walk(folder_path):
    for file in files:
        if file.endswith('.toml'):
            with open(f'{folder_path}/{file}', 'r', encoding='utf-8') as f:
                lang = rtoml.load(f)
                langs[file[:-5]] = lang

needs_langs = ['Portuguese', 'Polish', "English"]

usedTranslate = 0


def translate_langs():
    def recusive(source: dict, target: dict, lang_: str):
        global usedTranslate
        for_translate = dict()
        for category in source:
            if category not in target:
                for_translate[category] = source[category]
            elif isinstance(source[category], dict):
                recusive(source[category], target[category], lang_)
        if (len(for_translate) > 0):
            traslated = translate(for_translate, lang_)
            print(traslated)
            target.update(traslated)
            usedTranslate += 1
            if usedTranslate == 3:
                time.sleep(62)
                usedTranslate = 0
        

    if default_lang_name not in langs:
        raise Exception('Default language not found')

    default_lang = langs[default_lang_name]

    for iter_lang in needs_langs:
        if iter_lang not in langs:
            langs[iter_lang] = {}

        if iter_lang != default_lang_name:
            print(f'Generating {iter_lang}...')
            recusive(default_lang, langs[iter_lang], iter_lang)


try:
    translate_langs()
except Exception as e:
    print('Error:', e)

for lang in langs:
    if lang != default_lang_name:
        with open(f'langs/{lang}.toml', 'w', encoding='utf-8') as f:
            rtoml.dump(langs[lang], f)
