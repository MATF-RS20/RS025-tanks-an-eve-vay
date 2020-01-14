#include "GameManager.h"
#include "Player.h"
#include "Tank.h"
#include "Terrain.h"
#include "Weapon.h"
#include <ctime>
#include <cstdlib>

#define Coords(x) ((m_MapSizeN*(x))/2.0f+(m_MapSizeN/2.0f))
#define lineLength(x1,y1,x2,y2) (std::sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)))
#define RealCoords(x) (((x)-m_MapSizeN/2)*(2.0f/m_MapSizeN))
#define lenghtInMap(x) ((((x)-m_MapSizeN/2)*(2.0f/m_MapSizeN))+1.0f)

void GameManager::Initialize()
{
	std::srand(time(nullptr));
	RestartGameState();
}

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

Player* GameManager::GetPlayerOnTurn()
{
	if (m_CurrentPlayer == 1)
	{
		return m_Player1;
	}
	else if (m_CurrentPlayer == 2)
	{
		return m_Player2;
	}
	else
	{
		ErrorLogger::Log("GetPlayerOnMove: current player");
		ShutDown();
		exit(1);
	}

}

Player* GameManager::GetPlayer(int i)
{
	if (i == 1)
	{
		return m_Player1;
	}
	else if (i == 2)
	{
		return m_Player2;
	}
	else
	{
		ErrorLogger::Log("GetPlayer: invalid player");
		ShutDown();
		exit(1);
	}
}

