#read input file
import sys
import codecs
import re
import string
pattern = "[0-9]+:[0-9]+:[0-9]+,[0-9]+ --> [0-9]+:[0-9]+:[0-9]+,[0-9]+"
pattern2 = "[0-9]+"
filename = sys.argv[1]
filename2 = sys.argv[2]
    lines = file.readlines()
new_lines = "" ;
for line in lines:
    if re.match(pattern,line):
        pass
    elif re.match(pattern2,line):
        pass
    else:
        line = re.sub('[.,;!?-]', '', line)
        line = re.sub('\W^\S', '', line)
        new_lines += line
#print(lines)
#write output file
with codecs.open('filename2', 'w', encoding = 'utf8') as file:
    file.write(new_lines.lower())
