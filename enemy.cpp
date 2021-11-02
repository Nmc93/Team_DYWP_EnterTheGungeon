#include "pch.h"
#include "enemy.h"

enemy::enemy()
{
}

enemy::~enemy()
{
}

HRESULT enemy::init(ENEMYTYPE enemyType, POINTF spt)
{
	setImage();
	setAni();

#pragma region ENEMYSTATE
	_idle = new enemyIdle;
	_idle->setE(this);

	_move = new enemyMove;
	_move->setE(this);

	_attack = new enemyAttack;
	_attack->setE(this);

	_hit = new enemyHit;
	_hit->setE(this);

	_die = new enemyDie;
	_die->setE(this);
#pragma endregion

	//그림자 중점좌표 초기화
	_sXY = spt;

	//몸통 중점좌표 및 그림자와 몸통의 거리값 초기화
	
	if (enemyType == ENEMYTYPE::BOOK)
	{
		_fly = 50.0f;
	}
	else if (enemyType == ENEMYTYPE::NORMAL)
	{
		_fly = 30.0f;
	}
	else if (enemyType == ENEMYTYPE::SHOTGUN)
	{
		_fly = 40.0f;
	}
	
	_bXY.x = _sXY.x;
	_bXY.y = _sXY.y - _fly;

	//현재 좌표
	_currentX = spt.x;
	_currentY = spt.y;

	//이미지 변환용 값 초기화
	_alpha = 255;

	//상태 및 이동 가능 여부 초기화
	_emState = _idle;
	_isMove = false;

	//소환 이미지 및 그림자 이미지 초기화
	_image = IMAGEMANAGER->findImage("enemyAppear");
	_sImg = IMAGEMANAGER->findImage("SHADOW");

	//렉트 초기화
	_sRc = RectMakeCenter(_sXY.x,_sXY.y, _sImg->getFrameWidth(), _sImg->getFrameHeight());
	//_bRc = RectMakeCenter(static_cast<int>(_bXY.x), static_cast<int>(_bXY.y), );
	//_playerRc = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, 50, 50);
	_isEnemyDie = false;

	return S_OK;
}

void enemy::release()
{
}

void enemy::update()
{
	if (KEYMANAGER->isOnceKeyDown('B')) changeState(ENEMYSTATE::HIT);

	attack();
	dir();
	if (_appear) _emState->update();

	changeAlpha();
	frameCount();

	// 움직임에 따른 그림자 업데이트
	_sXY.x = + _currentX;
	_sXY.y = + _currentY;

	// 그림자에 기반한 몸통 움직임 업데이트
	_bXY.x = _sXY.x;
	_bXY.y = _sXY.y - _fly;

	//중점좌표를 기준으로 렉트 업데이트
	_sRc = RectMakeCenter(_sXY.x, _sXY.y, _sImg->getWidth(), _sImg->getHeight());
	_bRc = RectMakeCenter(_bXY.x, _bXY.y, _enemyImg->getFrameWidth(), _enemyImg->getFrameHeight());

	//이미지 프레임 초기화
	if (_enemyImg->getMaxFrameX() < _idX && _appear) _idX = 0;

	//맞았는지 확인
	if (_isHit)
	{
		_isHit = false;
		hitDamage(5);
	}
}

void enemy::render()
{
	//alpha값은 0이 투명(소환 이미지 이후 에너미 이미지 출력)
	//if (!_appear)
	//{
	//	_image->frameRender(getMemDC(), _bRc.left, _bRc.top , _idX, _idY);
	//}
	//else
	//{
	//	if (_enemyImg->getMaxFrameX() < _idX) _idX = 0;
	//	_enemyImg->frameRender(getMemDC(), _bRc.left , _bRc.right , _idX, _idY);

	//	if (_enemyType == ENEMYTYPE::BOOK) _bullet->render(true);
	//}

	Rectangle(getMemDC(), _sRc);
	Rectangle(getMemDC(), _bRc);


	//Rectangle(getMemDC(), _playerRc);

	char str[128];
	sprintf_s(str, "_dir : %d", _enemyDir);
	TextOut(getMemDC(), 0, 200, str, strlen(str));

	sprintf_s(str, "_state : %d", _enemyState);
	TextOut(getMemDC(), 0, 400, str, strlen(str));

	sprintf_s(str, "_x : %0.1f, _y : %0.1f", _bXY.x, _bXY.y);
	TextOut(getMemDC(), 0, 300, str, strlen(str));

	sprintf_s(str, "_idX : %d, _idY : %d", _idX, _idY);
	TextOut(getMemDC(), 0, 700, str, strlen(str));
}

