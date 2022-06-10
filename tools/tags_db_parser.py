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

cur.execute("DROP TABLE IF EXISTS tags")
cur.execute("DROP TABLE IF EXISTS tag_to_models")
conn_db.commit();
cur.execute("""
CREATE TABLE tags (
	id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
	name TEXT NOT NULL
);
""");
conn_db.commit();
cur.executescript("""
CREATE TABLE tag_to_models (
	tag_id INTEGER NOT NULL,
	model_id INTEGER NOT NULL,
	CONSTRAINT tag_to_models_FK FOREIGN KEY (tag_id) REFERENCES tags(id) ON DELETE CASCADE,
	CONSTRAINT tag_to_models_FK_1 FOREIGN KEY (model_id) REFERENCES models(model_id) ON DELETE CASCADE
);

CREATE UNIQUE INDEX tag_to_models_tag_id_IDX ON tag_to_models (tag_id,model_id);
""");
conn_db.commit();

base_url = "https://dev.prineside.com";
start_url = "/en/gtasa_samp_model_id/tags/"
other_url = "/{}/gtasa_samp_model_id/tags/"

other_ln = ["ru"];

async def parse_page(session, url, tag_id: int, page: int):
    async with session.get(base_url + url + "?page={}".format(page), timeout=99999) as response:
        if response.status == 200:
                html = await response.text();
                soup = BeautifulSoup(html, 'lxml');
                models_ids = list(map(lambda tag: (tag_id, int(tag['data-model'])), soup.select("a.mp-item")));
                cur.executemany("INSERT INTO tag_to_models VALUES(?, ?);", models_ids);
                conn_db.commit();


async def parse_tag(session, tag: bs4.Tag, other_lang: dict, parent_id: int = -1):
    tag_name = tag.get_text().strip();
    
    translate_name = "t_" + tag['href'].split("/")[-2].lower().replace('-', '_');
    my_file = open("tags_en.toml", "a")
    my_file.write("{} = \"{}\"\n".format(translate_name, tag_name));
    my_file.close();

    for lang in other_ln:
        other_file = codecs.open("tags_{}.toml".format(lang), "a", "utf-8");
        other_a = other_lang[lang].find("a", attrs={'href':"/{}{}".format(lang, tag['href'][3:])});
        if other_a:
            other_file.write("{} = \"{}\"\n".format(translate_name, other_a.span.get_text().strip()));
        other_file.close();

    tag_name = "object_tags." + translate_name;
    print(tag_name);

    cur.execute("INSERT INTO tags (name) VALUES(?) RETURNING *;", (tag_name,));
    result = cur.fetchone();
    conn_db.commit();

    async with session.get(base_url + tag['href'], timeout=99999) as response:
        if response.status == 200:
            html = await response.text();
            soup = BeautifulSoup(html, 'lxml');
            try:
                count_pages = int(soup.find("div", attrs={'class': 'dt-paginator'}).find_all("a")[-2].get_text());
                for page in range(1, count_pages + 1):
                    await parse_page(session, tag['href'], result[0], page);
            except Exception as e:
                print(e);
                print(base_url + tag['href']);

async def parse_tags_name_other(session, lang):
    async with session.get(base_url + other_url.format(lang), timeout=99999) as response:
        if response.status == 200:
            html = await response.text();
            soup = BeautifulSoup(html, 'lxml');
            return soup;

async def parse_tags():
    async with ClientSession() as session:
        async with session.get(base_url + start_url, timeout=99999) as response:
            if response.status == 200:
                html = await response.text();
                soup = BeautifulSoup(html, 'lxml');
                dict_other_ln = {};
                for lang in other_ln:
                    dict_other_ln[lang] = await parse_tags_name_other(session, lang);
                tags_list = soup.find_all("a", attrs={"class": "mp-tag"});
                for tag in tags_list:
                    await parse_tag(session, tag, dict_other_ln);
                    pass


loop = asyncio.get_event_loop()
loop.run_until_complete(parse_tags())