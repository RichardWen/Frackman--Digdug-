#include "Actor.h"
#include "StudentWorld.h"
#include <random>


Actor::Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld *inputWorld)
	:GraphObject(imageID, startX, startY, dir, size, depth)
{
	gameWorld = inputWorld;
	myDir = dir;
	mySize = size;
	isAlive = true;
	xLocation = startX;
	yLocation = startY;
}
int Actor::getX()
{
	return xLocation;
}
void Actor::setX(int inX)
{
	xLocation = inX;
}
int Actor::getY()
{
	return yLocation;
}
void Actor::setY(int inY)
{
	yLocation = inY;
}
bool Actor::wasCollected()
{
	return false;
}

void Actor::setDir(Direction newDir)
{
	myDir = newDir;	
	setDirection(newDir);
}
StudentWorld *Actor::getWorld()
{
	return gameWorld;
}

bool Actor::alive()
{
	return isAlive;
}
void Actor::kill()
{
	isAlive = false;
}
void Actor::display()
{
	setVisible(true);
}
Actor::~Actor()
{
	setVisible(false);
}

Agent::Agent(int imageID, int startX, int startY, int health, Direction dir, StudentWorld *inputWorld)
	:Actor(imageID, startX, startY, dir, 1.0, 0, inputWorld)
{
	setVisible(true);
	hitPoints = health;
}

int Agent::getHealth()
{
	return hitPoints;
}

void Agent::takeDamage(int damage)
{
	hitPoints -= damage;
}
bool Agent::dirtInTheWay(Direction dir)
{
	for (int i = 0; i < 4; i++)
	{
		switch (dir)
		{
		case up:
			if (getWorld()->isOccupied(getX() + i, getY() + 4))
			{
				return true;
			}
			break;
		case down:
			if (getWorld()->isOccupied(getX() + i, getY() - 1))
			{
				return true;
			}
			break;
		case left:
			if (getWorld()->isOccupied(getX() - 1, getY() + i))
			{
				return true;
			}
			break;
		case right:
			if (getWorld()->isOccupied(getX() + 4, getY() + i))
			{
				return true;
			}
			break;
		}
	}
	return false;
}
void Agent::moveDir(Direction dir)
{
	switch (dir)
	{
	case left:
		if (getDir() != left)
		{
			setDir(left);
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				if (getWorld()->isOccupiedType(getX() - 1, getY() + i, TID_BOULDER) || getX() <= 0)
				{
					return;
				}
			}
			setX(getX() - 1);
			moveTo(getX(), getY());
		}
		break;
	case right:
		if (getDir() != right)
		{
			setDir(right);
		}
		else
		{
			for (int i = 0; i <= 3; i++)
			{
				if (getWorld()->isOccupiedType(getX() + 4, getY() + i, TID_BOULDER) || getX() > 55)
				{
					return;
				}
			}
			setX(getX() + 1);
			moveTo(getX(), getY());
		}
		break;
	case up:
		if (getDir() != up)
		{
			setDir(up);
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				if (getWorld()->isOccupiedType(getX() + i, getY() + 4, TID_BOULDER) || getY() + 1 > 60)
				{
					return;
				}
			}
			setY(getY() + 1);
			moveTo(getX(), getY());
		}
		break;
	case down:
		if (getDir() != down)
		{
			setDir(down);
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				if (getWorld()->isOccupiedType(getX() + i, getY() - 1, TID_BOULDER) || getY() - 1 < 0)
				{
					return;
				}
			}
			setY(getY() - 1);
			moveTo(getX(), getY());
		}
		break;
	}
}

