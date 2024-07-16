CC=gcc
OBJ=BattleShip
SRC=BS.c

all: BS

BS:
	$(CC) -o $(OBJ) $(SRC)
debug:
	$(CC) -o $(OBJ) $(SRC) -g
clean:
	rm BattleShip
