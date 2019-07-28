main: list.o tree.o marathon.o input.o main.o
	gcc -o main list.o tree.o marathon.o input.o main.o


main.o:  main.c list.h tree.h marathon.h input.h
	gcc -Wall -Wextra -std=c11 -O2 -c -g main.c

input.o: input.c list.h tree.h marathon.h input.h
	gcc -Wall -Wextra -std=c11 -O2 -c -g input.c

marathon.o: marathon.c list.h tree.h marathon.h
	gcc -Wall -Wextra -std=c11 -O2 -c -g marathon.c

tree.o: tree.c list.h tree.h
	gcc -Wall -Wextra -std=c11 -O2 -c -g tree.c

list.o: list.c list.h
	gcc -Wall -Wextra -std=c11 -O2 -c -g list.c


clean:
	rm list.o
	rm tree.o
	rm marathon.o
	rm input.o
	rm main.o
	rm main
