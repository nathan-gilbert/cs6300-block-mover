#!/usr/bin/python
#This is a program to test assignment 1
#in CS 6300

import random
import sys
from time import *

#Where the magic happens.
def main(args=[]):
    blocks = "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFG"
        used_blocks = ""
        x = 0
        t = asctime().replace(" ", "")
        outFile = open("test-" + t + ".txt", "w")

        if(len(args) == 2):
            num_blocks = int(args[0])
                num_tables = int(args[1])
        else:
            num_blocks = random.randrange(1, 31, 1)
                num_tables = random.randrange(1, 11, 1)

        goal_states = random.randrange(1, 4, 1)

        tables_used = []

        print "The number of blocks: %s" % num_blocks
        print "The number of tables: %s" % num_tables

        outFile.write("# of tables: " + str(num_tables) + "\n")
        outFile.write("INITIAL STATE\n")

        while (x < num_blocks):
            block = ""

                table = random.randrange(1, num_tables+1,1)
                if table not in tables_used:
                    tables_used.append(table)
                else:
                    continue

                #How many blocks to put on this table.
                y = random.randrange(1, (num_blocks - x) + 1, 1)

                z = 0
                while (z < y):
                    b = blocks[random.randrange(0, num_blocks, 1)]
                        used_blocks += b
                        block += b + " "
                        z = z + 1

                outFile.write("table " + str(table) + ": " + block.strip() + "\n")
                x = x + y

        x = 0
        table = 0
        tables_used = []
        outFile.write("GOAL STATE\n")

        while (x < goal_states):
            goal_blocks = ""
                z = 0

                table = random.randrange(1, num_tables+1, 1)
                if table not in tables_used:
                    tables_used.append(table)
                else:
                    continue

                #Number of blocks in goal
                y = random.randrange(1, (goal_states - x) + 1, 1)
                while(z < y):
                    goal_blocks += used_blocks[random.randrange(0, len(used_blocks), 1)] + " "
                        z = z + 1

                x = x + y
                outFile.write("table " + str(table) + ": " + goal_blocks + "\n")

        outFile.close()

#The body of the tester.
if len(sys.argv) == 3:
    main(sys.argv[1:])
else:
    main()
