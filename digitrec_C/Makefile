CC = gcc
FLAGS = -g -std=c99
MEMCHECK=valgrind --tool=memcheck --leak-check=full --track-origins=yes

D_PROG=digit_rec
S_PROG=sparsify

.PHONY : $(D_PROG) $(S_PROG)

all: $(D_PROG) $(S_PROG)

$(D_PROG):
	$(CC) $(FLAGS) -o $(D_PROG) digit_rec.c calc_dist.c test_digitrec.c utils.c

$(S_PROG):
	$(CC) $(FLAGS) -o $(S_PROG) sparsify.c make_sparse.c test_sparsify.c utils.c

$(S_PROG)-memcheck: $(S_PROG)
	$(MEMCHECK) ./$(S_PROG)
	
clean:
	rm -f core $(D_PROG) $(S_PROG) *~ *.o "#"*"#" Makefile.bak
