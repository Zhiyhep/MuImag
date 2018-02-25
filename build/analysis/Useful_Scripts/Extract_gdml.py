#!/usr/bin/env python
#-*- coding: utf-8 -*-
'''
Description:
    This script automatically extract the constant values defined in gdml.
    And generates a "DetectorLayout.hh" file based on the constants.
Usage:
    Extract_gdml.py PATH_TO_GDML
'''
import sys
import re
if len(sys.argv) == 2:
    fin = open(sys.argv[1],'r')
    fout = open("../DetectorLayout.hh",'w')
    p = re.compile(r'(?<=<constant)\s+name="(\w*?)"\s+value="(-?\d*\.?\d*\.*)"\s*(?=/>)')
    for line in fin.readlines():
        l = p.findall(line)
        print(line)
        print(l)
        if l:
            fout.writelines(['double ',l[0][0],' = ',l[0][1],';\n'])
    fin.close()
    fout.close()
else:
    print("Error! Number of arguments should be 1")
