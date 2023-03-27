# Parsing and processing files

## Table of Contents
* [General Info](#general-information)
* [Technologies Used](#technologies-used)
* [Installation](#installation)
* [Usage](#usage)
* [Features](#features)
* [Usage](#usage)
* [License](#license)



## General Information
- This parser can parse information from kinopoisk.ru and connect with subs from films to find popular film with help of subtitles



## Technologies Used
- Python 3.7.2 and higher
- Libraries: bs4, requests, csv, zipfile, fnmatch, selenium

## Installation


1. Clone the repository:

```bash
$ git clone https://github.com/Nole19/Parsing_and_processing_files.git
```
2. Install the required packages:
```bash
$ pip install -r requirements.txt
```

## Features
-It's working with name, year, link, genre
-After parsing all information saves in csv file
-Also it can create zip and unzip files
-It findes subs for different films with help of selenium
-And matches subs with films 
-After this we can find movies with help of subs

## Usage
- First you should start file parser.py to parse films to csv file
```bash
$ python parser.py --pages <number_of_pages> --csv <output_csv_file>
```
For example, to scrape the first 5 pages of Kinopoisk.ru, connect the movies with subtitles, and save the data to output.csv, run:
```bash
$ python parser.py --pages 5 --csv output.csv
```
The script will scrape the following information for each movie:
Title
Year
Genre
Director
Actors
Rating
Number of votes

The data will be saved to a CSV file with the specified filename.
Example:
```bash
$ python parser.py --pages 1 --csv movies.csv
Scraping page 1...
Connecting movies with subtitles...
Scraping complete. 50 movies scraped and saved to movies.csv. 30 subtitles downloaded.

```

Then we need to download subtitles for this films. For it need to run file subs.py:
```bash
$ python subs.py
```

Next step is unzip subs:
```bash
$ python zip.py
```

After this we need to clean all extra letters and digits from file:
```bash
$ python prepare_file.py
```
Now we created base with movie + subs

## License

[MIT](https://choosealicense.com/licenses/mit/)
