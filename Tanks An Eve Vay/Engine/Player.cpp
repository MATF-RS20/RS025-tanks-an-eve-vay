#include "Player.h"

Player::Player()
{

}

Player::Player(Tank *t, std::string name, const int id) : m_playerTank(t),
m_playerName(name),
m_uniqueID(id)
{}

double Player::getHealth() { return m_health; }
double Player::getFirePower() { return m_firePower; }
double Player::getAngle() { return m_angle; }
std::string Player::getPlayerName() { return m_playerName; }
Tank& Player::getPlayerTank() { return *m_playerTank; }
const int Player::getID() { return m_uniqueID; }

void Player::setHealth(double hp) { m_health = hp; }
void Player::setFirePower(double power) { m_firePower = power; }
void Player::setAngle(double angle) { m_angle = angle; }

bool Player::amDead()
{
	return !(m_health); // False means Player is not dead ( yet :D )
}
// This functions should call some function on m_playerTank(Tank class)
void rotateTurret(double forAngle) {}
void moveTank(int direction) {}

// This function should call something with weapons and map?
void fireInTheHole() {}
