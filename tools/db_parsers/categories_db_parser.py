from distutils.filelist import translate_pattern
import re
import sqlite3
import bs4;
from bs4 import BeautifulSoup;
from aiohttp import ClientSession
import asyncio;
import re;
import codecs;

print(sqlite3.sqlite_version);

conn_db = sqlite3.connect('./models.sqlite3');
cur = conn_db.cursor();

cur.execute("DROP TABLE IF EXISTS categories")
cur.execute("DROP TABLE IF EXISTS category_to_models")
conn_db.commit();
cur.execute("""
CREATE TABLE categories (
	id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
	category_name TEXT NOT NULL,
	parent_id INTEGER DEFAULT -1
);
""");
conn_db.commit();
cur.executescript("""
CREATE TABLE category_to_models (
	category_id INTEGER NOT NULL,
	model_id INTEGER NOT NULL,
	CONSTRAINT category_to_models_FK FOREIGN KEY (category_id) REFERENCES categories(id) ON DELETE CASCADE,
	CONSTRAINT category_to_models_FK_1 FOREIGN KEY (model_id) REFERENCES models(model_id) ON DELETE CASCADE
);

CREATE UNIQUE INDEX category_to_models_category_id_IDX ON category_to_models (category_id,model_id);
""");
conn_db.commit();

base_url = "https://dev.prineside.com";
start_url = "/en/gtasa_samp_model_id/"
other_url = "/{}/gtasa_samp_model_id/"

other_ln = ["ru"];

async def parse_page(session, url, category_id: int, page: int):
    async with session.get(base_url + url + "?page={}".format(page), timeout=99999) as response:
        if response.status == 200:
                html = await response.text();
                soup = BeautifulSoup(html, 'lxml');
                models_ids = list(map(lambda tag: (category_id, int(tag['data-model'])), soup.select("a.mp-item")));
                cur.executemany("INSERT INTO category_to_models VALUES(?, ?);", models_ids);
                conn_db.commit();


async def parse_category(session, category_tag: bs4.Tag, curr: int, other_lang: dict, parent_id: int = -1):
    category_name = category_tag.div.a.get_text();

    translate_name = "c_" + category_tag.div.a['href'].split("/")[-2].lower().replace('-', '_');
    my_file = open("categories_en.toml", "a")
    my_file.write("{} = \"{}\"\n".format(translate_name, category_name));
    my_file.close();
    category_name = "object_categories." + translate_name;
    print(category_name);

    for lang in other_ln:
        other_file = codecs.open("categories_{}.toml".format(lang), "a", "utf-8");
        other_file.write("{} = \"{}\"\n".format(translate_name, other_lang[lang][curr].div.a.get_text()));
        other_file.close();

    cur.execute("INSERT INTO categories (category_name, parent_id) VALUES(?, ?) RETURNING *;", (category_name, parent_id));
    result = cur.fetchone();
    conn_db.commit();

    dict_other_ln = {};
    for lang in other_ln:
        dict_other_ln[lang] = other_lang[lang][curr].find_all("div", recursive=False, attrs={'class': 'mp-index-category'});
    categories_list = category_tag.find_all("div", recursive=False, attrs={'class': 'mp-index-category'});
    if (len(categories_list) == 0):
        async with session.get(base_url + category_tag.div.a['href'], timeout=99999) as response:
            if response.status == 200:
                html = await response.text();
                soup = BeautifulSoup(html, 'lxml');
                count_pages = int(soup.find("div", attrs={'class': 'dt-paginator'}).find_all("a")[-2].get_text());
                for page in range(1, count_pages + 1):
                    await parse_page(session, category_tag.div.a['href'], result[0], page);

    for i in range(len(categories_list)):
        await parse_category(session, categories_list[i], i, dict_other_ln, result[0]);

async def parse_categories_name_other(session, lang):
    async with session.get(base_url + other_url.format(lang), timeout=99999) as response:
        if response.status == 200:
            html = await response.text();
            soup = BeautifulSoup(html, 'lxml');
            tag_categies = soup.select_one("#mp-index-categories");
            return tag_categies.find_all("div", recursive=False, attrs={'class': 'mp-index-category'});

async def parse_catefories():
    async with ClientSession() as session:
        async with session.get(base_url + start_url, timeout=99999) as response:
            if response.status == 200:
                html = await response.text();
                soup = BeautifulSoup(html, 'lxml');
                tag_categies = soup.select_one("#mp-index-categories");
                categories_list = tag_categies.find_all("div", recursive=False, attrs={'class': 'mp-index-category'});
                dict_other_ln = {};
                for lang in other_ln:
                    dict_other_ln[lang] = await parse_categories_name_other(session, lang);
                for i in range(len(categories_list)):
                    await parse_category(session, categories_list[i], i, dict_other_ln);


loop = asyncio.get_event_loop()
loop.run_until_complete(parse_catefories())