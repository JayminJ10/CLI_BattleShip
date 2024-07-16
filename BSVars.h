typedef struct tile {
	int id;
	int indi;
	int indj;
	char sym;
	uint hit;
	char *color;
} Tile;

Tile **board;
Tile **cpu_board;

int blist_e[5] = {2, 3, 3, 4, 5};
int blist_m[4] = {2, 3, 4, 5};
int blist_h[3] = {2, 3, 4};
