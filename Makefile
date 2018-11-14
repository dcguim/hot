hamilt: ssbmh/const_heu.c ssbmh/hamilt.c ssbmh/pcg_basic.c
	gcc $^ -Wall -lm -o $@

clean:
	rm ./hamilt
