#include "Player.h"


Player::Player() {};

Player::Player(Tank *t, std::string name, const int id) :
	m_playerTank(t),
	m_playerName(name),
	m_uniqueID(id)
{}

Player::Player(std::string name, const int id) :
	m_playerName(name),
	m_uniqueID(id)
{
	if (id == 1)
		m_playerTank = new Tank(Vector2f(-0.5f, 0.0f), Vector2f(0.1f, 0.1f)); //predefinisao sam poziciju radi provere

	else if (id == 2)
		m_playerTank = new Tank(Vector2f(0.5f, 0.0f), Vector2f(0.1f, 0.1f)); //predefinisao sam poziciju radi provere
}

Player::~Player()
{
	delete m_playerTank;
}


double Player::getHealth() { return m_health; }
double Player::getFirePower() { return m_firePower; }
double Player::getAngle() { return m_angle; }
std::string Player::getPlayerName() { return m_playerName; }
Tank& Player::getPlayerTank() { return *m_playerTank; }
const int Player::getID() { return m_uniqueID; }
Vector2f Player::GetTankPosition() { return (*this).getPlayerTank().GetPosition(); }

bool Player::getCanFire()
{
	return m_canFire;
}

int Player::getMoves()
{
	return m_movesLeft;
}

void Player::setHealth(double hp) { m_health = hp; }
void Player::setFirePower(double power) { m_firePower = power; }
void Player::setAngle(double angle) { m_angle = angle; }

void Player::setCanFire(bool x)
{
	m_canFire = x;
}

void Player::setMoves(int mov)
{
	m_movesLeft = mov;
}

bool Player::amDead()
{
	return !(m_health); // False means Player is not dead ( yet :D )
}
// This functions should call some function on m_playerTank(Tank class)
void Player::rotateTurret(double forAngle) {}
void Player::moveMyTank(Vector2f dv)
{
	(*this).getPlayerTank().moveTank(dv);
}

// This function should call something with weapons and map?
Weapon* Player::fireInTheHole()
{
	Weapon *projectile = new Weapon(m_playerTank->GetPosition(),
		Vector2f(0.01f, 0.01f),
		m_angle, m_firePower);

	return projectile;
}