void enemy::frameCount()
{
	_iCount++;
	if (!_appear) _continueCount = 4;
	else  _continueCount = 10;

	if (_iCount > _continueCount)
	{
		_iCount = 0;
		_idX++;

		// 첫 등장 씬만
		if (_image->getMaxFrameX() < _idX && !_appear)
		{
			_appear = true;
			_idX = 0;
		}

		// 나머지 씬들
		if (_enemyImg->getMaxFrameX() < _idX && _appear)
		{
			if (_isLoop) _idX = 0;
			else		 _idX -= 1;

			if (_isTrigger) changeState(ENEMYSTATE::IDLE);
		}
	}
}

void enemy::dir()
{
	//if(_x < cosf((WINSIZEX/2 - _x) || _y < -sinf(WINSIZEY/2- _y))
	_angle = atan2f(_sXY.y - _pXY.y/* - _camera->getMy()*/, _pXY.x - _sXY.x /*- _camera->getMx()*/);

	if (_angle < PI / 6 && _angle >= -PI / 2) _enemyDir = ENEMYDIRECTION::RIGHT;						// 30 , -90
	else if (_angle >= 5 * PI / 6 || _angle < -PI / 2) _enemyDir = ENEMYDIRECTION::LEFT;				// 150, -90
	else if (_angle >= PI / 6 && _angle < 5 * PI / 6) _enemyDir = ENEMYDIRECTION::UP;					// 30 , 150
}

void enemy::move()
{
	_currentAngle = atan2f(_sXY.y - _pXY.y, _sXY.x - _pXY.x);
	_currentX -= cosf(_currentAngle) * 3;
	_currentY -= sinf(_currentAngle) * 3;
}

void enemy::changeAlpha()
{
	if (_emState == _idle) _alpha = 255;

	if (_emState == _hit)
	{
		_alphaCount++;

		if (_alphaCount % 30 == 0) _alpha = 0;
		else _alpha = 255;
	}
	else if (_emState != _hit)
	{
		_alphaCount = 0;
		_alpha = 255;
	}

	if (_emState == _die)
	{
		_isEnemyDie = true;
		_alpha = 200;
	}
}

void enemy::hitDamage(int damage)
{
	if (_currentHP > 0) _currentHP -= damage;
	else
	{
		changeState(ENEMYSTATE::DIE);
	}
}

void enemy::returnIdle(void* obj)
{
	// 나 이런거 담겨있음
	enemy* em = (enemy*)obj;

	// 반환
	em->changeState(ENEMYSTATE::IDLE);
}

void enemy::appearEnemy(void* obj)
{
	enemy* e = (enemy*)obj;
	e->setAppear(true);
}

void enemy::setImage()
{
	IMAGEMANAGER->addFrameImage("enemyAppear", "image/enemyAppear.bmp", 1326, 80, 17, 1, true, RGB(255, 0, 255));
}

void enemy::setAni()
{

}

HRESULT normalEnemy::init(ENEMYTYPE enemyType, POINTF spt)
{
	setImage();
	setAni();
	enemy::init(enemyType, spt);

	changeState(ENEMYSTATE::IDLE);
	_enemyType = enemyType;
	
	_bRc = RectMakeCenter(_bXY.x, _bXY.y, _enemyImg->getFrameWidth(), _enemyImg->getFrameHeight());

	_currentHP = _maxHP = 15;

	return S_OK;
}

