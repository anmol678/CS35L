#!/usr/bin/python

import sys, locale
from optparse import OptionParser

class compare:
    def __init__(self, filename1, filename2):
        if filename1 == "-" and filename2 == "-":
            print ("Error: both files can't be read from standard input")
            sys.exit()
        elif filename1 == "-":
            f1 = sys.stdin
            f2 = open(filename2, "r")
        elif filename2 == "-":
            f1 = open(filename1, "r")
            f2 = sys.stdin
        else:
            f1 = open(filename1, "r")  
            f2 = open(filename2, "r")
        self.lines1 = f1.readlines()
        self.lines2 = f2.readlines()
        f1.close()
        f2.close()
        self.c1 = []
        self.c2 = []
        self.c3 = []

    def unsorted_compare(self):
        for i in range(len(self.lines1)):
            for j in range(len(self.lines2)):
                if self.lines1[i] == self.lines2[j]:
                    self.c1.append("\t")
                    self.c2.append("\t")
                    self.c3.append(self.lines1[i])
                    del self.lines2[j]
                    matched = True
                    break
                else:
                    matched = False
            if matched == False:
                self.c1.append(self.lines1[i])
                self.c2.append("")
                self.c3.append("")
        for i in range(len(self.lines2)):
            self.c1.append("\t")
            self.c2.append(self.lines2[i])
            self.c3.append("")

    def sorted_compare(self):
        i = j = 0
        while i < len(self.lines1) and j < len(self.lines2):
            if self.lines1[i] == self.lines2[j]:
                self.c1.append("\t")
                self.c2.append("\t")
                self.c3.append(self.lines1[i])
                i += 1
                j += 1
            elif self.lines1[i] < self.lines2[j]:
                self.c1.append(self.lines1[i])
                self.c2.append("")
                self.c3.append("")
                i += 1
            elif self.lines1[i] > self.lines2[j]:
                self.c1.append("\t")
                self.c2.append(self.lines2[j])
                self.c3.append("")
                j += 1
        if i < j:
            while i < len(self.lines1):
                self.c1.append(self.lines1[i])
                self.c2.append("")
                self.c3.append("")
                i += 1
        elif i > j:
            while j < len(self.lines2):
                self.c1.append("\t")
                self.c2.append(self.lines2[j])
                self.c3.append("")
                j += 1

    def printlist(self, c1, c2, c3):
        mlist = []
        if c1 == True:
            self.c1 = [""] * len(self.c1)
        if c2 == True:
            self.c2 = [""] * len(self.c2)
        if c3 == True:
            self.c3 = [""] * len(self.c3)
        for i in range(len(self.c1)):
            mlist.append(self.c1[i] + self.c2[i] + self.c3[i])
        for i in range(len(mlist)):
            for j in range(len(mlist[i])):
                sys.stdout.write(mlist[i][j])

def main():
    locale.setlocale(locale.LC_ALL, 'C')
    version_msg = "%prog 1.0"
    usage_msg = """%prog [OPTION]... FILE1 FILE2

    Output line by line comparison of FILE1 and FILE2."""

    parser = OptionParser(version=version_msg, 
                          usage=usage_msg)
    parser.add_option("-1",
                      action="store_true", dest="check1", default = False,
                      help="suppress column 1 (lines unique to FILE1)")
    parser.add_option("-2",
                      action="store_true", dest="check2", default = False,
                      help="suppress column 2 (lines unique to FILE2)")
    parser.add_option("-3",
                      action="store_true", dest="check3", default = False,
                      help="suppress column 3 (lines that appear in both FILE1 
and FILE2)")
    parser.add_option("-u", 
                      action="store_true", dest="unsorted", default = False,
                      help="to indicate that FILE1 and/or FILE 2 are/is 
unsorted" )
    options, args = parser.parse_args(sys.argv[1:])

    try:
        check1 = bool(options.check1)
    except:
        parser.error("invalid option type: {0}".
                    format(options.check1))
    try:
        check2 = bool(options.check2)
    except:
        parser.error("invalid option type: {0}".
                     format(options.check2))
    try:
        check3 = bool(options.check3)
    except:
        parser.error("invalid option type: {0}".
                     format(options.check3))
    try:
        unsorted = bool(options.unsorted)
    except:
        parser.error("invalid option type: {0}".
                     format(options.unsorted))

    if len(args) != 2:
        parser.error("wrong number of operands")
    input_file1 = args[0]
    input_file2 = args[1]

    try:
        temp = compare(input_file1, input_file2)
        if unsorted:
            temp.unsorted_compare()
        else:
            temp.sorted_compare()
        temp.printlist(check1, check2, check3)
    except IOError as err:
        parser.error("I/O error({0}): {1}".
                     format(err))

if __name__ == "__main__":
    main()

