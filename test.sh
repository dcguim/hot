#!/bin/bash

mkdir -p results
mkdir -p summaries

export_sol() {
	cut -d' ' -f3- $1 > /tmp/sol_$1
}

queue() {
	local name="$1"
	local command="$2"
	local outfile="$3"

	if which qsub 1>&2 2>/dev/null; then
		qsub -N "$name" -o "$outfile" $command;
		echo "queued $name"
	else
		echo "$command > $outfile"
	fi
}

ch() {
	for f in 00*.txt; do
		local grasp_runtime=$((60*10))
		local r_list="0.4"
		n=$(echo $f | sed -r 's/[^0-9]//g; s/^0*//;')
		for alg in rand_ch grasp; do
			if [ "$1" == "-s" ]; then
				sort -k1 -n results/${alg}_${f%%.txt}_*.txt | head -n1 > "summaries/${alg}_${f%%.txt}.txt"
			elif [ "$1" == "-c" ]; then
				export_sol ${alg}_${f%%.txt}.txt
			else
				for t in {0..11}; do
					for rand in $r_list; do
						queue "${alg}_${f%%.txt}_${rand}_${t}" "./hamilt $f $alg $rand $grasp_runtime" "results/${alg}_${f%%.txt}_${rand}_$t.txt"
					done
				done
			fi
		done

		alg=det_ch
		if [ "$1" == "-s" ]; then
			sort -k1 -n results/${alg}_${f%%.txt}_*.txt | head -n1 > "summaries/${alg}_${f%%.txt}.txt"
		elif [ "$1" == "-c" ]; then
			export_sol ${alg}_${f%%.txt}.txt
		else
			while [ $((--n)) -gt 0 ]; do
				queue "${alg}_${f%%.txt}_${n}" "./hamilt $f $alg $n" "results/${alg}_${f%%.txt}_${n}.txt"
			done
		fi
	done
}

ch "$@"
