#!/usr/bin/python3
import sys
import codecs
import re
import string
pattern = "[0-9]+:[0-9]+:[0-9]+,[0-9]+ --> [0-9]+:[0-9]+:[0-9]+,[0-9]+"
pattern2 = "[0-9]+"
filename = sys.argv[1]
filename2 = sys.argv[2]
with codecs.open(filename, 'r',encoding = 'cp1251') as file:
    lines = file.readlines()
new_lines = "" ;
for line in lines:
    if re.match(pattern,line):
        pass
    elif re.match(pattern2,line):
        pass
    elif line == "\r\n":
        pass
    else:
        line = re.sub('\r\n', ' ', line)
        line = re.sub('[.,;!?—\"\'-]', '', line)
        line = re.sub('\W^\S', '', line)
        #print(line)
        new_lines += line.lower()
#print(lines)
#write output file
with codecs.open(filename2, 'w', encoding = 'cp1251') as file:
    file.write(new_lines)
