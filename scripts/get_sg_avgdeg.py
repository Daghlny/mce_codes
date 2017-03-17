#!/usr/bin/python3

import json
import os
import sys

if len(sys.argv) != 2:
    print("Program graph.statistics.json")
    exit()
sfile = open(sys.argv[1], 'r')

data = json.load(sfile)
for sginfo in data["degeneracy neighbourhood"]:
    print(sginfo["average degree"])


