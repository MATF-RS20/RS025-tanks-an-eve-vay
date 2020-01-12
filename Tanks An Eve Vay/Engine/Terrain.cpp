#include "Object.h"
#include "Terrain.h"

#define Coords(x) ((m_N*(x))/2.0f+(m_N/2.0f))
#define MoveLength (-1+ 0.005)

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

Vector2f Terrain::FindNextMove(const Vector2f position,const Vector2f size, int move)
{
	double gridCells = Coords(MoveLength);
	double positionX = Coords(position.GetX());
	double positionY = Coords(position.GetY());
	double a = (Coords(position.GetX() - size.GetX() / 2));
	double b = (Coords(position.GetY() + size.GetY() / 2));
	gridCells = std::ceil(gridCells);

	if (gridCells <= 0)
	{
		return Vector2f(0,0);
	}
	else if (move == 1)
	{
		if (!m_TerrainMatrix[a + 1][b])
		{
			return Vector2f(0.005, 0);
		}
		else
		{
			return Vector2f();
		}
	}
	else if (move == -1)
	{
		if (!m_TerrainMatrix[a - 1][b])
		{
			return Vector2f(-0.005, 0);
		}
		else
		{
			return Vector2f();
		}
	}
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
		unsigned limit = m_N / 2;
		for (unsigned i = 0; i < limit;i++)
		{
			for (unsigned j = 0; j < m_M; j++)
			{
				//if (j < limit)
				//{
					m_TerrainMatrix[i][j] = true;
				//}
			}
		}
		break;
	}
	case TerrainType::Hill:
	{
		unsigned hillStart = m_N / 4;
		for (unsigned i = 0u; i < m_N; i++)
		{
			for (unsigned j = 0u; j < m_M / 5; j++)
			{
				m_TerrainMatrix[i][j] = true;
			}

		}
		for (unsigned i = hillStart; i < 3 * m_N / 4; ++i)
		{
			for (unsigned j = m_M / 5; j < 3 * m_N / 4; j++)
			{
				if (i + j <= 3 * m_N / 4)
				{
					m_TerrainMatrix[i][j] = true;
				}
			}
		}
		break;
	}
	case TerrainType::Random:
	{
		unsigned limit = m_N / 2;
		for (unsigned i = 0; i < m_N; i++)
		{
			for (unsigned j = 0; j < m_M; j++)
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
	}
	default:
		break;
	}
}

Terrain::~Terrain()
{
}
