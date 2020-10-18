# Ruany Maia
# 27 Apr 2020
# Problem set 6 - DNA

# This program identifies a person based on their DNA

import sys
import csv

if (len(sys.argv) != 3):
    print("Incorrect input. Please enter: dna.py valid_database.csv valid_sequence.txt")

else:
    database_path = sys.argv[1]
    sequence_path = sys.argv[2]

    database_dict = csv.DictReader(open(database_path))
    headers = database_dict.fieldnames[1:]

    sequence_file = open(sequence_path, "r")
    sequence = sequence_file.read()

    str_count = dict()

    for header in headers:
        count = 0
        to_find = header
        while ((sequence.find(to_find) != -1)):
            to_find = to_find + header
            count += 1

        str_count[header] = count

    printed = False
    for person_str_count in database_dict:
        is_found_person = True

        for header in headers:
            is_found_person = is_found_person and str_count[header] == int(person_str_count[header])

        if is_found_person:
            print(person_str_count["name"])
            printed = True

    if not printed:
        print("No match")
