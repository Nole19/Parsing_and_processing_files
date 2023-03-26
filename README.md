# Parsing_and_processing_files

## Table of Contents
* [General Info](#general-information)
* [Technologies Used](#technologies-used)
* [Features](#features)
* [Usage](#usage)



## General Information
- This parser can parse information from kinopoisk.ru and connect with subs from films to find popular film with help of subtitles



## Technologies Used
- Python 3.7.2 and high
- Libraries: bs4, requests, csv, zipfile, fnmatch, selenium


## Features
-It's working with name, year, link, genre
-After parsing all information saves in csv file
-Also it can create zip and unzip files
-It findes subs for different films with help of selenium
-And matches subs with films 
-After this we can find movies with help of subs

## Usage
- First you should start file parser.py to parse films to csv file
- Then parse subs with using film name 
- The number of filmes is unlimited, so we can ad more and more 
- After this we can find movie with help of subtitles 
