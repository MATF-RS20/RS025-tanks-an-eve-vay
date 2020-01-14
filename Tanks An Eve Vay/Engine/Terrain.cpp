#include "Object.h"
#include "Terrain.h"
#include "ErrorLogger.h"

#define Coords(x) ((m_N*(x))/2.0f+(m_N/2.0f))
#define MoveLength (-1+ 0.005)
#define CanCross (2)

#define PI (atan(1.0f)*4.0f)

Terrain::Terrain(unsigned n, unsigned m)
	:m_N(n), m_M(m)
{
	m_TerrainMatrix = std::vector<std::vector<bool>>(n, std::vector<bool> ( m, false));
}

Terrain::Terrain(unsigned n)
	:m_N(n), m_M(n)
{
	m_TerrainMatrix = std::vector<std::vector<bool>>(n, std::vector<bool>(n, false));
}

unsigned Terrain::GetN() const
{
	return m_N;
}

unsigned Terrain::GetM() const
{
	return m_M;
}

bool Terrain::GetElement(unsigned i, unsigned j)
{
	return m_TerrainMatrix[i][j];
}

void Terrain::DestroyTerrain(unsigned bottomLeftX, unsigned bottomLeftY, unsigned topRightX, unsigned topRightY)
{
	for (unsigned i = bottomLeftX; i <= topRightX; i++)
	{
		for (unsigned j = bottomLeftY;j <= topRightY; j++)
		{
			m_TerrainMatrix[i][j] = false;
		}
	}
	UpdateTerrainState(bottomLeftX,bottomLeftY,topRightX);
}
Vector2f Terrain::FindNextMove(Tank & playerTank, int move, std::vector<unsigned>* outline)
{
	if (move != 1 && move != -1)
	{
		return Vector2f(0, 0);
	}

	double gridCells = std::ceil(Coords(MoveLength));
	if (gridCells <= 0)
	{
		ErrorLogger::Log("gredCells < 0");
		return Vector2f(0, 0);
	}

	//double a, b;
	Vector2f tankPosition = playerTank.GetPosition();
	Vector2f tankSize = playerTank.GetSize();
	
	if (move == 1)
	{
		double leftX = std::ceil((Coords(tankPosition.GetX() - tankSize.GetX() / 2))) + 1;
		double y = (Coords(tankPosition.GetY()))-1;
		double rightX = std::ceil(Coords(tankPosition.GetX() + tankSize.GetX() / 6)) - 1;

		unsigned mapY = outline->at(rightX + 1);
		int dy = mapY - y;

		if (dy == 0)
		{
			return Vector2f(0.01, 0);
		}
		if (dy == 1)
		{
			return Vector2f(0.01, 0.01);
		}
		if (dy > 1)
		{
			return Vector2f();
		}

		if (dy < 0)
		{
			bool canFall = true;
			for (unsigned i = leftX; i <= rightX; i++)
			{
				if (m_TerrainMatrix[i][y])
				{
					canFall = false;
					break;
				}
			}

			if (canFall)
			{
				unsigned jFirst = 0;
				unsigned iFirst = 0;
				for (unsigned i = leftX; i < rightX; i++)
				{
					if (outline->at(i) >= jFirst)
					{
						jFirst = outline->at(i);
						iFirst = i;
					}
				}
				unsigned iSecond = -1;
				unsigned jSecond = -1;
				for (unsigned i = leftX; i < rightX; i++)
				{
					if (i == iFirst)
					{
						continue;
					}
					if (outline->at(i) >= jFirst)
					{
						jSecond = outline->at(i);
						iSecond = i;
					}
				}
				double angle = std::atan((iFirst*1.0f - iSecond*1.0f) / (jFirst*1.0f - jSecond * 1.0f));
				//playerTank.setTankDrawAngle(angle);
				return Vector2f(0, -0.01*(y - jFirst));
			}
			else
			{
				return Vector2f(0.01, 0);
			}
		}

	}
	else if(move == -1)
	{
		double leftX = std::ceil((Coords(tankPosition.GetX() - tankSize.GetX() / 2)));
		double y = (Coords(tankPosition.GetY())) - 1;
		double rightX = std::ceil(Coords(tankPosition.GetX() + tankSize.GetX() / 6)) - 1;

		unsigned mapY = outline->at(leftX + 1);
		int dy = mapY - y;

		if (dy == 0)
		{
			return Vector2f(-0.01, 0);
		}
		if (dy == 1)
		{
			return Vector2f(-0.01, 0.01);
		}
		if (dy > 1)
		{
			return Vector2f();
		}

		if (dy < 0)
		{
			bool canFall = true;
			for (unsigned i = leftX; i <= rightX; i++)
			{
				if (m_TerrainMatrix[i][y])
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
					if (outline->at(i) >= jFirst)
					{
						jFirst = outline->at(i);
						iFirst = i;
					}
				}
				unsigned iSecond = -1;
				unsigned jSecond = -1;
				for (unsigned i = leftX; i < rightX; i++)
				{
					if (i == iFirst)
					{
						continue;
					}
					if (outline->at(i) >= jFirst)
					{
						jSecond = outline->at(i);
						iSecond = i;
					}
				}
				double angle = std::atan((iFirst*1.0f - iSecond * 1.0f) / (jFirst*1.0f - jSecond * 1.0f));
				//playerTank.setTankDrawAngle(angle);
				return Vector2f(0, -0.01*(y - jFirst));
			}
			else
			{
				return Vector2f(-0.01, 0);
			}
		}
	}

	return Vector2f();
}

