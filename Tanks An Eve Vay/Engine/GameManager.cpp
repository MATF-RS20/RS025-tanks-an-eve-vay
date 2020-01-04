#include "GameManager.h"
#include "Tank.h"
enum PlayerMove {
	Left,
	Right
};

bool GameManager::GetGridValue(int i, int j) {
	if (i >= m_MapSizeN || j >= m_MapSizeM || i < 0 || j < 0)
	{
		ErrorLogger::Log("Grid selected index out of range!!!!");
		return false;

	}
	return m_Map->GetElement(i,j);
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
	m_Map->FillTerrain(TerrainType::Flat);
	m_MapSizeN = m_Map->GetN();
	m_MapSizeM = m_Map->GetM();
}

float GameManager::ScaleRatioX()
{
	return 2.f / m_MapSizeN;
}

float GameManager::ScaleRatioY()
{
	return 2.f / m_MapSizeM;
}
int GameManager::GetMapN()
{
	return m_MapSizeN;
}
int GameManager::GetMapM()
{
	return m_MapSizeM;
}

int GameManager::m_MapSizeN = 0;
int GameManager::m_MapSizeM = 0;
std::vector<std::vector<bool>> GameManager::Matrix = std::vector<std::vector<bool>>(100, std::vector<bool>(100, false));
Terrain* GameManager::m_Map = new Terrain(10);
Player* GameManager::m_Player1 = new Player();
Player* GameManager::m_Player2 = new Player();