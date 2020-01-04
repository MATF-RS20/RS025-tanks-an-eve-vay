#include "GameManager.h"
#include "Tank.h"
enum PlayerMove {
	Left,
	Right
};

bool GameManager::GetGridValue(int i, int j) {
	if (i >= n || j >= m || i < 0 || j < 0)
	{
		ErrorLogger::Log("Grid selected index out of range!!!!");
		return false;

	}
	return Matrix[i][j];
}

Vector2f GameManager::GetPlayerPosition(int player)
{
	if (player == 1)
	{
		return Vector2f();
	}
	else
	{
		return Vector2f();
	}
}

void GameManager::MovePlayer(int player, Vector2f dv)
{
	//if (player == 1)
	//{
	//	p1 = p1 + dv;
	//}
	//else
	//{
	//	p2 = p2 + dv;
	//}
}

void GameManager::Initialize()
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m / 2; j++)
		{
			Matrix[i][j] = true;
		}
	}
}

float GameManager::ScaleRatioX()
{
	return 2.f / n;
}

float GameManager::ScaleRatioY()
{
	return 2.f / m;
}
int GameManager::GetMapN()
{
	return n;
}
int GameManager::GetMapM()
{
	return m;
}

int GameManager::n = 100;
int GameManager::m = 100;
std::vector<std::vector<bool>> GameManager::Matrix = std::vector<std::vector<bool>>(n, std::vector<bool>(m, false));
Map* GameManager::Mapa = new Map();
Player* GameManager::m_Player1 = new Player();
Player* GameManager::m_Player2 = new Player();