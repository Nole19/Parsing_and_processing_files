from selenium import webdriver
from time import sleep


class Bot:
    def __init__(self):
        self.driver = webdriver.Firefox()
        self.navigate()

    def navigate(self):
        self.driver.get('http://subs.com.ru/index.php?e=search&sq=&rs%5Bsetlimit%5D=0&rs%5Brfrom%5D%5Bday%5D=7&rs%5Brfrom%5D%5Bmonth%5D=6&rs%5Brfrom%5D%5Byear%5D=2020&rs%5Brto%5D%5Bday%5D=7&rs%5Brto%5D%5Bmonth%5D=6&rs%5Brto%5D%5Byear%5D=2021&rs%5Bsetuser%5D=&rs%5Bpagsubcat%5D=0&rs%5Bpagsubcat%5D=1&rs%5Bpagtitle%5D=0&rs%5Bpagtitle%5D=1&rs%5Bpagdesc%5D=0&rs%5Bpagtext%5D=0&rs%5Bpagfile%5D=0&rs%5Bpagfile%5D=1&rs%5Bpagsort%5D=title&rs%5Bpagsort2%5D=ASC&rs%5Bfrmsubcat%5D=0&rs%5Bfrmtitle%5D=0&rs%5Bfrmtitle%5D=1&rs%5Bfrmtext%5D=0&rs%5Bfrmtext%5D=1&rs%5Bfrmreply%5D=0&rs%5Bfrmsort%5D=title&rs%5Bfrmsort2%5D=ASC')
        value = 'se7en'
        search = self.driver.find_element_by_xpath("//input[@name='sq']")
        search.send_keys('Se7en')
        button = self.driver.find_element_by_xpath("//input[@value='искать']")
        button.click()
        sleep(2)
        needed_film = self.driver.find_element_by_link_text('Se7en')
        needed_film.click()


def main():
    b = Bot()


if __name__ == "__main__":
    main()
