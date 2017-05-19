#!/usr/bin/python

# This script is used for gathering all graph running statistic
import json
import os
import sys
import re

datapath = "/home/lyn/graphdata/"
datasets = ["as-skitter","cit-Patents","com-amazon","com-dblp","com-orkut","com-youtube","email-euall","live-journal","road-ca","twitter","web-google","wiki-Talk","wiki-vote"]

def avg_degree(nodenum, edgenum):
    return (edgenum*2) / nodenum

def get_stat_json(dataset):
    filepath = datapath + dataset
    files    = os.listdir(filepath)
    gpattern     = re.compile(".+statistics.json")
    sgpattern    = re.compile(".+sg.json")
    gfilename  = ""
    sgfilename = ""
    gflag = 0
    sgflag = 0
    for fname in files:
        if gflag == 1 and sgflag == 1:
            break
        if gflag == 0:
            gfilename = gpattern.findall(fname)
        if sgflag == 0:
            sgfilename = sgpattern.findall(fname)
        if len(gfilename) != 0 and gflag == 0:
            gfilename = gfilename[0]
            gflag = 1
        if len(sgfilename) != 0 and sgflag == 0:
            sgfilename = sgfilename[0]
            sgflag = 1
    if (gfilename == "") or (sgfilename == ""):
        print("error in get_stat_json")
    gfilename = filepath + "/" + gfilename
    sgfilename = filepath + "/" + sgfilename
    gfile = open(gfilename, "r+")
    sgfile = open(sgfilename, "r+")

    gjson = json.load(gfile)
    sgjson = json.load(sgfile)
    return gjson, sgjson

def vnum_could_delete_from_sg(dd, degtable):
    vnum = 0;
    for d in degtable:
        if d <= 1 | d == dd-1:
            vnum += 1
    return vnum
    

if __name__ == '__main__':
    if len(sys.argv) != 2 :
        print("Pragram DataFile.csv")
        exit()
    resfile = open(sys.argv[1], "w+")
    resfile.write("dataset,nodenum,edgenum,avgdeg,maxdeg,degeneracy,ddVertex num,nbh edgenum,nbh avgdeg,cc,nbh newsize\n")

    for dataset in datasets:
        print("!script log! "+dataset)
        gjson, sgjson = get_stat_json(dataset)
        resfile.write(dataset+","+str(gjson["nodenum"])+","+str(gjson["edgenum"])+","+str(gjson["average degree"])+","+str(gjson["maximum degree"])+","+str(gjson["degeneracy"])+","+str(gjson["ddVertex Count"])+",")
        degeneracy = gjson["degeneracy"]
        dNedgenums = 0
        dNavgdegs  = 0
        dNccnums   = 0
        dNnewsizes = 0
        cnt = 0
        for dN in sgjson["degeneracy neighbourhood"]:
            cnt += 1
            dNedgenums += dN["edgenum"]
            dNavgdegs  += dN["average degree"]
            dNccnums   += dN["CC count"]
            dNnewsizes   += degeneracy-vnum_could_delete_from_sg(degeneracy, dN["degree Table"])

        resfile.write(str(float(dNedgenums)/cnt)+","+str(float(dNavgdegs)/cnt)+","+str(float(dNccnums)/cnt)+","+str(float(dNnewsizes)/cnt)+"\n")

