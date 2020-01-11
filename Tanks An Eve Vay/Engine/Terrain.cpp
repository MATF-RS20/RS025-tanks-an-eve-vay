#include "Object.h"
#include "Terrain.h"

#define Coords(x) ((m_N*(x))/2.0f+(m_N/2.0f))

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
		for (unsigned i = 0; i < m_N ;i++)
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
	default:
		break;
	}
}

Terrain::~Terrain()
{
}