double GameManager::GetMovingAngle(int player)
{
	return GetPlayer(player)->getTankDrawAngle();
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


bool GameManager::PlayerHitted(int p)
{
	return GetPlayer(p)->getHitted();
}

void GameManager::SetPlayerHitted(bool hit, int player)
{
	return GetPlayer(player)->setHitted(hit);
}

bool GameManager::CheckCollision()
{

	// Tank 
	Vector2f tankPosition;
	Vector2f tankSize;

	if (m_CurrentPlayer == 1)
	{
		tankPosition = m_Player2->GetTankPosition();
		tankSize = m_Player2->GetTankSize();
	}

	if (m_CurrentPlayer == 2)
	{
		tankPosition = m_Player1->GetTankPosition();
		tankSize = m_Player1->GetTankSize();
	}

	double tankWidth = tankSize.GetX();
	double tankHeight = tankSize.GetY();
	double tankX = tankPosition.GetX() - tankWidth / 2;
	double tankY = tankPosition.GetY() - 0.01;

	// Projectile
	Vector2f objectPostion = m_Projectile->GetPosition();
	Vector2f objectSize = m_Projectile->GetSize();
	Vector2f BlastRadious = Vector2f(5, 5); //TODO Link to Projectile Blast Radious
	double a = (Coords(objectPostion.GetX() - objectSize.GetX() / 2));
	double b = (Coords(objectPostion.GetY() - objectSize.GetY() / 2));
	double c = (Coords(objectPostion.GetX() + objectSize.GetX() / 2));
	double d = (Coords(objectPostion.GetY() + objectSize.GetY() / 2));
	double projectileW = objectSize.GetX();
	double projectileH = objectSize.GetY();
	double projectileX = objectPostion.GetX() - projectileW / 2;
	double projectileY = objectPostion.GetY();


	// Out of map
	if (a < 0 || b < 0 || c < 0 || d < 0)
	{
		return false;
	}

	// Is projectile in map range
	if (a < m_MapSizeN && c < m_MapSizeN)
	{
		// Tank collision with projectile
		if ((tankX < projectileX + projectileW) && (projectileX < tankX + tankWidth)
			&& (tankY < projectileY + projectileH) && (projectileY < tankY + tankHeight))
		{
			// Friendly fire is off - cheking if enemy tank is hitted 
			if (m_CurrentPlayer == 1)
			{
				if (!(GameManager::PlayerHitted(2)))
				{
					m_Player2->setHealth(m_Player2->getHealth() - 10);
					GameManager::SetPlayerHitted(true, 2);
				}
			}
			else if (m_CurrentPlayer == 2)
			{
				if (!(GameManager::PlayerHitted(1)))
				{
					m_Player1->setHealth(m_Player1->getHealth() - 10);
					GameManager::SetPlayerHitted(true, 1);
				}
			}

			// Setting blast radious for destroying map under tank
			if (a > BlastRadious.GetX())
			{
				a -= BlastRadious.GetX();
			}
			if (b > BlastRadious.GetY())
			{
				b -= BlastRadious.GetY();
			}
			if (c < m_MapSizeN - BlastRadious.GetX())
			{
				c += BlastRadious.GetX();
			}
			if (d > m_MapSizeM - BlastRadious.GetY())
			{
				d += BlastRadious.GetY();
			}

			// Destroying map and projectile
			m_Map->DestroyTerrain(a, b, c, d);
			delete m_Projectile;
			m_Projectile = nullptr;

			// Cheking if any of them are dead to call GameOver
			if (m_Player1->amIDead())
			{
				m_Player1->setHealth(0);
				GameOver();
				return true;
			}
			if (m_Player2->amIDead())
			{
				m_Player2->setHealth(0);
				GameOver();
				return true;
			}

			if (m_CurrentPlayer == 1)
			{
				GameManager::SetPlayerHitted(false, 2);
			}
			else if (m_CurrentPlayer == 2)
			{
				GameManager::SetPlayerHitted(false, 1);
			}

			GameManager::ChangePlayer();
			return true;
		} // End tank collision with projectile

		// Projectile collision with map
		if (m_Outline->at(a) >= b || m_Outline->at(c) >= d)
		{
			if (a > BlastRadious.GetX())
			{
				a -= BlastRadious.GetX();
			}
			if (b > BlastRadious.GetY())
			{
				b -= BlastRadious.GetY();
			}
			if (c < m_MapSizeN - BlastRadious.GetX())
			{
				c += BlastRadious.GetX();
			}
			if (d > m_MapSizeM - BlastRadious.GetY())
			{
				d += BlastRadious.GetY();
			}

			// Settings for cheking if projectile miss the tank, but blast radious hit it
			projectileW = projectileW + lenghtInMap(15);
			projectileH = projectileH + lenghtInMap(20);
			projectileX = projectileX - projectileW / 2;
			projectileY = projectileY - projectileH / 2;

			// Tank collision with blast radious
			if ((tankX < projectileX + projectileW) && (projectileX < tankX + tankWidth)
				&& (tankY < projectileY + projectileH) && (projectileY < tankY + tankHeight))
			{
				if (m_CurrentPlayer == 1)
				{
					if (!(GameManager::PlayerHitted(2)))
					{
						m_Player2->setHealth(m_Player2->getHealth() - 5);
						GameManager::SetPlayerHitted(true, 2);
					}
				}
				else if (m_CurrentPlayer == 2)
				{
					if (!(GameManager::PlayerHitted(1)))
					{
						m_Player1->setHealth(m_Player1->getHealth() - 5);
						GameManager::SetPlayerHitted(true, 1);
					}
				}
			}

			m_Map->DestroyTerrain(a, b, c, d);
			delete m_Projectile;
			m_Projectile = nullptr;

			if (m_Player1->amIDead())
			{
				m_Player1->setHealth(0);
				GameOver();
			}

			if (m_Player2->amIDead())
			{
				m_Player2->setHealth(0);
				GameOver();
			}

			if (m_CurrentPlayer == 1)
			{
				GameManager::SetPlayerHitted(false, 2);
			}

			else if (m_CurrentPlayer == 2)
			{
				GameManager::SetPlayerHitted(false, 1);
			}

			GameManager::ChangePlayer();
			return true;
		}
	}
	return false;
}

Vector2f GameManager::GetPlayerPosition(int player)
{
	return GetPlayer(player)->GetTankPosition();
}

Vector2f GameManager::GetPlayerSize(int player)
{
	return GetPlayer(player)->GetTankSize();
}

unsigned GameManager::GetPlayerScore(int player)
{
	return GetPlayer(player)->getScore();
}

void GameManager::AddScoreToPlayer(int player, unsigned score)
{
	return GetPlayer(player)->setScore(score);
}

Vector2f GameManager::MovePlayer(int player)
{
	if (player == 1)
	{
		if (m_CurrentPlayer == 1)
		{
			if (m_possibleMoves > 0 && m_Player1->getCanFire())
			{
				if (m_nextMove == LEFT)
				{
					Vector2f dv = m_Map->FindNextMove(m_Player1->getPlayerTank(), -1, m_Outline); //Mocked tank size
					m_Player1->moveMyTank(dv);
				}
				else if (m_nextMove == RIGHT)
				{
					Vector2f dv = m_Map->FindNextMove(m_Player1->getPlayerTank(), 1, m_Outline); //Mocked tank size
					m_Player1->moveMyTank(dv);
				}

				m_possibleMoves--;
			}

			return m_Player1->GetTankPosition();
		}

		else
		{
			return m_Player1->GetTankPosition();
		}
	}
	else if (player == 2)
	{
		if (m_CurrentPlayer == 2)
		{
			if (m_possibleMoves > 0 && m_Player2->getCanFire())
			{
				if (m_nextMove == LEFT)
				{
					Vector2f dv = m_Map->FindNextMove(m_Player2->getPlayerTank(), -1, m_Outline); //Mocked tank size
					m_Player2->moveMyTank(dv);
				}
				else if (m_nextMove == RIGHT)
				{
					Vector2f dv = m_Map->FindNextMove(m_Player2->getPlayerTank(), 1, m_Outline); //Mocked tank size
					m_Player2->moveMyTank(dv);
				}
				m_possibleMoves--;
			}

			return m_Player2->GetTankPosition();
		}
		else
		{
			return m_Player2->GetTankPosition();
		}
	}

	else
		return m_Player2->GetTankPosition();
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
	double xScale = -1.0f + (mousePosition.GetX() / (1540.0f/2.0f));
	double yScale = 1.0f - (mousePosition.GetY() / (1200.0f/2.0f));

	Vector2f playerPosition = GetPlayerPosition(m_CurrentPlayer);
	double radianAngle = 0.0f;
	if (m_CurrentPlayer == 1)
	{
		radianAngle = m_Player1->getAngle();
	}
	else
	{
		radianAngle = m_Player2->getAngle();
	}
	double k = (yScale - playerPosition.GetY()) / (xScale - playerPosition.GetX());
	if (k <= 0 && yScale > (playerPosition.GetY() + GameManager::GetPlayerSize(m_CurrentPlayer).GetY())) {
		radianAngle = std::atan(k);
		if (radianAngle < 0)
		{
			radianAngle += PI;
		}

		double power = lineLength(playerPosition.GetX(), playerPosition.GetY(), xScale, yScale);
		if (power > 1.0)
		{
			power = 1.0;
		}

		if (m_CurrentPlayer == 1)
		{
			m_Player1->setAngle(radianAngle);
			m_Player1->setFirePower(power);
		}
		else
		{
			m_Player2->setAngle(radianAngle);
			m_Player2->setFirePower(power);
		}
		OutputDebugStringA(std::to_string(power).c_str());
	}
	else if(k>=0 && yScale > (playerPosition.GetY() + GameManager::GetPlayerSize(m_CurrentPlayer).GetY()))
	{
		radianAngle = std::atan(k);

		double power = lineLength(playerPosition.GetX(), playerPosition.GetY(), xScale, yScale);
		if (power > 1.0)
		{
			power = 1.0;
		}

		if (m_CurrentPlayer == 1)
		{
			m_Player1->setAngle(radianAngle);
			m_Player1->setFirePower(power);
		}
		else
		{
			m_Player2->setAngle(radianAngle);
			m_Player2->setFirePower(power);
		}

	}
#ifdef DEBUG
	OutputDebugStringA(std::to_string(m_Player1->getAngle()).c_str());
	OutputDebugStringA("    ");
#endif
}

double GameManager::GetPlayerPower()
{
	return GetPlayerOnTurn()->getFirePower();
}

std::string GameManager::GetPlayerName(int num)
{
	return GetPlayer(num)->getPlayerName();
}

double GameManager::GetPlayerAngle()
{
	return GetPlayerOnTurn()->getAngle();
}

int GameManager::GetPlayerHealth(int player)
{
	return GetPlayer(player)->getHealth();
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
		if (m_Player1->getCanFire())
		{
			m_Player1->setCanFire(false);
			m_Player2->setCanFire(true);
			m_Projectile = m_Player1->fireInTheHole();
		}
	}
	else
	{
		if (m_Player2->getCanFire())
		{
			m_Player2->setCanFire(false);
			m_Player1->setCanFire(true);
			m_Projectile = m_Player2->fireInTheHole();
		}
	}
}

