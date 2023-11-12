import json
from itertools import batched
from dotenv import load_dotenv
from openai import OpenAI
import toml
import time
from os import walk

load_dotenv()

client = OpenAI()
instruction = """"You are an advanced artificial intelligence translator. In your request, you will receive a JSON 
object with the value being the text in {} for translation. This text is intended for the interface of a mod for 
the GTA SA game, focused on creating custom missions. Please consider this context when translating. Your response 
should include a JSON object with the same structure, but with the translated text as the value. The language of 
translation is {}."""

default_lang_name = 'English'


def translate(fields: dict, current_lang: str) -> dict:
    completion = client.chat.completions.create(
        model="gpt-3.5-turbo-16k",
        # response_format={"type": "json_object"},
        messages=[
            {"role": "system", "content": instruction.format(default_lang_name, current_lang)},
            {"role": "user", "content": json.dumps(fields)}
        ]
    )

    return json.loads(completion.choices[0].message.content)


langs = {}

folder_path = 'langs'
for root, dirs, files in walk(folder_path):
    for file in files:
        if file.endswith('.toml'):
            with open(f'{folder_path}/{file}', 'r', encoding='utf-8') as f:
                lang = toml.load(f)
                langs[file[:-5]] = lang

needs_langs = ['Russian', 'Portuguese', 'Polish']


def translate_langs():
    if default_lang_name not in langs:
        raise Exception('Default language not found')

    default_lang = langs[default_lang_name]

    for iter_lang in needs_langs:
        if iter_lang not in langs:
            langs[iter_lang] = {}

        if iter_lang != default_lang_name:
            print(f'Generating {iter_lang}...')
            i = 0
            for category in default_lang:
                if category not in langs[iter_lang]:
                    langs[iter_lang][category] = {}

                values: dict = default_lang[category]
                for bath_keys in batched(filter(lambda s: s not in langs[iter_lang][category], values.keys()), 50):
                    translated = translate({key: values[key] for key in bath_keys}, iter_lang)
                    print(translated)
                    langs[iter_lang][category].update(translated)
                    i += 1
                    if i == 3:
                        time.sleep(62)
                        i = 0


try:
    translate_langs()
except Exception as e:
    print('Error:', e)

for lang in langs:
    if lang != default_lang_name:
        with open(f'langs/{lang}.toml', 'w', encoding='utf-8') as f:
            toml.dump(langs[lang], f)
