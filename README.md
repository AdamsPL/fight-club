colloseum
=========

Arena for algorithmic battles

# Compilation #
	mkdir build
	cd build
	cmake ..
	make

# Reversi #
## Starting a reversi match ##
	./reversi/engine_cli [PARAMS] --player0 PATH_TO_PLAYER0 --player1 PATH_TO_PLAYER1

## Reversi parameters#
**--size N** - _half_ of board size

**--enable-ui true** - if set engine will display an UI with current game status

# Tournament mode #
	./tournament.sh PLAYER_LIST.txt PATH_TO_ENGINE ENGINE_PARAMS
