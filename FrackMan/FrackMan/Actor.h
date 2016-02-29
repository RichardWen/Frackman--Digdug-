#ifndef ACTOR_H_
#define ACTOR_H_
class StudentWorld;
#include "GraphObject.h"

const int TID_PLAYER = 0;
const int TID_PROTESTER = 1;
const int TID_HARD_CORE_PROTESTER = 2;
const int TID_SQUIRT = 3;
const int TID_BOULDER = 4;
const int TID_BARREL = 5;
const int TID_DIRT = 6;
const int TID_GOLD = 7;
const int TID_SONAR = 8;
const int TID_WATER_POOL = 9;

class Actor : public GraphObject
{
public:
	Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld *inputWorld);
	virtual void doSomething() = 0;
	virtual int getType() = 0;
	virtual bool wasCollected();
	int getX();
	void setX(int inX);
	int getY();
	void setY(int inY);
	bool alive();
	void kill();
	Direction getDir()
	{
		return myDir;
	}
	void setDir(Direction newDir);
	void display();
	virtual StudentWorld* getWorld();
	~Actor();
private:
	bool isAlive;
	int xLocation;
	int yLocation;
	double mySize;
	Direction myDir;
	bool isAnnoyed;
	StudentWorld* gameWorld;
};



class Agent : public Actor
{
public:
	Agent(int imageID, int startX, int startY, int health, Direction dir, StudentWorld* inputWorld);
	int getHealth();
	bool dirtInTheWay(Direction dir);
	void moveDir(Direction dir);
	void takeDamage(int damage);

private:
	int hitPoints;
};

class FrackMan : public Agent
{
public:
	FrackMan(StudentWorld* inputWorld);
	virtual void doSomething();
	virtual int getType();
	int getAmmo();
	void findAmmo();
	int getCharges();
	void findCharge();
	int getNuggets();
	void findNugget();
	virtual ~FrackMan();
private:
	int waterAmmo;
	int sonarCharges;
	int goldNuggets;
};

class Protestors : public Agent
{
public:
	Protestors(int imageID, int startX, int startY, StudentWorld* inputWorld);
	bool lineOfSight(Direction &dir);
	bool playerInRange();
	void restStateOperation();
	void leaveMap();
	void attackFrackMan();
	void movement();
	void checkForDamage();
	bool isRestState();
	bool isLeaveState();
	virtual ~Protestors();
private:
	int restTick;
	int timeToShout;
	int timeToAction;
	int numInCurrentDir;
	int tickCanMove;
	int hitStun;
 	bool hit;
	bool targetable;
	bool shout;
	bool restState;
	bool leaveState;
	bool isAnnoyed;
};
class RegularProtestors : public Protestors
{
public:
	RegularProtestors(int startX, int startY, StudentWorld* inputWorld);
	virtual int getType();
	virtual void doSomething();
	virtual ~RegularProtestors();
private:
};

class HardcoreProtestors : public Protestors
{
public:
	HardcoreProtestors(int startX, int startY, StudentWorld* inputWorld);
	virtual int getType();
	virtual void doSomething();
	virtual ~HardcoreProtestors();
private:
};

class Dirt : public Actor
{
public:
	Dirt(int startX, int startY, StudentWorld* inputWorld);
	virtual void doSomething();
	virtual int getType();
	bool isDug();
	void dig();
	int getStep();
	void setStep(int i);
	int getDir();
	void setDir(int i);
	virtual ~Dirt();
private:
	bool dug;
	int stepsToExit;
	int direction;
};


class Boulder : public Actor
{
public:
	Boulder(int startX, int startY, StudentWorld* inputWorld);
	virtual void doSomething();
	virtual bool isBoulder();
	virtual int getType();
	virtual ~Boulder();
private:
	bool stable;
	bool waiting;
	bool falling;
	int waitTime;
};


class Squirt : public Actor
{
public:
	Squirt(int startX, int startY, Direction dir, StudentWorld* inputWorld);
	virtual void doSomething();
	virtual int getType();
	virtual ~Squirt();
private:
	int travelDistance;
	bool hit;
};


class ActivatingObject : public Actor
{
public:
	ActivatingObject(int imageID, int startX, int startY, StudentWorld * inputWorld);
	virtual bool wasCollected();
	void collect();
	bool canCollect();
private:
	bool collected;
};

class Barrel : public ActivatingObject
{
public:
	Barrel(int startX, int startY, StudentWorld * inputWorld);
	virtual void doSomething();
	virtual int getType();
	virtual ~Barrel();
private:
	bool visible;
};

class GoldNugget : public ActivatingObject
{
public:
	GoldNugget(int startX, int startY, StudentWorld* inputWorld, bool forFrackMan);
	virtual void doSomething();
	virtual int getType();
	bool forPlayer();
	virtual ~GoldNugget();
private:
	bool visible;
	bool forDaPlayer;
	int tickCount;
};

class SonarKit : public ActivatingObject
{
public:
	SonarKit(int startX, int startY, StudentWorld* inputWorld);
	virtual void doSomething();
	virtual int getType();
	virtual ~SonarKit();
private:
	int curLife;
	int lifeSpan;
};

class WaterPool : public ActivatingObject
{
public:
	WaterPool(int startX, int startY, StudentWorld* inputWorld);
	virtual void doSomething();
	virtual int getType();
	virtual ~WaterPool();
private:
	int curLife;
	int lifeSpan;
};


#endif // ACTOR_H_