FrackMan::FrackMan(StudentWorld *inputWorld)
	:Agent(IID_PLAYER, 30, 60, 10, right, inputWorld)
{
	waterAmmo = 5;
	goldNuggets = 0;
	sonarCharges = 1;
}
void FrackMan::doSomething()
{
	if (getHealth() <= 0)
	{
		kill();
		return;
	}
	int ch;
	if (getWorld()->getKey(ch) == true)
	{
		switch (ch)
		{
		case KEY_PRESS_LEFT:
			moveDir(left);
			break;
		case KEY_PRESS_RIGHT:
			moveDir(right);
			break;
		case KEY_PRESS_UP:
			moveDir(up);
			break;
		case KEY_PRESS_DOWN:
			moveDir(down);
			break;
		case KEY_PRESS_ESCAPE:
			kill();
			break;
		case 'z':
		case 'Z':
			if (sonarCharges > 0)
			{
				getWorld()->sonarCharge(getX(), getY());
				sonarCharges--;
			}
			break; 
		case 'p':
			getWorld()->printMapLog();
			break;
		case KEY_PRESS_TAB:
			if (goldNuggets > 0)
			{
				GoldNugget *g = new GoldNugget(getX(), getY(), getWorld(), false);
				getWorld()->addActor(g);
				goldNuggets--;
			}
			break;
		case KEY_PRESS_SPACE:
			if (waterAmmo > 0)
			{
				Squirt *s = new Squirt(getX(), getY(), getDir(), getWorld());
				getWorld()->addActor(s);
				getWorld()->playSound(SOUND_PLAYER_SQUIRT);
				waterAmmo--;
			}
			break;
		}
	}
}
int FrackMan::getType()
{
	return TID_PLAYER;
}
int FrackMan::getAmmo()
{
	return waterAmmo;
}
void FrackMan::findAmmo()
{
	waterAmmo += 5;
}
int FrackMan::getCharges()
{
	return sonarCharges;
}
void FrackMan::findCharge()
{
	sonarCharges++;
}
int FrackMan::getNuggets()
{
	return goldNuggets;
}
void FrackMan::findNugget()
{
	goldNuggets++;
}
FrackMan::~FrackMan()
{

}


