#include "GameManager.h"
#include "Player.h"
#include "Tank.h"
#include "Terrain.h"
#include "Weapon.h"

#define Coords(x) ((m_MapSizeN*(x))/2.0f+(m_MapSizeN/2.0f))
#define lineLength(x1,y1,x2,y2) (std::sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)))
#define RealCoords(x) (((x)-m_MapSizeN/2)*(2.0f/m_MapSizeN))
#define lenghtInMap(x) ((((x)-m_MapSizeN/2)*(2.0f/m_MapSizeN))+1.0f)

void GameManager::Initialize()
{
	m_Player1->setCanFire(true);
	m_Map->FillTerrain(TerrainType::Hill);

	m_MapSizeN = m_Map->GetN();
	m_MapSizeM = m_Map->GetM();
	m_Outline = new std::vector<unsigned>(m_MapSizeN);
	UpdateTerrainOutline();
	Fall();
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

double GameManager::GetMovingAngle(int player)
{
	if (player == 1)
	{
		return m_Player1->getTankDrawAngle();
	}
	else if (player == 2)
	{
		return m_Player2->getTankDrawAngle();
	}

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
	if (p == 1)
	{
		return m_Player1->getHitted();
	}

	else if (p == 2)
	{
		return m_Player2->getHitted();
	}
}

void GameManager::SetPlayerHitted(bool hit, int player)
{
	if (player == 1)
	{
		m_Player1->setHitted(hit);
	}

	else if (player == 2)
	{
		m_Player2->setHitted(hit);
	}
}


bool GameManager::CheckCollision()
{
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
	double tankX = tankPosition.GetX() - tankWidth/2;
	double tankY = tankPosition.GetY() - 0.01;

	
	Vector2f objectPostion = m_Projectile->GetPosition();
	Vector2f objectSize = m_Projectile->GetSize();
	Vector2f BlastRadious = Vector2f(5, 5); //TODO Link to Projectile Blast Radious
	double a = (Coords(objectPostion.GetX() - objectSize.GetX() / 2));
	double b = (Coords(objectPostion.GetY() - objectSize.GetY() / 2));
	double c = (Coords(objectPostion.GetX() + objectSize.GetX() / 2));
	double d = (Coords(objectPostion.GetY() + objectSize.GetY() / 2));

	
	double aMaloW = objectSize.GetX();
	double aMaloH = objectSize.GetY();
	double aMaloX = objectPostion.GetX() - aMaloW/2;
	double aMaloY = objectPostion.GetY();


	if (a < 0 || b < 0 || c < 0 || d < 0 )
	{
		return false;
	}
	if (a < m_MapSizeN && c < m_MapSizeN)
	{
		// Tank collision with projectile
		if ((tankX < aMaloX + aMaloW) && (aMaloX < tankX + tankWidth)
			&& (tankY < aMaloY + aMaloH) && (aMaloY < tankY + tankHeight))
		{
			if (m_CurrentPlayer == 1)
			{
				if (!(GameManager::PlayerHitted(2)))
				{
					m_Player2->setHealth(m_Player2->getHealth() - 10);
					if (m_Player2->amIDead())
					{
						// ovde bi trebalo oslobadjati sve sto je neophodno dok se program ne ugasi
						//exit(0);
					}
					GameManager::SetPlayerHitted(true, 2);
				}
			}

			else if (m_CurrentPlayer == 2)
			{
				if (!(GameManager::PlayerHitted(1)))
				{
					m_Player1->setHealth(m_Player1->getHealth() - 10);
					if (m_Player1->amIDead())
					{
						// ovde bi trebalo oslobadjati sve sto je neophodno dok se program ne ugasi
						//exit(0);
					}
					GameManager::SetPlayerHitted(true, 1);
				}
			}

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

			m_Map->DestroyTerrain(a, b, c, d);
			delete m_Projectile;
			m_Projectile = nullptr;
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


		} // END of tank collision

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
			if (c < m_MapSizeN -BlastRadious.GetX())
			{
				c += BlastRadious.GetX();
			}
			if (d > m_MapSizeM - BlastRadious.GetY())
			{
				d += BlastRadious.GetY();
			}

			
			aMaloW = aMaloW + lenghtInMap(15);
			aMaloH = aMaloH + lenghtInMap(15);
			aMaloX = aMaloX - aMaloW/2;
			aMaloY = aMaloY - aMaloH/2;


			//TODO: weird things happen when using (NUMBER * 2 / m_mapSizeN) ??? cant implement AOE dmg using blast radious
			if ((tankX < aMaloX + aMaloW) && (aMaloX < tankX + tankWidth)
				&& (tankY < aMaloY + aMaloH) && (aMaloY < tankY + tankHeight))
			{
				if (m_CurrentPlayer == 1)
				{
					if (!(GameManager::PlayerHitted(2)))
					{
						m_Player2->setHealth(m_Player2->getHealth() - 3);
						GameManager::SetPlayerHitted(true, 2);
					}
				}

				else if (m_CurrentPlayer == 2)
				{
					if (!(GameManager::PlayerHitted(1)))
					{
						m_Player1->setHealth(m_Player1->getHealth() - 3);
						GameManager::SetPlayerHitted(true, 1);
					}
				}
			} // END of tank collision


			m_Map->DestroyTerrain(a, b, c, d);
			delete m_Projectile;
			m_Projectile = nullptr;
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

Vector2f GameManager::MovePlayer(int player)
{
	/*if (m_CurrentPlayer == 1)
	{
		if(m_Player1->getCanFire())
			m_Player1->moveMyTank(dv);
	}
	else if (m_CurrentPlayer == 2)
	{
		if (m_Player2->getCanFire())
			m_Player2->moveMyTank(dv);
	}*/

	if (player == 1)
	{
		if (m_CurrentPlayer == 1)
		{
			if (m_possibleMoves > 0 && m_Player1->getCanFire())
			{
				if (m_nextMove == LEFT)
				{
					Vector2f dv = m_Map->FindNextMove(m_Player1->getPlayerTank(), -1,m_Outline); //Mocked tank size
					m_Player1->moveMyTank(dv);
				}
				else if (m_nextMove == RIGHT)
				{
					Vector2f dv = m_Map->FindNextMove(m_Player1->getPlayerTank(), 1,m_Outline); //Mocked tank size
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
		if(m_CurrentPlayer == 2)
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
	double xScale = -1.0f + (mousePosition.GetX() / (1551.0f/2.0f));
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
	if (m_CurrentPlayer == 1)
		return m_Player1->getFirePower();
	else
		return m_Player2->getFirePower();
}

std::string GameManager::GetPlayerName(int num)
{
	if (num == 1)
		return m_Player1->getPlayerName();
	else if (num == 2)
		return m_Player2->getPlayerName();
	else return 0;
}

double GameManager::GetPlayerAngle()
{
	if (m_CurrentPlayer == 1)
		return m_Player1->getAngle();
	else
		return m_Player2->getAngle();
}

int GameManager::GetPlayerHealth(int player)
{
	if (player == 1)
	{
		return m_Player1->getHealth();
	}
	else
	{
		return m_Player2->getHealth();
	}
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
}

void GameManager::AllowMove(PlayerMovement side)
{
	m_nextMove = side;
	if(m_Projectile == nullptr)
		m_possibleMoves = 10;
	else
		m_possibleMoves = 0;
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
	if (m_CurrentPlayer == 1)
	{
		return m_Player1->getMoves();
	}
	else if (m_CurrentPlayer == 2)
	{
		return m_Player2->getMoves();
	}
}

void GameManager::ReduceMoves()
{
	if (m_CurrentPlayer == 1)
	{
		m_Player1->setMoves(m_Player1->getMoves()-1);
	}
	else if (m_CurrentPlayer == 2)
	{
		m_Player2->setMoves(m_Player2->getMoves() - 1);
	}
}

void GameManager::SetMovesDefault()
{
	if (m_CurrentPlayer == 1)
	{
		m_Player1->setMoves(3);
	}
	else if (m_CurrentPlayer == 2)
	{
		m_Player2->setMoves(3);
	}
}

void GameManager::SetWindowSize(Vector2f size)
{
	m_WindowHeight = size.GetY();
	m_WindowWidth = size.GetX();
}

void GameManager::Fall()
{
	Vector2f tankPosition = m_Player2->GetTankPosition();
	Vector2f tankSize = m_Player2->GetTankSize();
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
		m_Player2->moveMyTank(Vector2f(0, -0.01*(y - jFirst)));
	}
	else
	{
		//return Vector2f(-0.01, 0);
	}



}



//Initializing of static class member
int GameManager::m_CurrentPlayer = 1;
Player* GameManager::m_Player1 = new Player("Player1", 1);
Player* GameManager::m_Player2 = new Player("Player2", 2);

Terrain* GameManager::m_Map = new Terrain(200);
int GameManager::m_MapSizeN = 0;
int GameManager::m_MapSizeM = 0;

PlayerMovement GameManager::m_nextMove = INVALID;
int GameManager::m_possibleMoves = 0;

Weapon * GameManager::m_Projectile = nullptr;

std::vector<unsigned>* GameManager::m_Outline = nullptr;

int GameManager::m_WindowHeight = 1;
int GameManager::m_WindowWidth = 1;