import requests
from bs4 import BeautifulSoup

URL = 'https://www.kinopoisk.ru/lists/top250/'
HEADERS = {"User-Agent": "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:88.0) Gecko/20100101 Firefox/88.0",
           "accept": "*/*"}
HOST = "https://www.kinopoisk.ru"


def get_html(url, params=None):
    r = requests.get(url, headers=HEADERS, params=params)
    return r


def get_content(html):
    soup = BeautifulSoup(html, 'html.parser')
    items = soup.find_all('div', class_='selection-film-item-meta')

    films = []
    for item in items:
        films.append({
            "title": item.find('p', class_="selection-film-item-meta__name").get_text(strip=True),
            "link": HOST + item.find('a', class_="selection-film-item-meta__link").get("href"),
            "original_name_and_year": item.find('p', class_='selection-film-item-meta__original-name').get_text(),
            "country": item.find('span', class_="selection-film-item-meta__meta-additional-item").get_text(),
        })

    print(films)


def parse():
    html = get_html(URL)
    if html.status_code == 200:
        get_content(html.text)
    else:
        print("Error")


parse()
