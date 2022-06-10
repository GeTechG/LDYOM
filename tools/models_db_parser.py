import sqlite3
from bs4 import BeautifulSoup;
from aiohttp import ClientSession
import asyncio;
import re;

conn_db = sqlite3.connect('./models.sqlite3');
cur = conn_db.cursor();
conn_db_icons = sqlite3.connect('./icons_models.sqlite3');
cur_icons = conn_db_icons.cursor();

base_url = "https://dev.prineside.com";
start_url = "/en/gtasa_samp_model_id/customsearch/?c[0]=1&s=id-asc&bc=-1&bb=-1&bt=-1&ba=-1&page={}"

cur.execute("DROP TABLE IF EXISTS models");
conn_db.commit();
cur.execute("""
CREATE TABLE models (
	model_id INTEGER PRIMARY KEY NOT NULL,
	title_name TEXT,
	model_name TEXT,
	has_collision INTEGER NOT NULL,
	is_destroy_by_hit INTEGER NOT NULL,
	see_by_time INTEGER NOT NULL
, icon_url TEXT);
""");
conn_db.commit();

# cur_icons.execute("DROP TABLE IF EXISTS icons");
# conn_db_icons.commit();
# cur_icons.execute("""
# CREATE TABLE icons (
# 	model_id INTEGER NOT NULL,
# 	icon BLOB NOT NULL,
# 	CONSTRAINT icons_PK PRIMARY KEY (model_id)
# );
# """);
# conn_db_icons.commit();

async def download_icon(session, url, model_id):
    try:
        async with session.get(url, timeout=99999) as response:
            if response.status == 200:
                icon = await response.read();
                cur_icons.execute("INSERT INTO icons VALUES(?, ?);", (model_id, icon));
                conn_db_icons.commit();
    except Exception as e:
        print("error icon - reply, " + str(model_id));
        await asyncio.sleep(1);
        await download_icon(session, url, model_id);



def extract_info_input(row) -> str:
    return row.select_one("td:nth-child(2n)").input['value'];

def extract_info(rows, name) -> str:
    for row in rows.find_all("td"):
        if row.get_text().strip() == name:
            return row.parent.select_one("td:nth-child(2n)").get_text().strip();

async def parse_model_page(session, url: str, title_name: str, model_id: int, model_name: str, icon_url: str):
    async with session.get(base_url + url, timeout=99999) as response:
        if response.status == 200:
            html = await response.text();
            try:
                soup = BeautifulSoup(html, 'lxml');
                table = soup.find('table', id="mp-model-info");

                use_collision: int = extract_info(table, "Has collision").find('Yes') != -1 if 1 else 0;
                destroy_by_hit: int = extract_info(table, "Breaks on hit").find('Yes') != -1 if 1 else 0;
                see_by_time: int = extract_info(table, "Visible by time").find('Yes') != -1 if 1 else 0;
                model_data = (model_id, title_name, model_name, use_collision, destroy_by_hit, see_by_time, icon_url);
                return model_data;
            except Exception as e:
                print(e);
                print(base_url + url);
                return (model_id, title_name, model_name, 1, 0, 0);
        

async def parse_page(session, page):
    async with session.get(base_url + start_url.format(page), timeout=99999) as response:
        if response.status == 200:
            try:
                html = await response.text();
                soup = BeautifulSoup(html, 'lxml');
                tags_a = soup.select("a.mp-item");
            except Exception as e:
                print(base_url + start_url.format(page));

            tasks = []
            models_list = [];
            for tag in tags_a:
                url = str(tag['href']).replace("+", "_").replace("(", "").replace(")", "");
                icon_url = tag.div.img['src'];
                # asyncio.create_task(download_icon(session, icon_url, int(tag['data-model'])));
                tasks.append(asyncio.create_task(parse_model_page(session, url, tag['data-title'], int(tag['data-model']), tag['data-name'], icon_url)));

            for task in tasks:
                models_list.append(await task);
            
            cur.executemany("INSERT INTO models VALUES(?, ?, ?, ?, ?, ?, ?);", models_list);
            conn_db.commit();
            print(page);

async def get_models():
    tasks = [];
    async with ClientSession() as session:
        for i in range(1, 190):
            tasks.append(asyncio.ensure_future(parse_page(session, i)));
        await asyncio.gather(*tasks)

loop = asyncio.get_event_loop()
loop.run_until_complete(get_models())