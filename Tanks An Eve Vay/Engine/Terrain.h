#pragma once

#include "Object.h"
#include "Vector2f.h"
#include <vector>
enum MapType
{
	Flat = 0,
	Sin = 1,
	Random = 2
};

class Map
{
public:
	Map(MapType type = MapType::Flat);
	bool DetectCoalision(const Object* object);

	void PrintMap();

	~Map();
protected:
	std::vector<Vector2f> FindAllAffectedGridCell(const Object *object);
	void DestroyMap(const Object* object);
private:

	Object* m_Grid[30][30];

	Map(const Map& m) = delete;
	Map& operator=(const Map& p) = delete;
};

