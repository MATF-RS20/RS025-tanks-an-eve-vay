#include "Player.h"


Player::Player() {};

Player::Player(Tank *t, std::string name, const int id)
  : m_playerTank(t), m_playerName(name), m_uniqueID(id)
{
}

Player::Player(std::string name, const int id) :
	m_playerName(name),
	m_uniqueID(id)
{
	if (id == 1)
		m_playerTank = new Tank(Vector2f(-0.8f, 0.9f), Vector2f(0.1f, 0.1f)); //predefinisao sam poziciju radi provere

	else if (id == 2)
		m_playerTank = new Tank(Vector2f(0.8f, 0.9f), Vector2f(0.1f, 0.1f)); //predefinisao sam poziciju radi provere
}

Player::~Player()
{
	delete m_playerTank;
}

int Player::getHealth() { return m_health; }
double Player::getFirePower() { return m_firePower; }
double Player::getAngle() { return m_angle; }
std::string Player::getPlayerName() { return m_playerName; }
Tank& Player::getPlayerTank() { return *m_playerTank; }
const int Player::getID() { return m_uniqueID; }
Vector2f Player::GetTankPosition() { return (*this).getPlayerTank().GetPosition(); }
Vector2f Player::GetTankSize() { return (*this).getPlayerTank().GetSize(); }

bool Player::getCanFire()
{
	return m_canFire;
}

int Player::getMoves()
{
	return m_movesLeft;
}

double Player::getTankDrawAngle() const
{
	return m_playerTank->getTankDrawAngle();
}
bool Player::getHitted()
{
	return m_hitted;
}
void Player::setHealth(int hp) { m_health = hp; }
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

void Player::setTankDrawAngle(const double newAngle)
{
	m_playerTank->setTankDrawAngle(newAngle);
}

void Player::setHitted(bool x)
{
	m_hitted = x;
}

void Player::setScore(int x)
{
	m_score = x;
}

unsigned Player::getScore()
{
	return m_score;
}

bool Player::amIDead()
{
	if (m_health <= 0)
	{
		return true;
	}
	else
		return false;
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
