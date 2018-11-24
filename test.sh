#!/bin/bash

mkdir -p results
mkdir -p summaries

export_sol() {
	cut -d' ' -f3- $1 > /tmp/sol_$1
}

if which qsub 1>&2 2>/dev/null; then
	COMMAND="./hamilt.sh"
else
	COMMAND="./hamilt"
fi

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

det_ch() {
	for f in *.txt; do
		n=$(echo $f | sed -r 's/[^0-9]//g; s/^0*//;')
		alg=det_ch
		if [ "$1" == "-s" ]; then
			sort -k1 -n results/${alg}_${f%%.txt}_*.txt | head -n1 > "summaries/${alg}_${f%%.txt}.txt"
		elif [ "$1" == "-c" ]; then
			export_sol ${alg}_${f%%.txt}.txt
		else
			while [ $((n--)) -gt 0 ]; do
				queue "${alg}_${f%%.txt}_${n}" "$COMMAND $f $alg $n" "results/${alg}_${f%%.txt}_${n}.txt"
			done
		fi
	done
}

rand_ch() {
	for f in *.txt; do
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
						queue "${alg}_${f%%.txt}_${rand}_${t}" "$COMMAND $f $alg $rand $grasp_runtime" "results/${alg}_${f%%.txt}_${rand}_$t.txt"
					done
				done
			fi
		done
	done
}

local_search() {
	local runtime=$((60*10))
	local alg=ls
	for f in *.txt; do
		v=$(sort -k1 -n results/det_ch_${f%%.txt}_*.txt | head -n1 | cut -d' ' -f 2)
		for n in 2.5opt 3opt; do
			for s in first_improv best_improv rand; do
				queue "${alg}_${f%%.txt}_${n}_${s}" "$COMMAND $f $alg $v $n $s $runtime" "results/${alg}_${f%%.txt}_${n}_${s}.txt"
			done
		done
	done
}

vnd() {
	local alg=vnd
	for f in *.txt; do
		v=$(sort -k1 -n results/det_ch_${f%%.txt}_*.txt | head -n1 | cut -d' ' -f 2)
		queue "${alg}_${f%%.txt}" "$COMMAND $f $alg $v" "results/${alg}_${f%%.txt}.txt"
	done
}

case "$1" in
	det_ch) shift; det_ch "$@";;
	rand_ch) shift; rand_ch "$@" ;;
	ls) shift; local_search "$@" ;;
	vnd) shift; vnd "$@" ;;
esac
