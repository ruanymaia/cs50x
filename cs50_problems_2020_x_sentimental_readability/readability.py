# Ruany Maia
# 23 Apr 2020
# Problem set 6 - Readability

# This program computes the approximate grade level needed to comprehend some text using Coleman-Liau index

from cs50 import get_string

# Asks for user input
text_input = get_string("Text: ")

# https://stackoverflow.com/questions/24878174/how-to-count-digits-letters-spaces-for-a-string-in-python
# Counts the number of letters in the text
text_letters = sum(c.isalpha() for c in text_input)
# Counts the number of spaces in the text + 1 (each space is a word)
text_words = sum(c.isspace() for c in text_input) + 1

# https://www.geeksforgeeks.org/python-string-count/
# Counts the number of ".", "!" and "?" in the text and considers it as the end of a sentence
text_sentences = text_input.count(".") + text_input.count("!") + text_input.count("?")

# Calculates the Coleman-Liau index
L = (text_letters * 100) / text_words       # average number of letters per 100 words
S = (text_sentences * 100) / text_words     # average number of sentences per 100 words
index = round(0.0588 * L - 0.296 * S - 15.8)

# print(f"{text_letters} letter(s)")
# print(f"{text_words} word(s)")
# print(f"{text_sentences} sentence(s)")

# Prints the grade level computed by the Coleman-Liau formula, rounded to the nearest integer.
if (index < 1):
    print("Before Grade 1")
elif (index >= 16):
    print("Grade 16+")
else:
    print(f"Grade {index}")