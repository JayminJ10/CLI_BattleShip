typedef unsigned short 	u8;
typedef unsigned int 	u32;
typedef int 			i32;

typedef struct tile {
	u8 id;
	u8 indi;
	u8 indj;
	char sym;
	u8 hit;
	char *color;
} Tile;

typedef struct player {
	Tile **board;
	u8 boats;
} Player;

typedef struct packet {
	//u8 type;

	//u32 size;

	u8 i;
	u8 j;
} packet_t;