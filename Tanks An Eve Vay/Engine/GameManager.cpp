#include "GameManager.h"
#include "Tank.h"
#include "Weapon.h"
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
	return m_Map->GetElement(i, j);
}

Vector2f GameManager::GetProjectilePosition()
{
	return m_Projectile->fly();
}

Vector2f GameManager::GetProjectileSize()
{
	return m_Projectile->GetSize();
}

bool GameManager::Projectile()
{
	return m_Projectile != nullptr;
}

Vector2f GameManager::GetPlayerPosition(int player)
{
	if (player == 1)
	{
<<<<<<< HEAD
		return Vector2f(0.0,0.0);
	}
	else
	{
		return Vector2f(0,0);
=======
		return m_Player1->GetTankPosition();
	}
	else
	{
		return m_Player2->GetTankPosition();
>>>>>>> master
	}
}

Vector2f GameManager::GetPlayerSize(int player)
{
	if (player == 1)
	{
		return m_Player1->getPlayerTank().GetSize();
	}
	else
	{
		return m_Player2->getPlayerTank().GetSize();
	}
}

void GameManager::MovePlayer(Vector2f dv)
{
	if (m_CurrentPlayer == 1)
	{
		m_Player1->moveMyTank(dv);
	}
	else if (m_CurrentPlayer == 2)
	{
		m_Player2->moveMyTank(dv);
	}
}

void GameManager::Initialize()
{
	m_Map->FillTerrain(TerrainType::Flat);
	m_MapSizeN = m_Map->GetN();
	m_MapSizeM = m_Map->GetM();
	m_Projectile = m_Player1->fireInTheHole();
}

float GameManager::ScaleRatioX()
{
	return 2.f / m_MapSizeN;
}

float GameManager::ScaleRatioY()
{
	return 2.f / m_MapSizeM;
}
void GameManager::RotateTurret(Vector2f mousePosition)
{
	//TODO mouse coord system need to be transfered to game coord system
	double xScale = -1.0f + mousePosition.GetX() / 400.0f;
	double yScale = 1.f - mousePosition.GetY() / 300.0f;

	Vector2f playerPosition = GetPlayerPosition(1);
	double k = (yScale - playerPosition.GetX()) / (xScale - playerPosition.GetY());
	double radianAngle = std::atan(k);
	/*if (radianAngle < 0)
	{
		if (yScale > playerPosition.GetY() && yScale > 0)
		{
			radianAngle += 3.1415926536;
			m_Player1->setAngle(radianAngle);
		}
	}
	else
	{
		if (yScale > playerPosition.GetY())
		{
			m_Player1->setAngle(radianAngle);
		}

	}*/
	m_Player1->setAngle(radianAngle);
	OutputDebugStringA(std::to_string(m_Player1->getAngle()).c_str());
	OutputDebugStringA("    ");

	//*(180.0 / 3.141592653589793238463)
}
double GameManager::GetPlayerAngle()
{
	return m_Player1->getAngle();
}
int GameManager::GetMapN()
{
	return m_MapSizeN;
}
int GameManager::GetMapM()
{
	return m_MapSizeM;
}

void GameManager::Fire()
{
	if (m_CurrentPlayer == 1)
	{
		m_Projectile = m_Player1->fireInTheHole();
	}
}

int GameManager::m_MapSizeN = 0;
int GameManager::m_MapSizeM = 0;

Terrain* GameManager::m_Map = new Terrain(100);
Player* GameManager::m_Player1 = new Player("Player1", 1);
Player* GameManager::m_Player2 = new Player("Player2", 2);

int GameManager::m_CurrentPlayer = 1;

Weapon * GameManager::m_Projectile = nullptr;

