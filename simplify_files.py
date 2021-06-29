#read input file
import codecs
import re
import string
pattern = "[0-9]+:[0-9]+:[0-9]+,[0-9]+ --> [0-9]+:[0-9]+:[0-9]+,[0-9]+"
pattern2 = "[0-9]+"
with codecs.open('Mortal.srt', 'r', encoding = 'mbcs') as file:
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
with codecs.open('Mortaltttt.srt', 'w', encoding = 'utf8') as file:
    file.write(new_lines.lower())
