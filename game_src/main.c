#include <stdio.h>
#include <stdlib.h>

#include "vec2.h"
#include "game.h"
#include "datas/cstack.h"
#include "datas/carray.h"
#include "datas/chash.h"

int main(int argc, char **argv){
	(void) argc;
	(void) argv;

	Game *game;

	game = Game_Create("acelera", 480, 272);

	Game_Run(game);

	Game_Destroy(game);

	return 0;
}
