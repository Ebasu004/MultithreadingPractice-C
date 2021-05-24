unsynced:
	gcc -pthread -o Threads Threads.c -lpthread

clean:
	rm -f Threads core *~
