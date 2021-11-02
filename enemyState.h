#pragma once
#include "gameNode.h"
#include "animation.h"

enum class ENEMYSTATE {
	IDLE,
	MOVE,
	ATTACK,
	HIT,
	DIE
};

enum class ENEMYDIRECTION
{
	LEFT,
	RIGHT,
	UP,
	DOWN
};

enum class ENEMYTYPE
{
	NORMAL,
	SHOTGUN,
	BOOK
};


class enemy;

class enemyState : public gameNode
{
protected:
	enemy* _em;

	ENEMYDIRECTION _eDir;

public:
	enemyState();
	~enemyState();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	virtual void changeState();

	void setE(enemy* em) { _em = em; }
};

class enemyIdle : public enemyState
{
public:
	enemyIdle();
	~enemyIdle();

	virtual HRESULT init() override;
	virtual void release() override;
	virtual void update() override;
	virtual void render() override;

	void changeState();
};

class enemyMove : public enemyState
{
public:
	enemyMove();
	~enemyMove();

	virtual HRESULT init() override;
	virtual void release() override;
	virtual void update() override;
	virtual void render() override;

	void changeState();
};

class enemyAttack : public enemyState
{
public:
	enemyAttack();
	~enemyAttack();

	virtual HRESULT init() override;
	virtual void release() override;
	virtual void update() override;
	virtual void render() override;

	void changeState();
};

class enemyHit : public enemyState
{
public:
	enemyHit();
	~enemyHit();

	virtual HRESULT init() override;
	virtual void release() override;
	virtual void update() override;
	virtual void render() override;

	void changeState();
};

class enemyDie : public enemyState
{
public:
	enemyDie();
	~enemyDie();

	virtual HRESULT init() override;
	virtual void release() override;
	virtual void update() override;
	virtual void render() override;

	void changeState();
};