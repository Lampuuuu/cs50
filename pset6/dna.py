from sys import argv, exit
import csv


def main():
    # check for correct usage
    if len(argv) != 3:
        exit("Usage: python dna.py <database.csv> <text.txt>")

    # get a list of names and STRs
    STRs = get_essentials("STR")
    names = get_essentials("names")

    # create a list of dictionaries: each with name and STRs and
    # theircorresponding values
    dictlist = get_dictlist()

    # open the sequence file and load it into a single string
    seqfile = open(argv[2], "r")
    sequence = seqfile.read()

    # create a dictionary filled with STRs and their corresponding longest chains
    # from the DNA-sequence
    STRdict = {}
    for i in range(len(STRs)):
        STRdict.update({STRs[i]: find_longest_chain(STRs[i], sequence)})

    # compare every element of the dictlist with STRdict
    for d in dictlist:

        # set true before for loop, if it stays true, match found
        matching = True

        # go through each key in the dict
        for key in STRdict:

            # if key is in both dicts and their values are not equal
            # they don't match
            if (key in d) and (int(STRdict[key]) != int(d[key])):
                matching = False

        # if matching stays after all keys are compared,
        # we have a match
        if matching:
            # print the name key value of current dict and return
            print(d['name'])
            return

    # if none of the Dictionaries match we don't have one.
    print("No match")
    return


# get essential values(names and STRs)
def get_essentials(instring):
    # open the csvfile and make a reader iterator.
    with open(argv[1], "r") as csvfile:
        csvreader = csv.reader(csvfile)

        # first extract the header, containing the names.
        header = next(csvreader)

        # create variables for names and STRs
        nms = []
        strs = []

        # create STR list if the input wants STR
        if instring == "STR":
            for seq in header[1:]:
                strs.append(seq)

            return strs

        # create name list if input wants names.
        if instring == "names":
            for row in csvreader:
                nms.append(row[0])

            return nms


# create a list of dictionaries from csv-input file
def get_dictlist():
    with open(argv[1], "r") as data:

        # create multiple dictionaries via the csv.DictReader method
        # and put them all together in dlist
        dlist = []
        dictreader = csv.DictReader(data)
        for row in dictreader:
            dlist.append(row)

    return dlist


# Algorithm to find the longest chain of this STR in the DNA-sequence
def find_longest_chain(STR, sequence):

    # initialize variables
    longest_chain = 0
    found = sequence.find(STR)
    counter = 1
    search = found

    # while loop going through the entire string.
    # until there are no more copies of the STR to find
    while found != -1:

        # always keep longest_chain counter updated
        if counter > longest_chain:
            longest_chain = counter

        # find the next copy of STR
        found = sequence[search + len(STR):].find(STR)

        # if it is consecutive, increase counter and search to next copy
        if found == 0:
            counter += 1
            search += len(STR)

        # if not zero back counter go to next found copy.
        else:
            counter = 0
            search += found

    # return the longest chain.
    return longest_chain


# execute main function
main()
