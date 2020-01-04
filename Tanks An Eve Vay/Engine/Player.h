#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Tank.h"

class Player
{
private:
	Tank *m_playerTank;
	double m_health = 1.0; // in range [0.0, 1.0]
	double m_firePower = 0.3; // in range [0.0, 1.0]
	double m_angle = 180;
	std::string m_playerName;
	int m_uniqueID;

public:

	Player(Tank &t, std::string &name, const int id);

	// Getters
	double getHealth();
	double getFirePower();
	double getAngle();
	std::string getPlayerName();
	Tank &getPlayerTank();
	const int getID();

	// Setters
	void setHealth(double hp);
	void setFirePower(double power);
	void setAngle(double angle);


	void rotateTurret(double forAngle);
	void moveTank(int direction);
	bool amDead();
	void fireInTheHole();
}; // ClASS PLAYER


#endif // END OF PLAYER_H