Protestors::Protestors(int imageID, int startX, int startY, StudentWorld* inputWorld)
	:Agent(imageID, startX, startY, 5, left, inputWorld)
{
	restTick = std::max(0,3-getWorld()->getLevel()/4);
	timeToAction = 0;
	timeToShout = 15;
	tickCanMove = 5;
	numInCurrentDir = 5;
	hit = false;
	hitStun = std::max(50, 100 - getWorld()->getLevel() * 10);
	shout = true;
	targetable = true;
	isAnnoyed = false;
	restState = true;
	leaveState = false;
}
bool Protestors::lineOfSight(Direction &dir)
{
	int lowerX = getWorld()->getFrackManX();
	int upperX = getWorld()->getFrackManX() + 3;
	int lowerY = getWorld()->getFrackManY();
	int upperY = getWorld()->getFrackManY() + 3;
	if (getX() <= upperX && getX() >= lowerX)
	{
		if (getY() > lowerY)
		{
			while (lowerY < getY())
			{
				for (int i = 0; i < 4; i++)
				{
					if (dirtInTheWay(down))
					{
						return false;
					}
				}
				lowerY++;
			}
			dir = down;
			return true;
		}
		else
		{
			while (lowerY > getY())
			{
				for (int i = 0; i < 4; i++)
				{
					if (dirtInTheWay(up))
					{
						return false;
					}
				}
				lowerY--;
			}
			dir = up;
			return true;
		}
	}
	else if (getY() <= upperY && getY() >= lowerY)
	{
		if (getX() > lowerX)
		{
			while (lowerX < getX())
			{
				for (int i = 0; i < 4; i++)
				{
					if (dirtInTheWay(left))
					{
						return false;
					}
				}
				lowerX++;
			}
			dir = left;
			return true;
		}
		else
		{
			while (lowerX > getX())
			{
				for (int i = 0; i < 4; i++)
				{
					if (dirtInTheWay(right))
					{
						return false;
					}
				}
				lowerX--;
			}
			dir = right;
			return true;
		}
	}

	return false;
}
bool Protestors::playerInRange()
{
	if (getY() - getWorld()->getFrackManY() <= 4 && getY() - getWorld()->getFrackManY() >= -4
		&& getX() - getWorld()->getFrackManX() <= 4 && getX() - getWorld()->getFrackManX() >= -4)
	{
		return true;
	}
	return false;
}
void Protestors::restStateOperation()
{
	if (timeToAction >= restTick)
	{
		if (restTick == hitStun)
		{
			restTick = 3;
		}
		timeToAction = 0;
		restState = false;
	}
	else
	{
		timeToAction++;
	}
}
void Protestors::leaveMap()
{
	if (getX() == 59 && getY() == 60)
	{
		kill();
		return;
	}
	else
	{
		targetable = false;
		tickCanMove++;
		if (tickCanMove >= 5)
		{
			switch (getWorld()->getLocation(getX(), getY())->getDir())
			{
			case 1:
				if (getDir() != right)
				{
					setDir(right);
				}
				setX(getX() + 1);
				moveTo(getX(), getY());
				break;
			case 2:
				if (getDir() != up)
				{
					setDir(up);
				}
				setY(getY() + 1);
				moveTo(getX(), getY());
				break;
			case 3:
				if (getDir() != left)
				{
					setDir(left);
				}
				setX(getX() - 1);
				moveTo(getX(), getY());
				break;
			case 4:
				if (getDir() != down)
				{
					setDir(down);
				}
				setY(getY() - 1);
				moveTo(getX(), getY());
				break;
			}
			tickCanMove = 0;
			return;
		}
	}
}
void Protestors::attackFrackMan()
{
	timeToShout++;
	if (timeToShout >= 15)
	{
		shout = true;
		timeToShout = 0;
	}
	switch (getDir())
	{
	case up:
		if (getY() - getWorld()->getFrackManY() <= 0 && getY() - getWorld()->getFrackManY() >= -4 && getX() - getWorld()->getFrackManX() <= 4 && getX() - getWorld()->getFrackManX() >= -4 && shout)
		{
			getWorld()->getPlayer()->takeDamage(2);
			shout = false;
			getWorld()->playSound(SOUND_PROTESTER_YELL);
		}
		break;
	case right:
		if (getX() - getWorld()->getFrackManX() <= 0 && getX() - getWorld()->getFrackManX() >= -4 && getY() - getWorld()->getFrackManY() <= 4 && getY() - getWorld()->getFrackManY() >= -4 && shout)
		{
			getWorld()->getPlayer()->takeDamage(2);
			shout = false;
			getWorld()->playSound(SOUND_PROTESTER_YELL);
		}
		break;
	case down:
		if (getY() - getWorld()->getFrackManY() <= 4 && getY() - getWorld()->getFrackManY() >= 0 && getX() - getWorld()->getFrackManX() <= 4 && getX() - getWorld()->getFrackManX() >= -4 && shout)
		{
			getWorld()->getPlayer()->takeDamage(2);
			shout = false;
			getWorld()->playSound(SOUND_PROTESTER_YELL);
		}
		break;
	case left:
		if (getX() - getWorld()->getFrackManX() <= 4 && getX() - getWorld()->getFrackManX() >= 0 && getY() - getWorld()->getFrackManY() <= 4 && getY() - getWorld()->getFrackManY() >= -4 && shout)
		{
			getWorld()->getPlayer()->takeDamage(2);
			shout = false;
			getWorld()->playSound(SOUND_PROTESTER_YELL);
		}
		break;
	}
	restState = true;
}
void Protestors::movement()
{
	Direction facing = getDir();
	if (lineOfSight(facing))
	{
		moveDir(facing);
	}
	else
	{
		numInCurrentDir--;
		if (numInCurrentDir <= 0)
		{
			int newTest = getWorld()->randInt(1, 4);
			Direction newDir;
			bool isValid = false;
			switch (newTest)
			{
			case 1:
				isValid = !dirtInTheWay(up) && getY() + 4 < 60;
				newDir = up;
				break;
			case 2:

				isValid = !dirtInTheWay(down) && getY() - 1 >= 0;
				newDir = down;
				break;
			case 3:
				isValid = !dirtInTheWay(left) && getX() - 1 >= 0;
				newDir = left;
				break;
			case 4:
				isValid = !dirtInTheWay(right) && getX() + 4 < 60;
				newDir = right;
				break;
			}
			while (!isValid)
			{
				newTest = getWorld()->randInt(1, 4);
				switch (newTest)
				{
				case 1:
					isValid = !dirtInTheWay(up) && getY() + 4 < 60;
					newDir = up;
					break;
				case 2:
					isValid = !dirtInTheWay(down) && getY() - 1 >= 0;
					newDir = down;
					break;
				case 3:
					isValid = !dirtInTheWay(left) && getX() - 1 >= 0;
					newDir = left;
					break;
				case 4:
					isValid = !dirtInTheWay(right) && getX() + 4 < 60;
					newDir = right;
					break;
				}
			}
			setDir(newDir);
			numInCurrentDir = 5;
		}
		else
		{
			Direction dir = getDir();
			bool atIntersection = false;
			switch (dir)
			{
			case up:
			case down:
				atIntersection = !dirtInTheWay(right) && !dirtInTheWay(left);
				break;
			case left:
			case right:
				atIntersection = !dirtInTheWay(up) && !dirtInTheWay(down);
				break;
			}
			if (atIntersection)
			{
				int turnProb = getWorld()->randInt(0, 1);
				if ((dir == up || dir == down) && turnProb == 0)
				{
					if (dirtInTheWay(left))
					{
						setDir(right);
					}
					else
					{
						setDir(left);
					}
				}
				else if ((dir == right || dir == left) && turnProb == 0)
				{
					if (dirtInTheWay(down))
					{
						setDir(up);
					}
					else
					{
						setDir(down);
					}
				}
			}
		}
		switch (getDir())
		{
		case up:
			moveDir(up);
			break;
		case right:
			moveDir(right);
			break;
		case down:
			moveDir(down);
			break;
		case left:
			moveDir(left);
			break;
		}
	}
	restState = true;
}
void Protestors::checkForDamage()
{
	if (targetable)
	{
		for (int i = 0; i <= 3; i++)
		{
			if (getWorld()->isOccupiedType(getX() + i, getY(), TID_SQUIRT)
				|| getWorld()->isOccupiedType(getX(), getY() + i, TID_SQUIRT))
			{
				takeDamage(2);
				if (getHealth() <= 0)
				{
					getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
					getWorld()->addToScore(100);
					restState = false;
					leaveState = true;
				}
				else
				{
					getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
					restState = true;
					restTick = hitStun;
				}
				return;
			}
			else if (getWorld()->isOccupiedType(getX() + i, getY(), TID_GOLD)
				|| getWorld()->isOccupiedType(getX(), getY() + i, TID_GOLD))
			{
				getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
				getWorld()->addToScore(20);
				leaveState = true;
			}
			else if ((getWorld()->isOccupiedType(getX() + i, getY(), TID_BOULDER)
				|| getWorld()->isOccupiedType(getX(), getY() + i, TID_BOULDER)))
			{
				getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
				getWorld()->addToScore(500);
				restState = false;
				leaveState = true;
			}
		}
	}
}
bool Protestors::isRestState()
{
	return restState;
}
bool Protestors::isLeaveState()
{
	return leaveState;
}

