from sys import argv, exit
from hamilt import read_graph

from colony import Colony

def aco(args):
    print('Enter aco')
    g = None
    if len(args) == 6:
        print(str(args[1]))
        g = read_graph(str(args[1]))
        nants = int(args[2])
        print(g.edges)
        evaprate = float(args[3])
        maxit = int(args[4])
        randinit = int(args[5])
    else:
        exit("Incorrect number of arguments\nplease specify the: 1) test filename; 2) no. of ants.; 3) evaporation rate and 4) max no. of iterations 5) initialization ants 1 - randomize/ 0 nest node 0")
    if g:
        print(g.nodes)
        c = Colony(nants,g,maxit,evaprate,randinit)
    else:
        print('Graph not created')
    
if __name__ == "__main__":
    aco(argv)
    

