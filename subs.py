from selenium import webdriver
from time import sleep
import pandas as pd


class Bot:
    def __init__(self):
        self.driver = webdriver.Firefox()
        self.navigate()
        self.count = 0

    def navigate(self):
        df = pd.read_csv('all_filmsss.csv', sep='\t', comment='#')
        all_value = df.original_name_and_year
        for item in all_value:
            new_item = item[:-16]
            self.driver.get('http://subs.com.ru/index.php?e=search&sq=&rs%5Bsetlimit%5D=0&rs%5Brfrom%5D%5Bday%5D=7&rs%5Brfrom%5D%5Bmonth%5D=6&rs%5Brfrom%5D%5Byear%5D=2020&rs%5Brto%5D%5Bday%5D=7&rs%5Brto%5D%5Bmonth%5D=6&rs%5Brto%5D%5Byear%5D=2021&rs%5Bsetuser%5D=&rs%5Bpagsubcat%5D=0&rs%5Bpagsubcat%5D=1&rs%5Bpagtitle%5D=0&rs%5Bpagtitle%5D=1&rs%5Bpagdesc%5D=0&rs%5Bpagtext%5D=0&rs%5Bpagfile%5D=0&rs%5Bpagfile%5D=1&rs%5Bpagsort%5D=title&rs%5Bpagsort2%5D=ASC&rs%5Bfrmsubcat%5D=0&rs%5Bfrmtitle%5D=0&rs%5Bfrmtitle%5D=1&rs%5Bfrmtext%5D=0&rs%5Bfrmtext%5D=1&rs%5Bfrmreply%5D=0&rs%5Bfrmsort%5D=title&rs%5Bfrmsort2%5D=ASC')
            search = self.driver.find_element_by_xpath("//input[@name='sq']")
            search.send_keys(new_item)
            button = self.driver.find_element_by_xpath("//input[@value='искать']")
            button.click()
            #sleep(2)
            try:
                needed_film = self.driver.find_element_by_link_text(new_item)
                needed_film.click()
                download_sub = self.driver.find_element_by_partial_link_text("Субтитры к ")
                download_sub.click()
                self.count += 1
                print(self.count)
            except:
                print('No such film')


def main():
    b = Bot()


if __name__ == "__main__":
    main()
