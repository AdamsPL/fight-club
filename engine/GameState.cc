#include "GameState.h"

#include <cfloat>
#include <algorithm>

#include <QDebug>

GameState::GameState(int player)
	: player(player), currentPlayer(0)
{
}
