# Ruany Maia
# 21 Apr 2020
# Problem set 6 - Credit

from cs50 import get_string


def main():
    # Asks for user input
    card_number = get_string("Number: ")

    if is_amex(card_number):
        print("AMEX")

    elif is_visa(card_number):
        print("VISA")

    elif is_mastercard(card_number):
        print("MASTERCARD")

    else:
        print("INVALID")


def is_amex(n):
    amex_id = ["34", "37"]

    if (len(n) == 15) and (n[:2] in amex_id):
        return check_sum(n)
    else:
        return False


def is_visa(n):
    visa_id = ["4"]

    if (len(n) == 13) or (len(n) == 16) and (n[:1] in visa_id):
        return check_sum(n)
    else:
        return False


def is_mastercard(n):
    mastercard_id = ["51", "52", "53", "54", "55"]

    if (len(n) == 16) and (n[:2] in mastercard_id):
        return check_sum(n)
    else:
        return False


def check_sum(n):
    # This function applies Luhn's Algorithm through the digits

    # Initializes the variable that checks if number is legit
    checksum = 0

    # Step 1: multiplies every other digit by 2, starting with the number’s second-to-last digit
    # Reverses the given card number and converts it into a list
    n_reversed = list(reversed(n))

    # Creates a list with the numbers that should be multiplied by 2
    mult_list_1 = n_reversed[1::2]
    mult_list_2 = []

    # Multiplies each number from mult_list by 2
    for digit in mult_list_1:
        mult_digit = int(digit) * 2
        mult_digit_str = str(mult_digit)
        # Sums the digits of numbers multiplied by 2 on checksum
        for digit in mult_digit_str:
            mult_list_2.append(digit)

    for digit in mult_list_2:
        checksum += int(digit)

    # Step 2: sum the digits that weren't multiplied by 2
    # Creates a list with the numbers that should be summed up
    sum_list = n_reversed[::2]

    # Sums each number from sum_list on checksum
    for digit in sum_list:
        checksum += int(digit)

    if (checksum % 10 == 0):
        return True
    else:
        return False


main()