CC=gcc
TARG=BattleShip
OFILES=obj/*.o
objs=BSUtils.o BSServerClient.o BS.o
FLAGS=-std=c99 -Wall

all: init BS

init:
	mkdir -p obj

BS: $(objs)
	$(CC) -o $(TARG) $(OFILES) $(FLAGS)

debug: init $(objs)
	$(CC) -o $(TARG) $(OFILES) -g

BSUtils.o: src/BSUtils.c
	gcc -c src/BSUtils.c -o obj/BSUtils.o

BS.o: src/BS.c
	gcc -c src/BS.c -o obj/BS.o

BSServerClient.o: src/BSServerClient.c
	gcc -c src/BSServerClient.c -o obj/BSServerClient.o

clean:
	rm -f -- $(TARG) $(OFILES)
