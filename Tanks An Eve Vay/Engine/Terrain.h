#pragma once

#include "Object.h"
#include "Tank.h"
#include "Vector2f.h"
#include <vector>
#include <map>

enum TerrainType
{
	Flat,
	Hill,
	Pyramid,
	Greenland,
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

	void FillTerrain(TerrainType type);

	void DestroyTerrain(unsigned bottomLeftX,unsigned bottomLeftY,unsigned topRightX,unsigned topRightY);
	Vector2f FindNextMove(Tank& playerTank, int move,std::vector<unsigned>* outline);
	~Terrain();
protected:
	void UpdateTerrainState(unsigned bottomLeftX, unsigned bottomLeftY, unsigned topRightX);
private:
	std::vector<std::vector<bool>> m_TerrainMatrix;

	unsigned m_N;
	unsigned m_M;

	Terrain(const Terrain& m) = delete;
	Terrain& operator=(const Terrain& p) = delete;
};

