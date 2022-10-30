#!/usr/bin/env python3
import re, fileinput, sys

regex = sys.argv[1]
auto = re.compile(regex)

with fileinput.input(files = sys.argv[2:]) as f:
    for line in f:
        if auto.search(line):
            print(line, end='') 


