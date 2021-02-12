text = input("Text: ")
# sets letters, words and sentences to zero
l = 0
w = 1
s = 0
# goes through the string char by char
for i in text:
    i = ord(i)
    if (i > 64 and i < 91) or (i > 92 and i < 123):
        l += 1
    if i == 32:
        w += 1
    if i == 33 or i == 46 or i == 63:
        s += 1

# formula to calculate grade    
grade = round(0.0588 * (l * 100) / w - 0.296 * (s * 100) / w - 15.8)

if grade < 1:
    print("Before Grade 1")
elif grade > 16:
    print("Grade 16+")
else:
    print(f"Grade {grade}")