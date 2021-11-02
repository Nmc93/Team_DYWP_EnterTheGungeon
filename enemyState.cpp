#include "pch.h"
#include "enemyState.h"
#include "enemy.h"

enemyState::enemyState()
{
}

enemyState::~enemyState()
{
}

HRESULT enemyState::init()
{
	return S_OK;
}

void enemyState::release()
{
}

void enemyState::update()
{
	changeState();
	_eDir = _em->getEDir();

	if (KEYMANAGER->isOnceKeyDown(VK_F1)) _em->changeState(ENEMYSTATE::HIT);
	if (KEYMANAGER->isOnceKeyDown(VK_F2)) _em->changeState(ENEMYSTATE::DIE);
}

void enemyState::render()
{
}

void enemyState::changeState()
{
}


enemyIdle::enemyIdle()
{
}

enemyIdle::~enemyIdle()
{
}

HRESULT enemyIdle::init()
{
	return S_OK;
}

void enemyIdle::release()
{
}

void enemyIdle::update()
{
	//changeState();
	enemyState::update();
}

void enemyIdle::render()
{
}

void enemyIdle::changeState()
{
	//getDistance(WINSIZEX / 2 - _em->getCamera()->getMx(), WINSIZEY / 2 - _em->getCamera()->getMy(), _em->getEnemyX(), _em->getEnemyY()) > 250
	if (getDistance(_em->getPlayerXY().x, _em->getPlayerXY().y, _em->getShadow().x, _em->getShadow().y) > 250)
	{
		_em->changeState(ENEMYSTATE::MOVE);
	}
	if (_eDir != _em->getEDir())
	{
		_em->changeState(ENEMYSTATE::MOVE);
	}
	//if (_em->getEnemyHP() <= 0 && _em->getEnemyState() != ENEMYSTATE::DIE) _em->changeState(ENEMYSTATE::DIE);
}

enemyMove::enemyMove()
{
}

enemyMove::~enemyMove()
{
}

HRESULT enemyMove::init()
{
	return S_OK;
}

void enemyMove::release()
{
}

void enemyMove::update()
{
	enemyState::update();
}

void enemyMove::render()
{
}

void enemyMove::changeState()
{
	if (_eDir != _em->getEDir()) _em->changeState(ENEMYSTATE::IDLE);
	//getDistance(WINSIZEX / 2 - _em->getCamera()->getMx(), WINSIZEY / 2 - _em->getCamera()->getMy(),_em->getEnemyX(), _em->getEnemyY()) <= 250) _em->changeState(ENEMYSTATE::IDLE
	if (getDistance(WINSIZEX / 2, WINSIZEY / 2,_em->getPlayerXY().x, _em->getPlayerXY().y) <= 250) _em->changeState(ENEMYSTATE::IDLE);
	else _em->move();

	//if (_eDir != _em->getEDir()) _em->changeAni();
}

enemyAttack::enemyAttack()
{
}

enemyAttack::~enemyAttack()
{
}

HRESULT enemyAttack::init()
{
	return S_OK;
}

void enemyAttack::release()
{
}

void enemyAttack::update()
{
	enemyState::update();
}

void enemyAttack::render()
{
}

void enemyAttack::changeState()
{
	if (_em->getEnemyType() != ENEMYTYPE::BOOK) _em->changeState(ENEMYSTATE::IDLE);
}

enemyHit::enemyHit()
{
}

enemyHit::~enemyHit()
{
}

HRESULT enemyHit::init()
{
	return S_OK;
}

void enemyHit::release()
{
}

void enemyHit::update()
{
	//_em->hitDamage(5);
	//맞았을 때 또 맞지 말라고
	//enemyState::update();
}

void enemyHit::render()
{
}

void enemyHit::changeState()
{
}

enemyDie::enemyDie()
{
}

enemyDie::~enemyDie()
{
}

HRESULT enemyDie::init()
{
	return S_OK;
}

void enemyDie::release()
{
}

void enemyDie::update()
{
	//enemyState::update();
}

void enemyDie::render()
{
}

void enemyDie::changeState()
{
}