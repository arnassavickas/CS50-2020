from sys import argv, exit
from cs50 import SQL
import csv

if len(argv) != 2:
    print("Usage: python import.py characters.csv")
    exit(0)

db = SQL("sqlite:///students.db")

characters = open(argv[1], "r")

reader = csv.DictReader(characters)

for row in reader:
    full = row['name'].split()
    if len(full) == 2:
        first = full[0]
        middle = None
        last = full[1]
    else:
        first = full[0]
        middle = full[1]
        last = full[2]
    house = row['house']
    birth = row['birth']
    db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)", first, middle, last, house, birth)