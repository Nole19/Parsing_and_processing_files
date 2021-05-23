import requests
from bs4 import BeautifulSoup
import csv

URL = 'https://www.kinopoisk.ru/lists/top250/'
HEADERS = {"User-Agent": "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:88.0) Gecko/20100101 Firefox/88.0",
           "accept": "*/*"}
HOST = "https://www.kinopoisk.ru"
FILE = "films.csv"


def get_html(url, params=None):
    r = requests.get(url, headers=HEADERS, params=params)
    return r


def counter_pages(html):
    soup = BeautifulSoup(html, 'html.parser')
    paginator = soup.find_all('a', class_="paginator__page-number")
    if paginator:
        return int(paginator[-1].get_text())
    else:
        return 6


def get_content(html):
    soup = BeautifulSoup(html, 'html.parser')
    items = soup.find_all('div', class_='selection-film-item-meta')

    films = []
    for item in items:
        films.append({
            "title": item.find('p', class_="selection-film-item-meta__name").get_text(strip=True),
            "link": HOST + item.find('a', class_="selection-film-item-meta__link").get("href"),
            "original_name_and_year": item.find('p', class_='selection-film-item-meta__original-name').get_text().replace(', The', ''),
            "country and genre": item.find('p', class_="selection-film-item-meta__meta-additional").get_text(),
        })
    return films


def save_file(items, path):
    with open(path, 'w', newline='') as file:
        writer = csv.writer(file, delimiter=';')
        writer.writerow(["title", "link", "original_name_and_year", "Country and genre"])
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
        print(films)

    else:
        print("Error")


parse()
