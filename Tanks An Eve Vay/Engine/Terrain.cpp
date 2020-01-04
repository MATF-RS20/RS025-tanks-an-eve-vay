#include "Terrain.h"
#include "Object.h"
Map::Map(MapType type)
{
	switch (type)
	{
	case MapType::Flat:
		for (unsigned i = 0; i < 30; i++)
		{
			for (unsigned j = 0; j < 30; j++)
			{
				if (j > 15)
				{
					m_Grid[i][j] = nullptr;
				}
				else
				{
					m_Grid[i][j] = new Object();
				}
			}
		}
		break;
	default:
		break;
	}
}

void Map::PrintMap()
{
	for (unsigned i = 0; i < 30; i++)
	{
		for (unsigned j = 0; j < 30; j++)
		{
			if (m_Grid[i][j] != nullptr)
			{
				//std::cout << m_Grid[i][j]->ToString() << std::endl;
			}
		}
	}
}

Map::~Map()
{
	for (unsigned i = 0; i < 30; i++)
	{
		for (unsigned j = 0; j < 30; j++)
		{
			if (m_Grid[i][j] != nullptr)
			{
				delete m_Grid[i][j];
			}
		}
	}

}

std::vector<Vector2f> Map::FindAllAffectedGridCell(const Object * object)
{
	return std::vector<Vector2f>();
}

void Map::DestroyMap(const Object * object)
{
	std::vector<Vector2f> affectedGridCell = FindAllAffectedGridCell(object);
	auto it = affectedGridCell.cbegin();
	while (it != affectedGridCell.cend())
	{
		unsigned i = it->GetX();
		unsigned j = it->GetY();
		m_Grid[i][j] = nullptr;
	}
}
