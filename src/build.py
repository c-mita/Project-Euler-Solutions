#! /usr/bin/env python

from subprocess import call
import sys
import os

g_includeDir = "../include"
g_binDir = "../bin"

def scrapeIncludes( srcFile ):
    headers = []
    with open( srcFile ) as source:
        for line in source:
            if line.startswith("#include"):
                header = line.split()[1]
                if header.startswith("<"): continue
                header = header[1:-1]
                headers.append( header.rsplit(".", 1)[0] + ".cpp" )
    return headers

def main():
    if len(sys.argv) < 2:
        print "Require problem number"
        return
    problem = sys.argv[1]

    srcName = problem + ".cpp"
    exeName = problem + ( ".exe" if os.name == "nt" else "" )

    headerSources = [ x for x in scrapeIncludes( srcName ) if os.path.isfile(x) ]
    sources = ' '.join(headerSources) + ' ' + srcName


    call( "g++ -gdwarf-2 -O3 -Wall -std=c++0x -I%s %s -o %s/%s" % (g_includeDir, sources, g_binDir, exeName) )

if __name__ == "__main__":
    main()
