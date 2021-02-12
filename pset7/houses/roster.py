from cs50 import SQL
from sys import argv, exit

if len(argv) != 2:
    print("Usage: python roster.py 'House name'")
    exit(0)
    
db = SQL("sqlite:///students.db")

list = db.execute("SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last, first", argv[1])

for row in list:
    if row['middle'] == None:
        middlename = ""
    else:
        middlename = row['middle'] + " "
    print(f"{row['first']} {middlename}{row['last']}, born {row['birth']}")