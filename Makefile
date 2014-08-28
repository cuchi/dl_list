all:
	gcc -O2 -g -o main main.c list.c

clean:
	rm main core*
