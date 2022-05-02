import re
import sys
import os

tru_hits = ['9', '4', '2', '2','2','265189','268469', '9', '5', '2', '2','2','265189', '265230']
tru_miss = ['8', '5', '3', '3','3','21775', '18495','8', '4', '3',  '3','3','21775','21734']
tru_evic = ['6', '2', '1', '0','0','21743', '18431','6', '1', '1', '0','0','21743','21670']

variations = ['(1,1,1)','(4,2,4)','(2,1,4)','(2,2,4)','(2,4,5)','(5,1,5)','(5,2,5)']
traces = ['traces/yi2.trace','traces/yi.trace','traces/dave.trace','traces/dave.trace','traces/dave.trace','traces/long.trace','traces/long.trace']
def main():
    hits = []
    misses = []
    evictions = []
    os.system('./csim -s 1 -E 1 -b 1 -p 1 -t traces/yi2.trace >> results.txt')
    os.system('./csim -s 4 -E 2 -b 4 -p 1 -t traces/yi.trace >> results.txt')
    os.system('./csim -s 2 -E 1 -b 4 -p 1 -t traces/dave.trace >> results.txt')
    os.system('./csim -s 2 -E 2 -b 4 -p 1 -t traces/dave.trace >> results.txt')
    os.system('./csim -s 2 -E 4 -b 5 -p 1 -t traces/dave.trace >> results.txt')
    os.system('./csim -s 5 -E 1 -b 5 -p 1 -t traces/long.trace >> results.txt')
    os.system('./csim -s 5 -E 2 -b 5 -p 1 -t traces/long.trace >> results.txt')


    os.system('./csim -s 1 -E 1 -b 1 -p 2 -t traces/yi2.trace >> results.txt')
    os.system('./csim -s 4 -E 2 -b 4 -p 2 -t traces/yi.trace >> results.txt')
    os.system('./csim -s 2 -E 1 -b 4 -p 2 -t traces/dave.trace >> results.txt')
    os.system('./csim -s 2 -E 2 -b 4 -p 2 -t traces/dave.trace >> results.txt')
    os.system('./csim -s 2 -E 4 -b 5 -p 2 -t traces/dave.trace >> results.txt')
    os.system('./csim -s 5 -E 1 -b 5 -p 2 -t traces/long.trace >> results.txt')
    os.system('./csim -s 5 -E 2 -b 5 -p 2 -t traces/long.trace >> results.txt')

    f = open("results.txt", "r")
    for li in f.readlines():
        results = re.findall('[0-9]+',li)
        hits.append(results[0])
        misses.append(results[1])
        evictions.append(results[2])

    score = []
    if len(hits) == 0:
        print('TEST_CSIM_RESULTS=' + 0)
        return

    for i in range(0,14):
        if hits[i] == tru_hits[i] and misses[i] == tru_miss[i] and evictions[i] == tru_evic[i]:
            score.append(1)
        else:
            score.append(0)

    print('                      Your simulator     Reference simulator')
    print('Points (s,E,b)  Hits   Misses  Evicts  Hits  Misses  Evicts')
    ind = 0
    for j in range(0,14):
        print '     %d %s  %s %s  %s  %s %s %s  %s' % (score[j],variations[ind].ljust(6),hits[j].ljust(6),misses[j].ljust(6),evictions[j].ljust(6),tru_hits[j].ljust(6),tru_miss[j].ljust(6),tru_evic[j].ljust(6),traces[ind].ljust(6))
        ind += 1
        if j == 6:
            ind = 0

    print('     %d' % sum(score))

    print('TEST_CSIM_RESULTS=' + str(sum(score)))
    f.close()


if __name__== "__main__":
    main()
