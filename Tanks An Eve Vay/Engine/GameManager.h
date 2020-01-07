#pragma once

#include <vector>

#include "ErrorLogger.h"
#include "Vector2f.h"
#include "Player.h"
#include "Terrain.h"

class Weapon;

class GameManager
{
public:

	static void Initialize();

	static void MovePlayer(Vector2f dv);
	static Vector2f GetPlayerPosition(int player);
	static Vector2f GetPlayerSize(int player);

	
	static float ScaleRatioX();
	static float ScaleRatioY();

	static void RotateTurret(Vector2f mousePosition);
	static double GetPlayerAngle();

	static int GetMapN();
	static int GetMapM();

	static void Fire();

	static void ChangePlayer();

	static int GetCurrentPlayer();

	static bool GetGridValue(int i, int j);
	
	static Vector2f GetProjectilePosition();
	static Vector2f GetProjectileSize();
	static bool Projectile();

private:
	static Terrain* m_Map;
	static int m_MapSizeN;
	static int m_MapSizeM;

	static Player *m_Player1;
	static Player *m_Player2;

	static int m_CurrentPlayer;

	static Weapon* m_Projectile;

};

