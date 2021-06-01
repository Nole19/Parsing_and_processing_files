import requests
from bs4 import BeautifulSoup
import csv

URL = 'https://www.kinopoisk.ru/top/navigator/m_act[egenre]/20%2C12%2C1747%2C15%2C21%2C28%2C25%2C999%2C26%2C1751/m_act[num_vote]/2530/m_act[is_film]/on/m_act[is_mult]/on/order/rating/perpage/200/#results'
HEADERS = {"User-Agent": "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:88.0) Gecko/20100101 Firefox/88.0",
           "accept": "*/*"}
HOST = "https://www.kinopoisk.ru"
FILE = "film.csv"


def get_html(url, params=None):
    r = requests.get(url, headers=HEADERS, params=params)
    return r


def counter_pages(html):
    soup = BeautifulSoup(html, 'html.parser')
    paginator = soup.find_all('li', class_="arr")
    if paginator:
        return 41
    else:
        return 41


def get_content(html):
    soup = BeautifulSoup(html, 'html.parser')
    items = soup.find_all('div', class_='item _NO_HIGHLIGHT_')

    films = []
    for item in items:
        films.append({
            "title": item.find('div', class_="name").find('a').get_text(strip=True),
            "link": HOST + item.find('div', class_="name").find("a").get("href"),
            "original_name_and_year": item.find('div', class_='name').find('span').get_text(),
            "country and genre": item.find('span', class_="gray_text").get_text(),
        })
    return films


def save_file(items, path):
    with open(path, 'a', newline='') as file:
        writer = csv.writer(file, delimiter=';')
        writer.writerow(["title", "link", "original_name_and_year", "country and genre"])
        for item in items:
            writer.writerow([item["title"], item["link"], item["original_name_and_year"], item["country and genre"]])


def parse():
    html = get_html(URL)
    if html.status_code == 200:
        films = []
        pages_count = counter_pages(html.text)
        for page in range(1, pages_count + 1):
            print(f'Parsing of page {page} from {pages_count}...')
            html = get_html(URL, params={'page': page})
            films.extend(get_content(html.text))
        save_file(films, FILE)
        #print(f"We've got {len(films)} films")

    else:
        print("Error")


parse()