void normalEnemy::release()
{
}

void normalEnemy::update()
{
	enemy::update();
	//_bullet->update();
}

void normalEnemy::render()
{
	enemy::render();
}

void normalEnemy::setImage()
{
	IMAGEMANAGER->addFrameImage("normalEnemyIdle", "image/normalEnemyIdle.bmp", 96, 225, 2, 3, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("normalEnemyMove", "image/normalEnemyMove.bmp", 360, 237, 6, 3, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("normalEnemyHit", "image/normalEnemyHit.bmp", 432, 150, 8, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("normalEnemyDie", "image/normalEnemyDie.bmp", 450, 576, 6, 8, true, RGB(255, 0, 255));
}

void normalEnemy::setAni()
{
	KEYANIMANAGER->addCoordinateFrameAnimation("NORMAL_ENEMY_IDLE_LEFT", "normalEnemyIdle", 0, 2, 2, false, true);
	KEYANIMANAGER->addCoordinateFrameAnimation("NORMAL_ENEMY_IDLE_RIGHT", "normalEnemyIdle", 2, 4, 2, false, true);
	KEYANIMANAGER->addCoordinateFrameAnimation("NORMAL_ENEMY_IDLE_UP", "normalEnemyIdle", 4, 6, 2, false, true);

	KEYANIMANAGER->addCoordinateFrameAnimation("NORMAL_ENEMY_MOVE_LEFT", "normalEnemyMove", 0, 6, 4, false, true);
	KEYANIMANAGER->addCoordinateFrameAnimation("NORMAL_ENEMY_MOVE_RIGHT", "normalEnemyMove", 6, 12, 4, false, true);
	KEYANIMANAGER->addCoordinateFrameAnimation("NORMAL_ENEMY_MOVE_UP", "normalEnemyMove", 12, 18, 4, false, true);

	KEYANIMANAGER->addCoordinateFrameAnimation("NORMAL_ENEMY_HIT_LEFT", "normalEnemyHit", 0, 8, 6, false, false, returnIdle, this);
	KEYANIMANAGER->addCoordinateFrameAnimation("NORMAL_ENEMY_HIT_RIGHT", "normalEnemyHit", 8, 16, 6, false, false, returnIdle, this);

	KEYANIMANAGER->addCoordinateFrameAnimation("NORMAL_ENEMY_DIE_LEFT", "normalEnemyDie", 12, 18, 4, false, false);
	KEYANIMANAGER->addCoordinateFrameAnimation("NORMAL_ENEMY_DIE_RIGHT", "normalEnemyDie", 18, 24, 4, false, false);
	KEYANIMANAGER->addCoordinateFrameAnimation("NORMAL_ENEMY_DIE_UP", "normalEnemyDie", 42, 48, 4, false, false);
}

void normalEnemy::attack()
{
	if (_emState == _idle || _emState == _move)
	{
		_count++;
		if (_count % 150 == 0)
		{
			_count = 0;

			changeState(ENEMYSTATE::ATTACK);
			//enemyBulletFire();
			if (_enemyDir == ENEMYDIRECTION::LEFT || _enemyDir == ENEMYDIRECTION::UP)
			{
				_bullet->fire(_bXY.x - 20, _bXY.y , getAngle(_sXY.x, _sXY.y, _pXY.x, _pXY.y), 3.0f, BTYPE::ENEMY);
			}
			else _bullet->fire(_bXY.x - 20 , _bXY.y , getAngle(_sXY.x, _sXY.y, _pXY.x, _pXY.y), 3.0f, BTYPE::ENEMY);
		}
	}

}

void normalEnemy::changeState(ENEMYSTATE state)
{
	_enemyState = state;

	switch (state)
	{
	case ENEMYSTATE::IDLE:
		_isLoop = true;
		_emState = _idle;
		_enemyImg = IMAGEMANAGER->findImage("normalEnemyIdle");
		break;
	case ENEMYSTATE::MOVE:
		_emState = _move;
		_enemyImg = IMAGEMANAGER->findImage("normalEnemyMove");
		break;
	case ENEMYSTATE::ATTACK:
		_emState = _attack;
		_enemyImg = IMAGEMANAGER->findImage("normalEnemyIdle");
		break;
	case ENEMYSTATE::HIT:
		hitDamage(5);
		_emState = _hit;
		_enemyImg = IMAGEMANAGER->findImage("normalEnemyHit");
		break;
	case ENEMYSTATE::DIE:
		_isEnemyDie = true;
		_emState = _die;
		_enemyImg = IMAGEMANAGER->findImage("normalEnemyDie");
		break;
	}
	changeAni();
}

void normalEnemy::changeAni()
{
	switch (_enemyState)
	{
	case ENEMYSTATE::IDLE:
		if (_enemyDir == ENEMYDIRECTION::RIGHT || _enemyDir == ENEMYDIRECTION::DOWN)
		{
			_idY = 1;
		}
		if (_enemyDir == ENEMYDIRECTION::LEFT)
		{
			_idY = 0;
		}
		if (_enemyDir == ENEMYDIRECTION::UP)
		{
			_idY = 2;
		}
		_isLoop = true;
		_isTrigger = false;
		break;
	case ENEMYSTATE::MOVE:
		if (_enemyDir == ENEMYDIRECTION::RIGHT || _enemyDir == ENEMYDIRECTION::DOWN)
		{
			_idY = 1;
		}
		if (_enemyDir == ENEMYDIRECTION::LEFT)
		{
			_idY = 0;
		}
		if (_enemyDir == ENEMYDIRECTION::UP)
		{
			_idY = 2;
		}
		_isLoop = true;
		_isTrigger = false;
		break;
	case ENEMYSTATE::ATTACK:
		if (_enemyDir == ENEMYDIRECTION::RIGHT || _enemyDir == ENEMYDIRECTION::DOWN)
		{
			_idY = 1;
		}
		if (_enemyDir == ENEMYDIRECTION::LEFT)
		{
			_idY = 0;
		}
		if (_enemyDir == ENEMYDIRECTION::UP)
		{
			_idY = 2;
		}
		_isLoop = false;
		_isTrigger = false;
		break;
	case ENEMYSTATE::HIT:
		if (_playerRc.left + 5 >= _sRc.right)
		{
			_idY = 1;
		}
		else if (_playerRc.left + 5 < _sRc.right)
		{
			_idY = 0;
		}
		_isLoop = false;
		_isTrigger = true;
		break;
	case ENEMYSTATE::DIE:
		if (_enemyDir == ENEMYDIRECTION::RIGHT || _enemyDir == ENEMYDIRECTION::DOWN)
		{
			_idY = 3;
		}
		if (_enemyDir == ENEMYDIRECTION::LEFT)
		{
			_idY = 2;
		}
		if (_enemyDir == ENEMYDIRECTION::UP)
		{
			_idY = 7;
		}
		_isLoop = false;
		_isTrigger = false;
		break;
	}
}

// 샷건
HRESULT shotgunEnemy::init(ENEMYTYPE enemyType, POINTF spt)
{
	setImage();
	setAni();
	enemy::init(enemyType, spt);

	changeState(ENEMYSTATE::IDLE);

	_enemyType = enemyType;

	_enemyImg = IMAGEMANAGER->findImage("shotgunEnemyIdle");

	_bRc = RectMakeCenter(_bXY.x, _bXY.y, _enemyImg->getFrameWidth(), _enemyImg->getFrameHeight());

	_enemyMotion = KEYANIMANAGER->findAnimation("SHOTGUN_ENEMY_IDLE_LEFT");
	_enemyMotion->start();

	_currentHP = _maxHP = 30;

	return S_OK;
}

void shotgunEnemy::release()
{
}

void shotgunEnemy::update()
{
	enemy::update();
	//_bullet->update();
}

void shotgunEnemy::render()
{
	enemy::render();
}

void shotgunEnemy::setImage()
{
	IMAGEMANAGER->addFrameImage("shotgunEnemyIdle", "image/shotgunEnemyIdle.bmp", 84, 261, 2, 3, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("shotgunEnemyMove", "image/shotgunEnemyMove.bmp", 396, 288, 6, 3, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("shotgunEnemyHit", "image/shotgunEnemyHit.bmp", 300, 180, 5, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("shotgunEnemyDie1", "image/shotgunEnemyDie1.bmp", 1134, 228, 9, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("shotgunEnemyDie2", "image/shotgunEnemyDie2.bmp", 555, 720, 5, 8, true, RGB(255, 0, 255));
}

void shotgunEnemy::setAni()
{
	KEYANIMANAGER->addCoordinateFrameAnimation("SHOTGUN_ENEMY_IDLE_LEFT", "shotgunEnemyIdle", 0, 2, 2, false, true);
	KEYANIMANAGER->addCoordinateFrameAnimation("SHOTGUN_ENEMY_IDLE_RIGHT", "shotgunEnemyIdle", 2, 4, 2, false, true);
	KEYANIMANAGER->addCoordinateFrameAnimation("SHOTGUN_ENEMY_IDLE_UP", "shotgunEnemyIdle", 4, 6, 2, false, true);

	KEYANIMANAGER->addCoordinateFrameAnimation("SHOTGUN_ENEMY_MOVE_LEFT", "shotgunEnemyMove", 0, 6, 4, false, true);
	KEYANIMANAGER->addCoordinateFrameAnimation("SHOTGUN_ENEMY_MOVE_RIGHT", "shotgunEnemyMove", 6, 12, 4, false, true);
	KEYANIMANAGER->addCoordinateFrameAnimation("SHOTGUN_ENEMY_MOVE_UP", "shotgunEnemyMove", 12, 18, 4, false, true);

	KEYANIMANAGER->addCoordinateFrameAnimation("SHOTGUN_ENEMY_HIT_LEFT", "shotgunEnemyHit", 0, 5, 6, false, false, returnIdle, this);
	KEYANIMANAGER->addCoordinateFrameAnimation("SHOTGUN_ENEMY_HIT_RIGHT", "shotgunEnemyHit", 5, 10, 6, false, false, returnIdle, this);

	KEYANIMANAGER->addCoordinateFrameAnimation("SHOTGUN_ENEMY_DIE1_LEFT", "shotgunEnemyDie1", 0, 9, 4, false, false);
	KEYANIMANAGER->addCoordinateFrameAnimation("SHOTGUN_ENEMY_DIE1_RIGHT", "shotgunEnemyDie1", 9, 18, 4, false, false);
	KEYANIMANAGER->addCoordinateFrameAnimation("SHOTGUN_ENEMY_DIE2_LEFT", "shotgunEnemyDie2", 10, 15, 4, false, false);
	KEYANIMANAGER->addCoordinateFrameAnimation("SHOTGUN_ENEMY_DIE2_RIGHT", "shotgunEnemyDie2", 15, 20, 4, false, false);
	KEYANIMANAGER->addCoordinateFrameAnimation("SHOTGUN_ENEMY_DIE2_UP", "shotgunEnemyDie2", 35, 40, 4, false, false);
}

void shotgunEnemy::attack()
{
	if (_emState == _idle || _emState == _move)
	{
		_count++;

		if (_count % 170 == 0)
		{
			_count = 0;

			changeState(ENEMYSTATE::ATTACK);
			if (_enemyDir == ENEMYDIRECTION::LEFT || _enemyDir == ENEMYDIRECTION::UP)
			{
				for (int i = -2; i <= 2; i++)
				{
					float angle = PI / 18;
					_bullet->fire( _bXY.x - 20, _bXY.y , (i * angle) + getAngle(_sXY.x, _sXY.y, _pXY.x, _pXY.y ), 3.0f, BTYPE::ENEMY);
				}
			}
			else if (_enemyDir == ENEMYDIRECTION::RIGHT)
			{
				for (int i = -2; i <= 2; i++)
				{
					float angle = PI / 18;
					_bullet->fire( _bXY.x + 20, _bXY.y , -(i * angle) + getAngle(_sXY.x, _sXY.y, _pXY.x, _pXY.y), 3.0f, BTYPE::ENEMY);
				}
			}
		}
	}
}

void shotgunEnemy::changeState(ENEMYSTATE state)
{
	_enemyState = state;

	switch (state)
	{
	case ENEMYSTATE::IDLE:
		_emState = _idle;
		_enemyImg = IMAGEMANAGER->findImage("shotgunEnemyIdle");
		break;
	case ENEMYSTATE::MOVE:
		_emState = _move;
		_enemyImg = IMAGEMANAGER->findImage("shotgunEnemyMove");
		break;
	case ENEMYSTATE::ATTACK:
		_emState = _attack;
		_enemyImg = IMAGEMANAGER->findImage("shotgunEnemyIdle");
		break;
	case ENEMYSTATE::HIT:
		hitDamage(5);
		_isHit = true;
		_emState = _hit;
		_enemyImg = IMAGEMANAGER->findImage("shotgunEnemyHit");
		break;
	case ENEMYSTATE::DIE:
		_isEnemyDie = true;
		_emState = _die;
		_dieType = RND->getInt(2);
		if (_dieType == 0)
		{
			_enemyImg = IMAGEMANAGER->findImage("shotgunEnemyDie1");
		}
		else if (_dieType == 1)
		{
			_enemyImg = IMAGEMANAGER->findImage("shotgunEnemyDie2");
		}
		break;
	}
	changeAni();
}

void shotgunEnemy::changeAni()
{
	switch (_enemyState)
	{
	case ENEMYSTATE::IDLE:
		if (_enemyDir == ENEMYDIRECTION::RIGHT || _enemyDir == ENEMYDIRECTION::DOWN)
		{
			_idY = 1;
		}
		if (_enemyDir == ENEMYDIRECTION::LEFT)
		{
			_idY = 0;
			_enemyMotion = KEYANIMANAGER->findAnimation("SHOTGUN_ENEMY_IDLE_LEFT");
		}
		if (_enemyDir == ENEMYDIRECTION::UP)
		{
			_idY = 2;
		}
		_isLoop = true;
		_isTrigger = false;
		break;
	case ENEMYSTATE::MOVE:
		if (_enemyDir == ENEMYDIRECTION::RIGHT || _enemyDir == ENEMYDIRECTION::DOWN)
		{
			_idY = 1;
		}
		if (_enemyDir == ENEMYDIRECTION::LEFT)
		{
			_idY = 0;
		}
		if (_enemyDir == ENEMYDIRECTION::UP)
		{
			_idY = 2;
		}
		_isLoop = true;
		_isTrigger = false;
		break;
	case ENEMYSTATE::ATTACK:
		if (_enemyDir == ENEMYDIRECTION::RIGHT || _enemyDir == ENEMYDIRECTION::DOWN)
		{
			_idY = 1;
		}
		if (_enemyDir == ENEMYDIRECTION::LEFT)
		{
			_idY = 0;
		}
		if (_enemyDir == ENEMYDIRECTION::UP)
		{
			_idY = 2;
		}
		_isLoop = false;
		_isTrigger = false;
		break;
	case ENEMYSTATE::HIT:
		if (_playerRc.left + 5 >= _sRc.right)
		{
			_idY = 1;
		}
		else if (_playerRc.left + 5 < _sRc.right)
		{
			_idY = 0;
		}
		_isLoop = false;
		_isTrigger = true;
		break;
	case ENEMYSTATE::DIE:
		if (_dieType == 0)
		{
			if (_enemyDir == ENEMYDIRECTION::RIGHT || _enemyDir == ENEMYDIRECTION::DOWN || _enemyDir == ENEMYDIRECTION::UP)
			{
				_idY = 1;
			}
			if (_enemyDir == ENEMYDIRECTION::LEFT)
			{
				_idY = 0;
			}
		}
		if (_dieType == 1)
		{
			if (_enemyDir == ENEMYDIRECTION::RIGHT || _enemyDir == ENEMYDIRECTION::DOWN)
			{
				_idY = 3;
			}
			if (_enemyDir == ENEMYDIRECTION::LEFT)
			{
				_idY = 2;
			}
			if (_enemyDir == ENEMYDIRECTION::UP)
			{
				_idY = 7;
			}
		}
		_isLoop = false;
		_isTrigger = false;
		break;
	}
}

// 책
HRESULT bookEnemy::init(ENEMYTYPE enemyType, POINTF spt)
{
	setImage();
	setAni();
	enemy::init(enemyType, spt);

	changeState(ENEMYSTATE::IDLE);

	_enemyType = enemyType;

	_enemyImg = IMAGEMANAGER->findImage("bookEnemyIdle");

	_bRc = RectMakeCenter(_bXY.x, _bXY.y, _enemyImg->getFrameWidth(), _enemyImg->getFrameHeight());

	_enemyMotion = KEYANIMANAGER->findAnimation("BOOK_ENEMY_IDLE");
	_enemyMotion->start();

	_currentHP = _maxHP = 25;

	return S_OK;
}

void bookEnemy::release()
{
}

void bookEnemy::update()
{
	enemy::update();
	if (_enemyState == ENEMYSTATE::DIE && _fly >= 0) _fly--;
	if (_bullet->getEnd())
	{
		_count2++;

		if (_count2 > 10)
		{
			_bullet->update();
		}
	}
	else _count2 = 0;
}

void bookEnemy::render()
{
	enemy::render();
}

void bookEnemy::setImage()
{
	IMAGEMANAGER->addFrameImage("bookEnemyIdle", "image/bookEnemyIdle.bmp", 180, 51, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("bookEnemyAttack", "image/bookEnemyAttack.bmp", 1584, 97, 24, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("bookEnemyDie", "image/bookEnemyDie.bmp", 102, 57, 2, 1, true, RGB(255, 0, 255));
}

void bookEnemy::setAni()
{
	KEYANIMANAGER->addCoordinateFrameAnimation("BOOK_ENEMY_IDLE", "bookEnemyIdle", 0, 4, 2, false, true);

	KEYANIMANAGER->addCoordinateFrameAnimation("BOOK_ENEMY_MOVE", "bookEnemyIdle", 0, 4, 2, false, true);

	KEYANIMANAGER->addCoordinateFrameAnimation("BOOK_ENEMY_ATTACK", "bookEnemyAttack", 0, 24, 12, false, false, returnIdle, this);

	KEYANIMANAGER->addCoordinateFrameAnimation("BOOK_ENEMY_HIT", "bookEnemyIdle", 0, 4, 2, false, false, returnIdle, this);

	KEYANIMANAGER->addCoordinateFrameAnimation("BOOK_ENEMY_DIE", "bookEnemyDie", 0, 2, 1, false, false);
}

void bookEnemy::attack()
{
	if (_emState == _idle)
	{
		_count++;

		if (_count % 230 == 0)
		{
			_count = 0;

			changeState(ENEMYSTATE::ATTACK);
			_attackType = RND->getInt(2);

			if (_attackType == 0)
			{
				// 공집합
				for (int k = -9; k < 9; k++)
				{
					_bullet->fire(_bXY.x + cosf(PI / 9 * k) * 60, _bXY.y - sinf(PI / 9 * k) * 60, PI / 9 * k, 3.0f, BTYPE::BOOK);
				}
				for (int i = 5; i > -3; i--)
				{
					_bullet->fire(_bXY.x, _bXY.y - 40 - 20 * i, PI - PI / 2, 3.0f, BTYPE::BOOK);
				}
				for (int j = -2; j < 6; j++)
				{
					_bullet->fire(_bXY.x, _bXY.y + 40 + 20 * j, PI + PI / 2, 3.0f, BTYPE::BOOK);
				}
			}
			else if (_attackType == 1)
			{
				//N
				for (int i = -2; i < 3; i++)
				{
					_bullet->fire(_bXY.x - 30, _bXY.y + 20 * i, getAngle(_sXY.x - 30, _sXY.y + 20 * i, _pXY.x, _pXY.y), 5.0f, BTYPE::BOOK);
				}
				for (int j = -2; j < 3; j++)
				{
					_bullet->fire(_bXY.x + 10 * j, _bXY.y + 20 * j, getAngle(_sXY.x + 15 * j, _sXY.y + 20 * j, _pXY.x, _pXY.y), 5.0f, BTYPE::BOOK);
				}
				for (int k = 2; k > -3; k--)
				{
					_bullet->fire(_bXY.x + 30, _bXY.y + 20 * k, getAngle(_sXY.x + 30, _sXY.y + 20 * k, _pXY.x, _pXY.y), 5.0f, BTYPE::BOOK);
				}
			}
		}
	}
}

void bookEnemy::changeState(ENEMYSTATE state)
{
	_idX = 0;
	_enemyState = state;

	switch (state)
	{
	case ENEMYSTATE::IDLE:
		_emState = _idle;
		_enemyImg = IMAGEMANAGER->findImage("bookEnemyIdle");
		break;
	case ENEMYSTATE::MOVE:
		_emState = _move;
		_enemyImg = IMAGEMANAGER->findImage("bookEnemyIdle");
		break;
	case ENEMYSTATE::ATTACK:
		_emState = _attack;
		_enemyImg = IMAGEMANAGER->findImage("bookEnemyAttack");
		break;
	case ENEMYSTATE::HIT:
		hitDamage(5);
		_isHit = true;
		_emState = _hit;
		_enemyImg = IMAGEMANAGER->findImage("bookEnemyIdle");
		break;
	case ENEMYSTATE::DIE:
		_isEnemyDie = true;
		_emState = _die;
		_enemyImg = IMAGEMANAGER->findImage("bookEnemyDie");
		break;
	}
	changeAni();
}

void bookEnemy::changeAni()
{
	switch (_enemyState)
	{
	case ENEMYSTATE::IDLE:
		if (_enemyDir == ENEMYDIRECTION::RIGHT || _enemyDir == ENEMYDIRECTION::DOWN)
		{
			_idY = 0;
		}
		if (_enemyDir == ENEMYDIRECTION::LEFT)
		{
			_idY = 0;
		}
		if (_enemyDir == ENEMYDIRECTION::UP)
		{
			_idY = 0;
		}
		_isLoop = true;
		_isTrigger = false;
		break;
	case ENEMYSTATE::MOVE:
		if (_enemyDir == ENEMYDIRECTION::RIGHT || _enemyDir == ENEMYDIRECTION::DOWN)
		{
			_idY = 0;
		}
		if (_enemyDir == ENEMYDIRECTION::LEFT)
		{
			_idY = 0;
		}
		if (_enemyDir == ENEMYDIRECTION::UP)
		{
			_idY = 0;
		}
		_isLoop = true;
		_isTrigger = true;
		break;
	case ENEMYSTATE::ATTACK:
		_idY = 0;
		_isLoop = false;
		_isTrigger = true;
		break;
	case ENEMYSTATE::HIT:
		if (_playerRc.left + 5 >= _sRc.right)
		{
			_idY = 0;
		}
		else if (_playerRc.left + 5 < _sRc.right)
		{
			_idY = 0;
		}
		_isLoop = false;
		_isTrigger = true;
		break;
	case ENEMYSTATE::DIE:
		if (_enemyDir == ENEMYDIRECTION::RIGHT || _enemyDir == ENEMYDIRECTION::DOWN)
		{
			_idY = 0;
		}
		if (_enemyDir == ENEMYDIRECTION::LEFT)
		{
			_idY = 0;
		}
		if (_enemyDir == ENEMYDIRECTION::UP)
		{
			_idY = 0;
		}
		_isLoop = false;
		_isTrigger = false;
		break;
	}
}