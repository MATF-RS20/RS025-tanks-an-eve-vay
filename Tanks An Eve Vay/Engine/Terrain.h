#pragma once

#include "Object.h"
#include "Vector2f.h"
#include <vector>

enum TerrainType
{
	Flat,
	Hill,
	Sin,
	Random
};

class Terrain
{
public:
	Terrain(unsigned  n,unsigned m);
	Terrain(unsigned n);

	unsigned GetN() const;
	unsigned GetM() const;
	bool GetElement(unsigned i, unsigned j);
	//bool DetectCoalision(const Object* object);

	void FillTerrain(TerrainType type);
	void PrintTerrain();
	bool DestroyTerrain(const Object* object);

	~Terrain();
protected:
	std::vector<Vector2f> FindAllAffectedGridCell(const Object *object);
private:
	unsigned m_N;
	unsigned m_M;
	std::vector<std::vector<bool>> m_TerrainMatrix;

	Terrain(const Terrain& m) = delete;
	Terrain& operator=(const Terrain& p) = delete;
};

