# Ruany Maia
# 10 Apr 2020
# Problem Set 6 - Mario - feeling more comfortable version
# This program asks the user for an integer between 1 and 8 and prints a pyramid with the integer's height

from cs50 import get_int


def main():
    height = get_positive_int()
    spaces = height - 1
    bricks = 1

    for row in range(height):
        for space in range(spaces):
            print(" ", end="")
        for brick in range(bricks):
            print("#", end="")
        print("  ", end="")
        for brick in range(bricks):
            print("#", end="")
        print()
        if spaces == 0 and bricks == height:
            break
        else:
            spaces -= 1
            bricks += 1


def get_positive_int():
    while True:
        n = get_int("Height: ")
        if n > 0 and n <= 8:
            break
    return n


main()