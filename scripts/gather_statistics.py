#!/usr/bin/python3

# This script is used for gathering all graph running statistic
import json
import os
import sys
import re

datapath = "/bigdata/lyn-data/"
datasets = ["as-skitter"]

class Neighbourhood:
    def __init__(self, _v, _e, _avg, _cc, _dvnum):
        self.E = _e
        self.V = _v
        self.avgdeg = _avg
        self.cc = _cc
        self.dvnum = _dvnum # the vertex count could be deleted

    def updateAvgDeg(self):
        self.avgdeg = self.E * 2 / self.V

    def newsize(self):
        return self.V - self.dvnum

    def csvStr(self):
        dNedgenum = str(self.E)
        dNavgdeg = str(self.avgdeg)
        dNccnum   = str(self.cc)
        dNnewsize = str(self.V - self.dvnum)
        resStr = dNedgenum + "," + dNavgdeg + "," + dNccnum + "," + str(self.density()) + "," + dNnewsize
        return resStr

    def density(self, roundnum = 3):
        maxedges = float( self.V * (self.V-1) / 2 )
        return round(float(self.E / maxedges), roundnum)


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
    if len(sys.argv) < 2 :
        print("Pragram DataFile.csv [nbhflag]")
        print("nbhflag=1 : all nbh's statistics\nnbhflag=2 : only average statitics\nnbhflag=3 : the max and min statistic")
        exit()
    nbhflag = 1;
    if len(sys.argv) == 3:
        nbhflag = int(sys.argv[2])
    resfile = open(sys.argv[1], "w+")
    resfile.write("dataset,nodenum,edgenum,avgdeg,maxdeg,density,degeneracy,ddVertexNum,NBHedgenum,NBHavgdeg, NBHcc,NBHdensity,NBHnewsize\n")

    for dataset in datasets:
        print("!script log! "+dataset)
        gjson, sgjson = get_stat_json(dataset)
        density = float(gjson["edgenum"]) / (float(gjson["nodenum"]) * float(gjson["nodenum"] - 1) / 2)
        density = round(density, 5)
        resfile.write(dataset+","+str(gjson["nodenum"])+","+str(gjson["edgenum"])+","+str(gjson["average degree"])+","+str(gjson["maximum degree"])+","+str(density)+","+str(gjson["degeneracy"])+","+str(gjson["ddVertex Count"])+",")
        degeneracy = gjson["degeneracy"]

        maxDNEdgeNum = 0;
        dstDN = Neighbourhood(0,0,0,0,0);
        for dN in sgjson["degeneracy neighbourhood"]:
            nb = Neighbourhood(degeneracy, dN["edgenum"], dN["average degree"], dN["CC count"], vnum_could_delete_from_sg(degeneracy, dN["degree Table"]))
            if nb.E > maxDNEdgeNum:
                maxDNEdgeNum = nb.E
                dstDN = nb
        resfile.write(dstDN.csvStr() + "\n")

