CC=gcc
TARG=BattleShip
OBJDIR=obj
objs=obj/*.o

all: init $(TARG)

init:
	mkdir -p obj

$(TARG): BSUtils.o BS.o
	$(CC) -o $(TARG) $(objs)

BSUtils.o: src/BSUtils.c
	gcc -c src/BSUtils.c -o obj/BSUtils.o

BS.o: src/BS.c
	gcc -c src/BS.c -o obj/BS.o

# debug:
# 	$(CC) -o $(TARG) $(SRC) -g
clean:
	rm $(TARG) $(objs)