Protestors::~Protestors()
{

}


RegularProtestors::RegularProtestors(int x, int y, StudentWorld* inputWorld)
	:Protestors(IID_PROTESTER, x, y, inputWorld)
{

}

void RegularProtestors::doSomething()
{
	if (!alive())
	{
		return;
	}
	else if (isRestState())
	{
		restStateOperation();
	}
	else if (isLeaveState())
	{
		leaveMap();
	}
	else if (playerInRange())
	{
		attackFrackMan();
	}
	else
	{
		movement();
	}
	checkForDamage();
}
int RegularProtestors::getType()
{
	return TID_PROTESTER;
}

RegularProtestors::~RegularProtestors()
{

}

HardcoreProtestors::HardcoreProtestors(int x, int y, StudentWorld* inputWorld)
	:Protestors(IID_HARD_CORE_PROTESTER, x, y, inputWorld)
{

}
int HardcoreProtestors::getType()
{
	return TID_HARD_CORE_PROTESTER;
}
void HardcoreProtestors::doSomething()
{
	if (!alive())
	{
		return;
	}
	else if (isRestState())
	{
		restStateOperation();
	}
	else if (isLeaveState())
	{
		leaveMap();
	}
	else if (playerInRange())
	{
		attackFrackMan();
	}
	else
	{
		movement();
	}
	checkForDamage();
}
HardcoreProtestors::~HardcoreProtestors()
{

}

Dirt::Dirt(int startX, int startY, StudentWorld *inputWorld)
	:Actor(IID_DIRT, startX, startY, right, 0.25, 3, inputWorld)
{
	dug = false;
	stepsToExit = -1;
	direction = -1;
}
int Dirt::getType()
{
	return TID_DIRT;
}
int Dirt::getStep()
{
	return stepsToExit;
}
void Dirt::setStep(int i)
{
	stepsToExit = i;
}
void Dirt::doSomething()
{
	return;
}
int Dirt::getDir()
{
	return direction;
}
void Dirt::setDir(int i)
{
	direction = i;
}
bool Dirt::isDug()
{
	return dug;
}
void Dirt::dig()
{
	dug = true;
}
Dirt::~Dirt()
{
}



