#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Tank.h"
#include "Vector2f.h"
#include "Weapon.h"

class Player
{
private:
	Tank *m_playerTank;
	int m_health = 99; // in range [0.0, 1.0]

	double m_firePower = 0.8; // in range [0.0, 1.0]
	double m_angle = 0;

	std::string m_playerName;
	int m_uniqueID;
	bool m_canFire = false;
	int m_movesLeft = 3;

public:
	Player();
	Player(Tank *t, std::string name, const int id);
	Player(std::string name, const int id);
	~Player();

	// Getters
	int getHealth();
	double getFirePower();
	double getAngle();
	std::string getPlayerName();
	Tank &getPlayerTank();
	const int getID();
	Vector2f GetTankPosition();
	bool getCanFire();
	int getMoves();


	// Setters
	void setHealth(int hp);
	void setFirePower(double power);
	void setAngle(double angle);
	void setCanFire(bool x);
	void setMoves(int mov);


	void rotateTurret(double forAngle);
	void moveMyTank(Vector2f dv);
	bool amDead();
	Weapon* fireInTheHole();
}; // ClASS PLAYER


#endif // END OF PLAYER_H

