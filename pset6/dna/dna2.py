import csv
from sys import argv, exit

# search for AGATC AATG and TATC
with open(argv[2], "r") as file:
    seq = file.read()
# AGATC,TTTTTTCT,AATG,TCTAG,GATA,TATC,GAAA,TCTG
    # AGATC
    AGATC = 0
    max_AGATC = 0
    i = 0
    while i < (len(seq)-4):
        if seq[i] == "A" and seq[i+1] == "G" and seq[i+2] == "A" and seq[i+3] == "T" and seq[i+4] == "C":
            AGATC += 1
            if AGATC > max_AGATC:
                max_AGATC = AGATC
            i += 4
        else:
            AGATC = 0
        i += 1
    # TTTTTTCT
    AGATC = 0
    max_AGATC = 0
    i = 0
    while i < (len(seq)-4):
        if seq[i] == "A" and seq[i+1] == "G" and seq[i+2] == "A" and seq[i+3] == "T" and seq[i+4] == "C":
            AGATC += 1
            if AGATC > max_AGATC:
                max_AGATC = AGATC
            i += 4
        else:
            AGATC = 0
        i += 1
    # AATG
    AATG = 0
    max_AATG = 0    
    i = 0
    while i < (len(seq)-3):
        if seq[i] == "A" and seq[i+1] == "A" and seq[i+2] == "T" and seq[i+3] == "G":
            AATG += 1
            if AATG > max_AATG:
                max_AATG = AATG
            i += 3
        else:
            AATG = 0
        i += 1
    # TATC
    TATC = 0
    max_TATC = 0    
    i = 0
    while i < (len(seq)-3):
        if seq[i] == "T" and seq[i+1] == "A" and seq[i+2] == "T" and seq[i+3] == "C":
            TATC += 1
            if TATC > max_TATC:
                max_TATC = TATC
            i += 3
        else:
            TATC = 0            
        i += 1

print(max_AGATC)
print(max_AATG)
print(max_TATC)

with open(argv[1], "r") as db:
    reader = csv.DictReader(db)
    for row in reader:
        if int(row["AGATC"]) == max_AGATC and int(row["AATG"]) == max_AATG and int(row["TATC"]) == max_TATC:
            print(row["name"])
            exit(1)
    print("No match")

