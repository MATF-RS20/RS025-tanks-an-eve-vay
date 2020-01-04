#pragma once

#include <vector>

#include "ErrorLogger.h"
#include "Vector2f.h"
#include "Player.h"
#include "Terrain.h"
class GameManager
{
public:
	static bool GetGridValue(int i, int j);

	static Vector2f GetPlayerPosition(int player);

	static void MovePlayer(int player, Vector2f dv);

	static void Initialize();

	static float ScaleRatioX();
	static float ScaleRatioY();

	static int GetMapN();
	static int GetMapM();

protected:
	static int n;
	static int m;
	static Map* Mapa;
private:

	static std::vector<std::vector<bool>> Matrix;
	static Player *m_Player1;
	static Player *m_Player2;

};

