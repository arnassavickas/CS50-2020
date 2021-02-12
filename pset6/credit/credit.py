import math
from sys import exit


def main():
    number = ask_number()
    number = [int(x) for x in str(number)]
    # print(number)
    sum1 = 0
    # goes through numbers in the list starting from second to last
    for i in range(len(number)-2, -1, -2):
        dig = number[i] * 2
        if dig >= 10:
            dig = 1 + dig % 10
        sum1 += dig
    
    sum2 = 0
    # goes through numbers from last
    for i in range(len(number)-1, -1, -2):
        sum2 += number[i]
    
    if (sum1 + sum2) % 10 == 0:
        if number[0] == 4:
            print("VISA")
        elif number[0] == 3 and (number[1] == 4 or number[1] == 7):
            print("AMEX")
        elif number[0] == 5 and (number[1] >= 1 and number[1] <= 5):
            print("MASTERCARD")
        else:
            print("INVALID")
    else:
        print("INVALID")
    

def ask_number():
    while True:
        # imputs number and calculates length
        try:
            number = int(input("Number: "))
            digits = int(math.log10(number))+1
        # if inputted not number, try again
        except ValueError:
            continue
        else:
            # if length is not ok, exit
            if number < 0 or digits < 12 or digits > 16:
                print("INVALID")
                exit(1)
            return number
            break


main()