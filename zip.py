import os
import fnmatch
import zipfile


def zip_func():
    list_of_files = os.listdir('.')
    pattern = "*.zip"
    for entry in list_of_files:
        if fnmatch.fnmatch(entry, pattern):
            print(entry)
            zip_file = zipfile.ZipFile(f"/home/nole19/Nik_Pas/{entry}")
            zip_file.extractall("/home/nole19/Nik_Pas")


def rar_func():
    pass


def main():
    zip_func()


if __name__ == "__main__":
    main()
