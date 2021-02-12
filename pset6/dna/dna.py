import csv
from sys import argv, exit


db = open(argv[1], "r")
source = csv.reader(db)
head = next(source)
str_nr = len(head) - 1
# print(f"str_nr {str_nr}")
strs = {}
# search for AGATC AATG and TATC
file = open(argv[2], "r")
seq = file.read()
for i in range(0, str_nr, 1):
    strs[head[i+1]] = 0


# print(list(strs.keys())[0])
# print(list(strs.keys())[1])
# print(list(strs.keys())[2])
# print(len(list(strs.keys())[0]))

str_count = 0
while str_count < str_nr:
    sequences = 0
    max_sequences = 0
    i = 0
    name = list(strs.keys())[str_count]
    str_length = len(name)
    # print(name)

    while i < (len(seq) - str_length - 1):
        if seq[i:i+str_length] == name:
            sequences += 1
            if sequences > max_sequences:
                max_sequences = sequences
            i += str_length - 1
        else:
            sequences = 0
        i += 1
    strs[name] = max_sequences
    str_count += 1
db.close()
# print(max_AGATC)
# print(max_AATG)
# print(max_TATC)
db = open(argv[1], "r")
reader = csv.DictReader(db)
for row in reader:
    str_count = 0
    check = False
    while str_count < str_nr:
        name = list(strs.keys())[str_count]
        # print(name)
        # print(row)
        # print(strs[name])
        if int(row[name]) == strs[name]:
            check = True
            str_count += 1
        else:
            check = False
            break
    if check == True:
        print(row["name"])
        exit(1)

print("No match")
db.close()
file.close()