Boulder::Boulder(int startX, int startY, StudentWorld* inputWorld)
	:Actor(IID_BOULDER, startX, startY, down, 1.0, 1, inputWorld)
{
	setVisible(true);
	stable = true;
	waiting = false;
	falling = false;
	waitTime = 0;
}
void Boulder::doSomething()
{
	if (!alive())
	{
		return;
	}
	if (stable)
	{
		int count = 0;
		for (int i = 0; i < 4; i++)
		{
			if (!(getWorld()->isOccupied(getX() + i, getY() - 1)))
			{
				count++;
			}
		}
		if (count == 4)
		{
			stable = false;
			waiting = true;
			falling = false;
		}
		return;
	}
	if (waiting)
	{
		if (waitTime == 30)
		{
			stable = false;
			waiting = false;
			falling = true;
			getWorld()->playSound(SOUND_FALLING_ROCK);
		}
		waitTime++;
	}
	if (falling)
	{
		int count = 0;
		if (getX() - getWorld()->getFrackManX() <= 3 && getX() - getWorld()->getFrackManX() >= -3
			&& getY() - getWorld()->getFrackManY() <= 3 && getY() - getWorld()->getFrackManY() >= -3)
		{
			getWorld()->getPlayer()->kill();
		}
		for (int i = 0; i < 4; i++)
		{
			if (getY() - 1 > -1 && !getWorld()->isOccupied(getX() + i, getY() - 1))
			{
				count++;
			}
		}
		if (count == 4)
		{
			setY(getY() - 1);
			moveTo(getX(), getY());
		}
		else
		{
			kill();
		}
	}
	return;
}
bool Boulder::isBoulder()
{
	return true;
} 
int Boulder::getType()
{
	return TID_BOULDER;
}
Boulder::~Boulder()
{
}


Squirt::Squirt(int startX, int startY, Direction dir, StudentWorld* inputWorld)
	:Actor(IID_WATER_SPURT, startX, startY, dir, 1.0, 1, inputWorld)
{
	hit = false;
	travelDistance = 0;
}
void Squirt::doSomething()
{
	if (!alive())
	{
		return;
	}
	if (travelDistance >= 4 || hit == true)
	{
		kill();
		return;
	}
	travelDistance++;
	Direction check = getDir();
	switch (check)
	{
		case up:
			if (getY() + 1 < 60 && !(getWorld()->isOccupied(getX(), getY() + 1) ))
			{
				setY(getY() + 1);
				if (getWorld()->isOccupiedType2(getX(), getY(), TID_PROTESTER) ||
					getWorld()->isOccupiedType2(getX(), getY(), TID_HARD_CORE_PROTESTER))
				{
					kill();
					return;
				}
				moveTo(getX(), getY());
			}
			else
			{
				kill();
				return;
			}
			break;
		case right:
			if (getX() + 1 < 60 && !(getWorld()->isOccupied(getX() + 4, getY())))
			{
				setX(getX() + 1);
				if (getWorld()->isOccupiedType2(getX(), getY(), TID_PROTESTER) ||
					getWorld()->isOccupiedType2(getX(), getY(), TID_HARD_CORE_PROTESTER))
				{
					hit = true;
					return;
				}
				moveTo(getX(), getY());
			}
			else
			{
				kill();
				return;
			}
			break;
		case left:
			if (getX() - 1 >= 0 && !(getWorld()->isOccupied(getX() - 1, getY())))
			{
				setX(getX() - 1);
				if (getWorld()->isOccupiedType2(getX(), getY(), TID_PROTESTER) ||
					getWorld()->isOccupiedType2(getX(), getY(), TID_HARD_CORE_PROTESTER))
				{
					kill();
					return;
				}
				moveTo(getX(), getY());
			}
			else
			{
				kill();
				return;
			}
			break;
		case down:
			if (getY() - 1 >= 0  && !(getWorld()->isOccupied(getX(), getY() - 1)))
			{
				setY(getY() - 1);
				if (getWorld()->isOccupiedType2(getX(), getY(), TID_PROTESTER) ||
					getWorld()->isOccupiedType2(getX(), getY(), TID_HARD_CORE_PROTESTER))
				{
					kill();
					return;
				}
				moveTo(getX(), getY());
			}
			else
			{
				kill();
				return;
			}
			break;
		default:
			break;
	}
	return;
}
int Squirt::getType()
{
	return TID_SQUIRT;
}
Squirt::~Squirt()
{
}

