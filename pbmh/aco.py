from sys import argv, exit
from hamilt import read_graph

from colony import Colony

def aco(args):
    print('Enter aco')
    g = None
    if len(args) == 3:
        print(str(args[1]))
        g = read_graph(str(args[1]))
        nants = int(args[2])
        print(g.edges)
    else:
        exit("Incorrect number of arguments.")
    if g:
        init_node = 0
        c = Colony(nants,g,init_node)
    else:
        print('Graph not created')
    
if __name__ == "__main__":
    aco(argv)
    