void GameManager::ChangePlayer()
{
	GameManager::SetMovesDefault();
	if (m_CurrentPlayer == 1)
	{
		m_CurrentPlayer = 2;
	}
	else
	{
		m_CurrentPlayer = 1;
	}
}

int GameManager::GetCurrentPlayer()
{
	return m_CurrentPlayer;
}


void GameManager::UpdateTerrainOutline()
{
	for (unsigned i = 0u; i < m_MapSizeN; i++)
	{
		for (unsigned j = 0u; j < m_MapSizeM; j++)
		{
			if (!m_Map->GetElement(i, j))
			{
				if (j == 0)
				{
					break;
				}
				(*m_Outline)[i] = j - 1;
				break;
			}
		}
	}
	Fall(1);
	Fall(2);
}

void GameManager::AllowMove(PlayerMovement side)
{
	m_nextMove = side;
	if (m_Projectile == nullptr)
	{
		m_possibleMoves = 10;
	}
	else
	{
		m_possibleMoves = 0;
	}
}

bool GameManager::CanFire()
{
	if (m_possibleMoves == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int GameManager::MovesLeft()
{
	return GetPlayerOnTurn()->getMoves();
}

void GameManager::ReduceMoves()
{
	return GetPlayerOnTurn()->setMoves(GetPlayerOnTurn()->getMoves() - 1);
}

void GameManager::SetMovesDefault()
{
	return GetPlayerOnTurn()->setMoves(3);
}

void GameManager::GameOver()
{
	m_GameOverIndicator = true;
}

bool GameManager::getGameIndicator()
{
	return m_GameOverIndicator;
}

void GameManager::ShutDown()
{
	if (m_Player1 != nullptr)
	{
		delete m_Player1;
		m_Player1 = nullptr;
	}

	if (m_Player2 != nullptr)
	{
		delete m_Player2;
		m_Player2 = nullptr;
	}

	if (m_Map != nullptr)
	{
		delete m_Map;
		m_Map = nullptr;
	}

	if (m_Outline != nullptr)
	{
		delete m_Outline;
		m_Outline = nullptr;
	}
}

void GameManager::RestartGameState()
{
	ShutDown();
	m_GameOverIndicator = false;

	m_CurrentPlayer = 1;
	m_Player1 = new Player("Player1", 1);
	m_Player2 = new Player("Player2", 2);

	m_Map = new Terrain(200);
	m_MapSizeN = 0;
	m_MapSizeM = 0;

	m_nextMove = INVALID;
	m_possibleMoves = 0;

	m_Projectile = nullptr;
	m_Outline = nullptr;

	m_Player1->setCanFire(true);
	m_Player2->setCanFire(false);

	int mapRandom = std::rand() % 3 + 1;
	switch (mapRandom)
	{
	case 1:
		m_Map->FillTerrain(TerrainType::Hill);
		break;
	case 2:
		m_Map->FillTerrain(TerrainType::Hill);
		break;
	case 3:
		m_Map->FillTerrain(TerrainType::Hill);
		break;
	default:
		m_Map->FillTerrain(TerrainType::Random);
		break;
	}

	m_MapSizeN = m_Map->GetN();
	m_MapSizeM = m_Map->GetM();
	m_Outline = new std::vector<unsigned>(m_MapSizeN);
	UpdateTerrainOutline();
	Fall(2);
	Fall(1);
}

void GameManager::SetWindowSize(Vector2f size)
{
	m_WindowHeight = size.GetY();
	m_WindowWidth = size.GetX();
}

void GameManager::Fall(int i)
{
	Vector2f tankPosition = GetPlayerPosition(i);
	Vector2f tankSize = GetPlayerSize(i);
	double leftX = std::ceil((Coords(tankPosition.GetX() - tankSize.GetX() / 2))) + 1;
	double y = (Coords(tankPosition.GetY())) - 1;
	double rightX = std::ceil(Coords(tankPosition.GetX() + tankSize.GetX() / 6)) - 1;

	bool canFall = true;
	for (unsigned i = leftX; i <= rightX; i++)
	{
		if (m_Map->GetElement(i,y))
		{
			canFall = false;
			break;
		}
	}

	if (canFall)
	{
		unsigned jFirst = 0;
		unsigned iFirst = 0;
		for (unsigned i = leftX; i <= rightX; i++)
		{
			if (m_Outline->at(i) >= jFirst)
			{
				jFirst = m_Outline->at(i);
				iFirst = i;
			}
		}
		//double angle = std::atan((iFirst*1.0f - iSecond * 1.0f) / (jFirst*1.0f - jSecond * 1.0f));
		//playerTank.setTankDrawAngle(angle);
		GetPlayer(i)->moveMyTank(Vector2f(0, -0.01*(y - jFirst)));
	}
	else
	{
		//return Vector2f(-0.01, 0);
	}



}



//Initializing of static class member
int GameManager::m_CurrentPlayer = 1;
Player* GameManager::m_Player1 = nullptr;
Player* GameManager::m_Player2 = nullptr;

bool GameManager::m_GameRepeat = false;
bool GameManager::m_GameOverIndicator = false;

Terrain* GameManager::m_Map = nullptr;
int GameManager::m_MapSizeN = 0;
int GameManager::m_MapSizeM = 0;
std::vector<unsigned>* GameManager::m_Outline = nullptr;

PlayerMovement GameManager::m_nextMove = INVALID;
int GameManager::m_possibleMoves = 0;

Weapon * GameManager::m_Projectile = nullptr;

int GameManager::m_WindowHeight = 1;
int GameManager::m_WindowWidth = 1;
