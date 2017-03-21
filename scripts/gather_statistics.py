#!/usr/bin/python3

# This script is used for gathering all graph running statistic
import json
import os
import sys
import re

datapath = ""
datasets = ["email-euall"]

if __name__ == '__main__':
    if len(sys.argv) != 2 :
        print("Pragram DataFile")
        exit()
    sfile = open(sys.argv[1], "w+")

    for dataset in datasets:
        print("!script log! "+dataset)
        filepath = datapath+dataset
        files = os.listdir(filepath)
        pattern = re.compile(".+statistics.json")
        filename = ""
        for fname in files:
            filename = pattern.findall(fname)
            if len(filename) != 0:
                filename = filename[0]
                break

        datafilename = datapath+dataset+"/"+filename
        dfile = open(datafilename, "r+")
        data  = json.load(dfile)
        nodenum = data["nodenum"]
        edgenum = data["edgenum"]
        maxdeg  = data["maximum degree"]
        dVnum = data["ddVertex Count"]
        dd = data["degeneracy"]
        dN_avgdegs = []
        for dN in data["degeneracy neighbourhood"]:
            avgdeg = dN["average degree"]
            dN_avgdegs.append(avgdeg)
        dN_avgdeg = sum(dN_avgdegs) / float(len(dN_avgdegs))
        sfile.write("graph:"+dataset+"\n")
        sfile.write("vertex:"+str(nodenum)+"\n")
        sfile.write("edge:"+str(edgenum)+"\n")
        sfile.write("maxdeg:"+str(maxdeg)+"\n")
        sfile.write("degeneracy:"+str(dd)+"\n")
        sfile.write("dNavgdeg:"+str(dN_avgdeg)+"\n")
        sfile.write("\n")

