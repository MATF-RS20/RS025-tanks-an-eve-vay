#pragma once

#include <vector>

#include "ErrorLogger.h"
#include "Vector2f.h"
#include <map>

class Player;
class Terrain;
class Weapon;

enum PlayerMovement
{
	LEFT,
	RIGHT,
	INVALID
};

class GameManager
{
public:

	static void Initialize();

	static Vector2f MovePlayer(int player);
	static Vector2f GetPlayerPosition(int player);
	static Vector2f GetPlayerSize(int player);

	static unsigned GetPlayerScore(int player);

	static void AddScoreToPlayer(int player, unsigned score);


	static float ScaleRatioX();
	static float ScaleRatioY();

	static void RotateTurret(Vector2f mousePosition);
	static double GetPlayerPower();
	static std::string GetPlayerName(int num);
	static double GetPlayerAngle();
	static int GetPlayerHealth(int player);
	static int GetMapN();
	static int GetMapM();

	static void Fire();

	static void ChangePlayer();

	static int GetCurrentPlayer();

	static bool GetGridValue(int i, int j);
	
	static Vector2f GetProjectilePosition();
	static double GetMovingAngle(int player);
	static Vector2f GetProjectileSize();
	static bool Projectile();

	static bool PlayerHitted(int p);

	static void SetPlayerHitted(bool hit, int player);

	static bool CheckCollision();
	static void UpdateTerrainOutline();

	static void AllowMove(PlayerMovement side);

	static bool CanFire();

	static int MovesLeft();

	static void ReduceMoves();

	static void SetMovesDefault();

	static void SetWindowSize(Vector2f size);

	static void GameOver();

	static bool getGameIndicator();

	static void ShutDown();

	static void RestartGameState();
protected:
	static void Fall(int i);

private:
	static Terrain* m_Map;
	static int m_MapSizeN;
	static int m_MapSizeM;

	static bool m_GameOverIndicator;

	static Player *m_Player1;
	static Player *m_Player2;

	static int m_CurrentPlayer;

	static bool m_GameRepeat;

	static Weapon* m_Projectile;

	static std::vector<unsigned>* m_Outline;

	static int m_possibleMoves;
	static PlayerMovement m_nextMove;

	static int m_WindowHeight;
	static int m_WindowWidth;


	static Player * GetPlayerOnTurn();
	static Player * GetPlayer(int i);
};

