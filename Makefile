hamilt: ssbmh/const_heu.c ssbmh/ls_heu.c ssbmh/hamilt.c ssbmh/pcg_basic.c ssbmh/grasp.c
	gcc $^ -Wall -lm -o $@

hamilt-dbg: ssbmh/const_heu.c ssbmh/ls_heu.c ssbmh/hamilt.c ssbmh/pcg_basic.c ssbmh/grasp.c
	gcc $^ -Wall -g -lm -o $@

clean:
	rm ./hamilt
