CC=gcc
TARG=BattleShip
OFILES=obj/*.o
objs=BSUtils.o BSServerClient.o BS.o

all: init BS

init:
	mkdir -p obj

BS: $(objs)
	$(CC) -o $(TARG) $(OFILES)

debug: init $(objs)
	$(CC) -o $(TARG) $(OFILES) -g

BSUtils.o: src/BSUtils.c
	gcc -c src/BSUtils.c -o obj/BSUtils.o

BS.o: src/BS.c
	gcc -pthread -c src/BS.c -o obj/BS.o

BSServerClient.o: src/BSServerClient.c
	gcc -pthread -c src/BSServerClient.c -o obj/BSServerClient.o

clean:
	rm -f -- $(TARG) $(OFILES)
