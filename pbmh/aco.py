from sys import argv, exit
from hamilt import read_graph
from colony import Colony

def aco(args):
    print(args)
    g = None
    if len(args) >= 6:
        # init opt params
        ne=0
        alp=1
        bet=1
        while args:
            arg = args.pop()
            if arg[0] == '--alg':
                alg = arg[1]
            elif arg[0] == '--test':
                g = read_graph(str(arg[1]))
            elif arg[0] == '--nants':
                nants = int(arg[1])
            elif arg[0] == '--evaprate':
                evaprate = float(arg[1])
            elif arg[0] == '--maxit':
                maxit = int(arg[1])
            elif arg[0] == '--randinit':
                randinit = int(arg[1])
            elif arg[0] == '--nelit':
                ne = int(arg[1])
            elif arg[0] == '--alph':
                alp = float(arg[1])
            elif arg[0] == '--beta':
                bet = float(arg[1])
    else:
        exit("Incorrect number of arguments.\nplease specify the required parameters:\n* --alg type of ACO algorithm: (antsys, elitist);\n* --test test filename;\n* --nants no. of ants;\n* --evaprate evaporation rate;\n* --maxit max no. of iterations;\n* --randinit initialization position of ants: (1 - randomized/ 0 - start at node 0, assuming there is such node);\nand the optional arguments:\n+ --nelit natural int no. of elitist ants;\n+ --alph alpha exp of pherormone, used to calculate probability of picking edge\n+ --beta beta exp of visibility, used to calculate probability of picking edge")
    if g:
        if randinit == 0 and not g.has_node(0):
            exit("Graph must contain node 0, to initialize ants at 0.")
        c = Colony(alg,nants,g,maxit,evaprate,randinit,nelit=ne,alph=alp,beta=bet)
    else:
        print('Graph not existing')
    
if __name__ == "__main__":
    it = iter(argv[1:])
    a = zip(it, it)
    aco(list(a))
