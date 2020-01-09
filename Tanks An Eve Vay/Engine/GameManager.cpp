#include "GameManager.h"
#include "Tank.h"
#include "Weapon.h"

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
	if (m_Projectile == nullptr)
		return false;

	if ((m_Projectile->GetPosition().GetX() <= -1.0f) || (m_Projectile->GetPosition().GetX() >= 1.0f))
	{
		delete m_Projectile;
		m_Projectile = nullptr;
		GameManager::ChangePlayer();
	}

	return m_Projectile != nullptr;
}

Vector2f GameManager::GetPlayerPosition(int player)
{
	if (player == 1)
	{
		return m_Player1->GetTankPosition();
	}
	else
	{
		return m_Player2->GetTankPosition();
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
	m_Map->FillTerrain(TerrainType::Hill);
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
void GameManager::RotateTurret(Vector2f mousePosition)
{
	//TODO mouse coord system need to be transfered to game coord system
	double xScale = -1.0f + mousePosition.GetX() / 400.0f;
	double yScale = 1.f - mousePosition.GetY() / 300.0f;

	Vector2f playerPosition = GetPlayerPosition(m_CurrentPlayer);
	double radianAngle = 0.0f;
	if(m_CurrentPlayer == 1)
		radianAngle = m_Player1->getAngle();
	else
		radianAngle = m_Player2->getAngle();
	double k = (yScale - playerPosition.GetY()) / (xScale - playerPosition.GetX());
	if (k <= 0 && yScale > playerPosition.GetY() + GameManager::GetPlayerSize(m_CurrentPlayer).GetY()) {
		radianAngle = std::atan(k);
		if (radianAngle < 0)
		{
			radianAngle += PI;
		}

		if(m_CurrentPlayer == 1)
			m_Player1->setAngle(radianAngle);
		else
			m_Player2->setAngle(radianAngle);
	}
	else if(k>=0 && yScale > playerPosition.GetY() + GameManager::GetPlayerSize(m_CurrentPlayer).GetY())
	{
		radianAngle = std::abs(std::atan(k));

		if (m_CurrentPlayer == 1)
			m_Player1->setAngle(radianAngle);
		else
			m_Player2->setAngle(radianAngle);
	}
#ifdef DEBUG
	OutputDebugStringA(std::to_string(m_Player1->getAngle()).c_str());
	OutputDebugStringA("    ");
#endif
}

double GameManager::GetPlayerAngle()
{
	if (m_CurrentPlayer == 1)
		return m_Player1->getAngle();
	else
		return m_Player2->getAngle();
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
	else
	{ 
		m_Projectile = m_Player2->fireInTheHole();
	}
}

void GameManager::ChangePlayer()
{
	if (m_CurrentPlayer == 1)
		m_CurrentPlayer = 2;
	else
		m_CurrentPlayer = 1;
}

int GameManager::GetCurrentPlayer()
{
	return m_CurrentPlayer;
}

//Initializing of static class member
int GameManager::m_CurrentPlayer = 1;
Player* GameManager::m_Player1 = new Player("Player1", 1);
Player* GameManager::m_Player2 = new Player("Player2", 2);

Terrain* GameManager::m_Map = new Terrain(250);
int GameManager::m_MapSizeN = 0;
int GameManager::m_MapSizeM = 0;

Weapon * GameManager::m_Projectile = nullptr;

