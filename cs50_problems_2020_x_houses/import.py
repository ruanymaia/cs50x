# Ruany Maia
# 10 May 2020
# Problem set 7 - Houses

# This program inserts data into a .db file

import sys
import csv
import cs50

if (len(sys.argv) != 2):
    print("Incorrect input. Please enter: python import.py valid_csv_file.csv")

else:
    csv_path = sys.argv[1]

    # Declare database
    db = cs50.SQL("sqlite:///students.db")

    # Open CSV file
    with open(csv_path, "r") as csv_file:

        # Create DictReader
        reader = csv.DictReader(csv_file)

        # Iterate over CSV file
        for row in reader:
            fullname = (row["name"])
            splitname = fullname.split()
            names_list = [None] * 3
            if len(splitname) is 2:
                names_list[0] = splitname[0]
                names_list[2] = splitname[1]
            else:
                names_list = splitname

            # Insert students
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)",
                       names_list[0], names_list[1], names_list[2], row["house"], row["birth"])