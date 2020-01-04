#include "Object.h"
#include "Terrain.h"


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

void Terrain::PrintTerrain()
{
	for (unsigned i = 0; i < m_N; i++)
	{
		for (unsigned j = 0; j < m_M; j++)
		{
			if (m_TerrainMatrix[i][j])
			{
				std::cout << m_TerrainMatrix[i][j]<< std::endl;
			}
		}
	}
}

Terrain::~Terrain()
{
}

std::vector<Vector2f> Terrain::FindAllAffectedGridCell(const Object * object)
{
	return std::vector<Vector2f>();
}
bool Terrain::DestroyTerrain(const Object * object)
{
	bool hasAffectTerrain = false;
	std::vector<Vector2f> affectedGridCell = FindAllAffectedGridCell(object);
	auto it = affectedGridCell.cbegin();
	while (it != affectedGridCell.cend())
	{
		unsigned i = it->GetX();
		unsigned j = it->GetY();
		m_TerrainMatrix[i][j] = false;
		hasAffectTerrain = true;
	}
	return hasAffectTerrain;
}

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
	default:
		break;
	}
}