ActivatingObject::ActivatingObject(int imageID, int startX, int startY, StudentWorld* inputWorld)
	:Actor(imageID, startX, startY, right, 1.0, 2, inputWorld )
{
	collected = false;
}

bool ActivatingObject::wasCollected()
{
	return collected;
}
void ActivatingObject::collect()
{
	collected = true;
}
bool ActivatingObject::canCollect()
{
	if (getX() - getWorld()->getFrackManX() <= 3 && getX() - getWorld()->getFrackManX() >= -3
		&& getY() - getWorld()->getFrackManY() <= 3 && getY() - getWorld()->getFrackManY() >= -3)
	{
		collect();
		kill();
		return true;
	}
	return false;
}

Barrel::Barrel(int startX, int startY, StudentWorld* inputWorld)
	:ActivatingObject(IID_BARREL, startX, startY, inputWorld)
{
	visible = false;
}
void Barrel::doSomething()
{
	if (!visible)
	{
		if (getX() - getWorld()->getFrackManX() <= 4 && getX() - getWorld()->getFrackManX() >= -4
			&& getY() - getWorld()->getFrackManY() <= 4 && getY() - getWorld()->getFrackManY() >= -4)
		{
			visible = true;
			setVisible(visible);
			return;
		}
	}
	else
	{
		canCollect();
	}
}
int Barrel::getType()
{
	return TID_BARREL;
}
Barrel::~Barrel()
{
	getWorld()->playSound(SOUND_FOUND_OIL);
}

GoldNugget::GoldNugget(int startX, int startY, StudentWorld* inputWorld, bool forFrackMan)
	:ActivatingObject(IID_GOLD, startX, startY, inputWorld)
{
	if (forFrackMan)
	{
		visible = false;
		forDaPlayer = true;

	}
	else
	{
		visible = true;
		setVisible(true);
		forDaPlayer = false;
	}
	tickCount = 0;
}
void GoldNugget::doSomething()
{
	if (!visible && forDaPlayer)
	{
		if (getX() - getWorld()->getFrackManX() <= 4 && getX() - getWorld()->getFrackManX() >= -4
			&& getY() - getWorld()->getFrackManY() <= 4 && getY() - getWorld()->getFrackManY() >= -4)
		{	
			visible = true;
			setVisible(visible);
			return;
		}

	}
	else if(forDaPlayer)
	{
		canCollect();
	}
	else if (!forDaPlayer)
	{
		if (tickCount >= 100)
		{
			kill();
		}
		else if (getWorld()->isOccupiedType2(getX(), getY(), IID_PROTESTER))
		{
			kill();
		}
		tickCount++;
	}
}
bool GoldNugget::forPlayer()
{
	return forDaPlayer;
}
int GoldNugget::getType()
{
	return TID_GOLD;
}

GoldNugget::~GoldNugget()
{

}

SonarKit::SonarKit(int startX, int startY, StudentWorld* inputWorld)
	:ActivatingObject(IID_SONAR, startX, startY,inputWorld)
{
	setVisible(true);
	curLife = 0;
	lifeSpan = 100;
}
void SonarKit::doSomething()
{
	canCollect();
	if (curLife >= lifeSpan)
	{
		kill();
	}
	curLife++;
}

int SonarKit::getType()
{
	return TID_SONAR;
}
SonarKit::~SonarKit()
{

}

WaterPool::WaterPool(int startX, int startY, StudentWorld *inputWorld)
	:ActivatingObject(IID_WATER_POOL, startX, startY, inputWorld)
{
	setVisible(true);
	curLife = 0;
	lifeSpan = 100;
}
void WaterPool::doSomething()
{
	canCollect();
	if (curLife >= lifeSpan)
	{
		kill();
	}
	curLife++;
}
int WaterPool::getType()
{
	return TID_WATER_POOL;
}
WaterPool::~WaterPool()
{

}

