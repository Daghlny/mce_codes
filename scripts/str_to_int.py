
import sys
import json
import os


def check_dict(string):
    global dictionary
    global counter
    # print dict
    if string in dictionary:
        return str(dictionary[string])
    else:
        counter = counter + 1
        dictionary[string] = counter
        return str(counter)



def create_node_nums(x):
    nodes = x.split("\t")
    source = nodes[0]
    destination = nodes[1]
    #source = x.split("\t")[0]
    #destinaton = x.split("\t")[1]
    res = ''
    res = res + check_dict(source) + " " + check_dict(destinaton)
    return res


dictionary = {}
counter = -1

if __name__ == '__main__':
    if len(sys.argv) != 3 :
        print "programName inputfile outputfile"
        exit()
    inputFileName = sys.argv[1]
    outputFileName = sys.argv[2]

    inputFile = open(inputFileName,'r')
    tempFile = open(outputFileName + '_temp','w')
    dicFile = open(outputFileName + '_dict','w')

    for line in inputFile:
        line = line.strip()
        if line[0] == '#':
            continue
        tempFile.write(create_node_nums(line)+"\n")
    tempFile.close()
    edges = open(outputFileName+'_temp','r').read()

    outputFile = open(outputFileName,'w')
    outputFile.write(str(counter+1) + "\n")
    outputFile.write(edges)

    dicFile.write("#originalID  hashedID")
    dicFile.write(json.dumps(dictionary,indent=4))

    os.remove(outputFileName + '_temp')
    print "counter is ",counter

