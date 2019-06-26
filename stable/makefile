.PHONY: all seq seq2 omp #say_hello generate clean

CC = gcc	# compilet to use
LINKERFLAG = -lm
OPENMPFLAG = -fopenmp

all: seq seq2 omp

seq:
	$(CC) main.c file.c k-means.c -o exe/k-means_seq $(LINKERFLAG)

seq2:
	$(CC) main.c file.c k-meansv2.c -o exe/k-means_seq_v2 $(LINKERFLAG)

omp:
	$(CC) main.c file.c k-means_omp.c -o exe/k-means_omp $(LINKERFLAG) $(OPENMPFLAG)

clean:
	rm -rvf exe/k-means_seq exe/k-means_seq_v2 exe/k-means_omp