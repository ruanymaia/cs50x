# Ruany Maia
# 10 May 2020
# Problem set 7 - Houses

# This program reads data from a .db file and presents it back to the user

import sys
import csv
import cs50

if (len(sys.argv) != 2):
    print("Incorrect input. Please enter: python roster.py house")

else:
    house_query = sys.argv[1]

    # Declare database
    db = cs50.SQL("sqlite:///students.db")

    # Query database
    query = db.execute("SELECT * FROM students WHERE house = ? ORDER BY last, first", house_query)

    for row in query:
        if (row["middle"] is None):
            result = "{} {}, born {}".format(row['first'], row['last'], row['birth'])
            print(result)

        else:
            result = "{} {} {}, born {}".format(row['first'], row["middle"], row['last'], row['birth'])
            print(result)