void Terrain::UpdateTerrainState(unsigned bottomLeftX,unsigned bottomLeftY, unsigned topRightX)
{
	for (unsigned i = bottomLeftX; i <= topRightX; i++)
	{
		unsigned nextAviable = bottomLeftY;
		for (unsigned j = m_M-1; j > nextAviable; j--)
		{
			if (m_TerrainMatrix[i][j])
			{
				m_TerrainMatrix[i][nextAviable++] = true;
				m_TerrainMatrix[i][j] = false;
			}
		}
	}
}

//TODO map reading from document (better option!)
void Terrain::FillTerrain(TerrainType type)
{
	switch (type)
	{
	case TerrainType::Flat:
	{
		unsigned limit = m_N * 4 / 7;
		for (unsigned i = 0; i < m_N; i++)
		{
			for (unsigned j = 0; j < m_M; j++)
			{
				if (j < limit)
				{
					m_TerrainMatrix[i][j] = true;
				}
			}
		}
		break;
	}
	case TerrainType::Hill:
	{
		unsigned hillStart = m_N / 4;
		for (unsigned i = 0u; i < m_N; i++)
		{
			for (unsigned j = 0u; j < m_M / 2; j++)
			{
				m_TerrainMatrix[i][j] = true;
			}

		}
		for (unsigned i = hillStart; i < m_N * 35 / 36; ++i)
		{
			for (unsigned j = m_M / 2; j < m_N * 35 / 36; j++)
			{
				if (i + j <= m_N * 35 / 36)
				{
					m_TerrainMatrix[i][j] = true;
				}
			}
		}
		break;
	}

	case TerrainType::Pyramid:
	{
		for (unsigned i = 0u; i < m_N; i++)
		{
			for (unsigned j = 0u; j < m_M / 5; j++)
			{
				m_TerrainMatrix[i][j] = true;
			}

		}
		unsigned hillStart = m_N / 6;
		unsigned limit = m_N /2;
		unsigned jot = m_N / 5;

		for (unsigned i = hillStart; i < m_N / 2; ++i)
		{
			for (unsigned j = m_M / 5; j < jot; j++)
			{
				if (j)
				{
					m_TerrainMatrix[i][j] = true;
				}
			}
			jot++;
		}

		for (unsigned i = limit; i < m_N*5/6+2; ++i)
		{
			for (unsigned j = jot-1; j >= m_N/5; j--)
			{
				if (j)
				{
					m_TerrainMatrix[i][j] = true;
				}
			}
			jot--;
		}
		break;

		break;
	}

	case TerrainType::Random:
	{
		unsigned limit = m_N / 2;
		for (unsigned i = 0; i < m_N; i++)
		{
			for (unsigned j = 0; j < m_M-1; j++)
			{
				if (j < limit)
				{
					m_TerrainMatrix[i][j] = true;
				}
				if (i == 80 || i == 81 || i == 82 || i == 83)
				{
					m_TerrainMatrix[i][j] = true;
				}
			}
		}
		limit = m_N / 3;
		for (unsigned j = 0; j < m_M/2; j++)
		{
			m_TerrainMatrix[j][limit] = true;
			m_TerrainMatrix[j][limit+1] = true;
			m_TerrainMatrix[j][limit + 2] = true;
			m_TerrainMatrix[j][limit+3] = true;
			m_TerrainMatrix[limit+4][j] = true;
			m_TerrainMatrix[limit+5][j] = true;
			m_TerrainMatrix[limit + 6][j] = true;
			m_TerrainMatrix[limit + 7][j] = true;
			m_TerrainMatrix[limit + 8][j] = true;
			m_TerrainMatrix[limit + 9][j] = true;
		}

		limit = m_N / 4;
		for (unsigned i = limit; i < limit + 10;i++)
		{
			for (unsigned j = m_N/2; j < i + m_N/2-limit; j++)
			{
				m_TerrainMatrix[i][j] = true;
			}
		}


	}
	default:
		break;
	}
}

Terrain::~Terrain()
{
}
