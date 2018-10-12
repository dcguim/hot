% Check for a hamiltonian path

% nodes
int: nn = 20;
set of int: node = 1..nn;

% edges
int: ne = 28;
set of int: edge = 1..ne;

% source nodes and ending nodes
array[edge] of var node: from;
array[edge] of var node: to;

% hamiltonian path
array[node] of var node: hpath;

% input graph
constraint from[1] = 1;
constraint from[2] = 1;
constraint from[3] = 2;
constraint from[4] = 2;
constraint from[5] = 3;
constraint from[6] = 3;
constraint from[7] = 4;
constraint from[8] = 5;
constraint from[9] = 6;
constraint from[10] = 7;
constraint from[11] = 7;
constraint from[12] = 8;
constraint from[13] = 9;
constraint from[14] = 9;
constraint from[15] = 10;
constraint from[16] = 10;
constraint from[17] = 11;
constraint from[18] = 12;
constraint from[19] = 13;
constraint from[20] = 14;
constraint from[21] = 15;
constraint from[22] = 15;
constraint from[23] = 16;
constraint from[24] = 16;
constraint from[25] = 17;
constraint from[26] = 18;
constraint from[27] = 19;
constraint from[28] = 19;

constraint to[1] = 2;
constraint to[2] = 12;
constraint to[3] = 3;
constraint to[4] = 14;
constraint to[5] = 4;
constraint to[6] = 6;
constraint to[7] = 5;
constraint to[8] = 1;
constraint to[9] = 7;
constraint to[10] = 8;
constraint to[11] = 20;
constraint to[12] = 4;
constraint to[13] = 8;
constraint to[14] = 19;
constraint to[15] = 5;
constraint to[16] = 9;
constraint to[17] = 10;
constraint to[18] = 11;
constraint to[19] = 12;
constraint to[20] = 13;
constraint to[21] = 6;
constraint to[22] = 14;
constraint to[23] = 15;
constraint to[24] = 20;
constraint to[25] = 16;
constraint to[26] = 17;
constraint to[27] = 18;
constraint to[28] = 9;

% Every node in the hpath must be mapped to a source node
constraint forall(n in 1..nn-1)(
			      exists(e in 1..ne)(
						 hpath[n] = from[e]));

% Every node in the hpath must be mapped to a ending node
constraint forall(n in 2..nn)(
			      exists(e in 1..ne)(
						 hpath[n] = to[e]));

% Every two adjacent nodes in a path must be a source and a ending node
constraint forall(n in 1..nn-1)(
				exists(e in 1..ne)(
						   hpath[n] = from[e] /\
						   hpath[n+1] = to[e]));

% All random pair nodes in the hamiltonian path must be distinct
constraint forall(i,j in 1..nn where i < j)(
					    hpath[i]!=hpath[j]);
			      
solve satisfy;

output ["hpath=", show(hpath), "\t from=", show(from),
	"\t to=", show(to)];
