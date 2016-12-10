import sys
from collections import defaultdict

class clase (object):
    def __init__(self,n=3):
        self.n = n
        self.ngram_counts = [defaultdict(int) for i in xrange(self.n)]
        self.ngram_counts[1] = 15
        for i,e in self.ngram_counts:
            print (i,e), x[(i,e)]

def usage():
    print """
    python test.py
        archivo test.py
    """

if __name__ == "__main__":
    c = clase(3);
    sys.stdout.write('funciona python')