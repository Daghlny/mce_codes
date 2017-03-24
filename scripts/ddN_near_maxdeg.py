import json
import os
import sys
import re

datapath = "/"
datasets = ["email-euall"]

if __name__ == "__main__":
    for dataset in datasets:
        print("script log: "+ dataset)
        filepath = datapath + dataset
        files = os.listdir(filepath)
        pattern = re.compile(".+sg.json")
        sgfilename = ""
        for fname in files:
            sgfilename = pattern.findall(fname)
            if len(sgfilename) != 0:
                sgfilename = sgfilename[0]
                break
        sgfilename = datapath+dataset+"/"+sgfilename
        sgfile = open(sgfilename, "r+")
        jsondata = json.load(sgfile)
        ddeg = jsondata["degeneracy"]
        dNbhs = jsondata["degeneracy neighbourhood"]
        maxd_vnum = []
        for nbh in dNbhs:
            maxd_vnum.append(0)
            for deg in nbh["degree Table"]:
                if deg == ddeg-1:
                    maxd_vnum[-1] += 1
        print(maxd_vnum)
        print("avg vertex count:" + str(sum(maxd_vnum) / len(maxd_vnum)))


