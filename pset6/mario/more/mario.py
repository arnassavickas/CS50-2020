def main():
    height = get_int_1_8()
    print_bricks(height)
    

def get_int_1_8():
    while True:
        # asks to enter height
        try:
            height = int(input("Height: "))       
        # if not a number is entered, then error is catched here and returns to try
        except ValueError:
            continue
        else:
            # if input is not in the range of 1-8, then returns to try
            if height < 1 or height > 8:
                continue            
            return height
            break
        
        
def print_bricks(height):
    # just prints the bricks
    for x in range(height):
        print(" " * (height - x - 1), end="")
        print("#" * (x + 1), end="")
        print("  ", end="")
        print("#" * (x + 1), end="")
        print()
        

main()