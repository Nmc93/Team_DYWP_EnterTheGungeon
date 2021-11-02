#include "pch.h"
#include "boss.h"

HRESULT boss::init(int x, int y)
{
	imageAll();

	_cCount = _cIndex = _bCount = _bIndex = 0;

	_speed = 2;

	_actTimer, _actMaxTimer = _appearTimer = 0;

	_distance = 0;

	_angle = 0;

	_active = 0;

	_plus = 150;

	_isCheck = _isAppear = _isWho = _isActive = false;

	_cAS = CHAIRIDLE;

	_bAS = BOSSIDLE;

	_sx = x;

	_sy = y;

	_sImg = IMAGEMANAGER->findImage("shadow");

	_cImg = IMAGEMANAGER->findImage("bulletKingChair");

	_bImg = IMAGEMANAGER->findImage("bulletKing");

	_marineBack = IMAGEMANAGER->findImage("marineBack");

	_prisonerBack = IMAGEMANAGER->findImage("prisonerBack");

	_bulletKingBack = IMAGEMANAGER->findImage("bulletKingBack");

	_hpBar = new progressBar;

	_hpBar->init(-100, WINSIZEY - 100, 252, WINSIZEY - 89, 600, 36, 498, 24);

	_hpBar->setGauge(_currentHP, _maxHP);

	_currentHP = 250;

	_maxHP = 250;

    return S_OK;
}

void boss::update()
{
	if (_isActive)
	{
		bossAi();
		move();
		frameChange();
		bulletController();
		bulletCollision();
	
		_hpBar->setX(252);
		_hpBar->setY(WINSIZEY - 89);
		_hpBar->setGauge(_currentHP, _maxHP);
		_hpBar->update();
	}
	

	//_hpBar->setX(252);
	//_hpBar->setY(WINSIZEY - 89);
	//_hpBar->setGauge(_currentHP, _maxHP);
	//_hpBar->update();

	if (KEYMANAGER->isOnceKeyDown('O'))
	{
		_isWho = true;
		_isAppear = true;
	}
	if (KEYMANAGER->isStayKeyDown('P'))
	{
		_isWho = false;
		_isAppear = true;
	}
	imageAppear();
}

void boss::release()
{
}

void boss::render()
{
	char str[128];

	//sprintf_s(str,"%0.4f", _angle);
	//TextOut(getMemDC(), 300, 300, str, strlen(str));
	//sprintf_s(str, "%0.4f,%0.4f", _px, _py);
	//TextOut(getMemDC(), 300, 320, str, strlen(str));
	//sprintf_s(str, "%0.4f,%0.4f", _sx, _sy);
	//TextOut(getMemDC(), 300, 340, str, strlen(str));
	//_px, _py, _sx, _sy
	draw();
}

void boss::imageAll()
{
	IMAGEMANAGER->addFrameImage("bulletKingChair", "image/bulletKingChair.bmp", 884 * 3, 82 * 3, 13, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("bulletKing", "image/bulletKing.bmp", 1254 * 3, 44 * 3, 33, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("shadow", "image/bulletKingShadow.bmp", 200, 128, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("bullet", "image/enemyBullet.bmp", 22, 23, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("bulletKingBack", "image/bulletKingBack.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("prisonerBack", "image/prisonerBack.bmp", 1800, 383, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("marineBack", "image/marineBack.bmp", 1800, 420, 4, 1, true, RGB(255, 0, 255));
}

void boss::move()
{
	_bx = _cx;

	_by = _cy;

	_cx = _sx;

	_cy = _sy - _plus;

	_distance = getDistance(_px, _py, _sx, _sy);
	_angle = getAngle(_sx, _sy, _px, _py);
	
	if (_distance > 150.0f)
	{
		if (_bAS == BOSSIDLE || _bAS == BOSSATTACK1)
		{

			_sx += cosf(_angle) * 2;
			_sy += -sinf(_angle) * 2;
		}
	}
	else
	{
		_sx += cosf(_angle) * 0;
		_sy += -sinf(_angle) * 0;
	}
	_sRc = RectMakeCenter(_sx, _sy, _sImg->getWidth(), _sImg->getHeight());

	_chairRC = RectMakeCenter(_cx, _cy, _cImg->getFrameWidth(), _cImg->getFrameHeight());

	_bossRC = RectMakeCenter(_bx, _by, _bImg->getFrameWidth(), _bImg->getFrameHeight());
}

void boss::draw()
{
	//_sImg->render(getMemDC(), _sRc.left, _sRc.top);
	//_bullet->render();
	//_cImg->frameRender(getMemDC(), _chairRC.left, _chairRC.top);
	//_bImg->frameRender(getMemDC(), _bossRC.left, _bossRC.top);
	_hpBar->render();

	if (_isAppear == true)
	{
		_bulletKingBack->render(getMemDC());
		if (_isWho)_marineBack->frameRender(getMemDC(), _bRc.left, _bRc.top);
		else if (!_isWho)_prisonerBack->frameRender(getMemDC(), _bRc.left, _bRc.top);
	}
}

void boss::frameChange()
{
	//이미지 프레임 변환 함수
	_cCount++;
	if (_cCount % 10 == 0)
	{
		_cIndex++;
		_bIndex++;
		if (_bAS == BOSSIDLE && _cAS == CHAIRIDLE)//IDLE상태
		{
			_cIndex = 0;
			if (_bIndex > 1)
			{
				_bIndex = 0;
				_cCount = 0;
			}
		}
		else if (_bAS == BOSSATTACK1 && _cAS == CHAIRATTACK1)//패턴1
		{
			_cIndex = 0;
			if (_bIndex > 7 || _bIndex < 2)
			{
				_bIndex = 2;
				_cCount = 0;
			}
		}
		else if (_bAS == BOSSATTACK2 && _cAS == CHAIRATTACK2)//패턴2
		{
			_cIndex = 0;
			if (_bIndex > 17 || _bIndex < 7)
			{
				_bIndex = 7;
				_cCount = 0;
			}
			if (_bIndex == 13)_plus -= 30;
			else if (_bIndex == 14)_plus -= 30;
			else if (_bIndex == 15)_plus += 30;

		}
		else if (_bAS == BOSSATTACK3 && _cAS == CHAIRATTACK3)//패턴3
		{
			if (_bIndex > 23)
			{
				_bIndex = 23;
			}
			if (_cIndex > 6)
			{
				_cIndex = 3;
			}
		}
		else if (_bAS == BOSSSTUN && _cAS == CHAIRIDLE)//스턴
		{
			_cIndex = 0;
			if (_bIndex > 27 || _bIndex < 23)
			{
				_bIndex = 24;
				_cCount = 0;
			}
		}
		else if (_bAS == BOSSDIE && _cAS == CHAIRIDLE)//다이
		{
			_cIndex = 0;
			if (_bIndex == 32)
			{
				_bIndex = 29;
			}
			_cCount = 0;
		}
		else if (_bAS == BOSSHIT && _cAS == CHAIRIDLE)//히트
		{
			_cIndex = 0;
			if (_bIndex > 20 || _bIndex < 17)
			{
				_bIndex = 18;
				_cCount = 0;
			}
		}
		//프레임 돌려주기
		_cImg->setFrameX(_cIndex);
		_bImg->setFrameX(_bIndex);
	}
}

void boss::bossAi()
{
	//보스 죽었니? 살았니?=====================
	if (!_isCheck)
	{
		_actTimer++;
	}
	//========================================

	//보스 체력 충돌 판정 코드==================
	/*RECT temp;
	if (IntersectRect(&temp, &_chairRC, &_mRC))
	{
		hitDamage(5);
		if (_cAS == CHAIRIDLE && _bAS == BOSSIDLE)
		{
			_bAS = BOSSHIT;
			_cAS = CHAIRIDLE;
		}
	}*/
	//========================================

	//IDLE====================================
	if (_cAS == CHAIRIDLE && _bAS == BOSSIDLE)
	{
		if (_actMaxTimer <= _actTimer)
		{
			_actTimer = 0;

			_active = RND->getFromIntTo(0, 5);

			if (_active == 0 || _active == 1)
			{
				_cAS = CHAIRATTACK1;
				_bAS = BOSSATTACK1;
			}
			else if (_active == 2 || _active == 3)
			{
				_cAS = CHAIRATTACK2;
				_bAS = BOSSATTACK2;
			}
			else if (_active == 4)
			{
				_cAS = CHAIRATTACK3;
				_bAS = BOSSATTACK3;
				_bIndex = 21;
				_cIndex = 1;
			}
		}
	}
	//===================================================

	//패턴1(좌우 3발 공격)================================
	else if (_bAS == BOSSATTACK1 && _cAS == CHAIRATTACK1)
	{
		if (_bIndex >= 7)
		{
			_cAS = CHAIRIDLE;
			_bAS = BOSSIDLE;
		}
		_actMaxTimer = RND->getFromIntTo(250, 300);
	}
	//===================================================

	//패턴2(내려찍기 공격)=================================
	else if (_bAS == BOSSATTACK2 && _cAS == CHAIRATTACK2)
	{
		if (_bIndex >= 16)
		{
			_cAS = CHAIRIDLE;
			_bAS = BOSSIDLE;
			_plus = 150;
		}
		_actMaxTimer = RND->getFromIntTo(250, 300);
	}
	//====================================================
	
	//패턴3(회전 공격)=====================================
	else if (_bAS == BOSSATTACK3 && _cAS == CHAIRATTACK3)
	{
		if (_cIndex >= 6 && _actTimer >= 200)
		{
			_cAS = CHAIRIDLE;
			_bAS = BOSSSTUN;
			_actTimer = 0;
		}
	}
	//====================================================

	//패턴3 공격 후 일정시간 스턴===========================
	else if (_bAS == BOSSSTUN && _cAS == CHAIRIDLE)
	{
		if (_bAS == BOSSSTUN && _bIndex >= 27 && _actTimer >= 100)
		{
			_cAS = CHAIRIDLE;
			_bAS = BOSSIDLE;
		}
		_actMaxTimer = RND->getFromIntTo(250, 300);
	}
	//====================================================

	//보스 히트 판정 상태==================================
	else if (_bAS == BOSSHIT && _cAS == CHAIRIDLE)
	{
		if (_bIndex >= 20 && _actTimer >= 40)
		{
			_cAS = CHAIRIDLE;
			_bAS = BOSSIDLE;
		}
	}
	//=====================================================

	//보스 죽을때 상태======================================
	else if (_bAS == BOSSDIE && _cAS == CHAIRIDLE)
	{
		if (_actTimer >= 100)
		{
			_bIndex = 22;
		}
		if (_actTimer > 100 && _actTimer <= 120)
		{
			_plus -= 3;
		}
		if (_actTimer > 120)
		{
			_isCheck = true;
		}
	}
	if (_bAS != BOSSDIE && _currentHP <= 0)
	{
		_bAS = BOSSDIE;
		_cAS = CHAIRIDLE;
		_actTimer = 0;
		_bIndex = 28;
	}
	//======================================================
}

void boss::bulletController()
{
	//보스 불렛 관련 함수
	int plus;
	if (_bAS == BOSSATTACK1 && _cAS == CHAIRATTACK1)//패턴1 불렛 발사
	{
		if (_bIndex > 6)
		{
			for (int i = 0; i < 3; i++)
			{
				if (_ptMouse.x <= _sx)
				{
					_bullet->fire( _chairRC.left - 5, (_chairRC.top + _chairRC.bottom) / 2, 3 + PI / 12 * i, 6.0f);
				}
				if (_ptMouse.x > _sx)
				{
					_bullet->fire(_chairRC.right + 5, (_chairRC.top + _chairRC.bottom) / 2, PI / 12 * i - 0.5, 6.0f);
				}
			}
		}
	}
	else if (_bAS == BOSSATTACK2 && _cAS == CHAIRATTACK2)//패턴2 불렛 발사
	{

		if (_bIndex == 14)
		{
			if (_cCount % 9 == 0)
			{

				for (int i = 0; i < 36; i++)
				{
					_bullet->fire((_chairRC.left + _chairRC.right) / 2, (_chairRC.top + _chairRC.bottom) / 2, i * (PI / 16), 6.f);
				}
			}
		}
	}
	else if (_bAS == BOSSATTACK3 && _cAS == CHAIRATTACK3)//패턴3 불렛 발사
	{
		for (int i = 0; i < 11; i++)
		{

			if (_cIndex == 3)
			{
				if (_cCount % 10 == 0)
				{
					_bullet->fire((_chairRC.left + _chairRC.right) / 2, (_chairRC.top + _chairRC.bottom) / 2, i * (2 * PI / 6), 6.f);
				}
			}
			if (_cIndex == 4)
			{
				if (_cCount % 10 == 0)
				{
					plus = RND->getFromIntTo(20, 51);
					_bullet->fire((_chairRC.left + _chairRC.right) / 2, (_chairRC.top + _chairRC.bottom) / 2, i * (2 * PI / 6) + plus, 6.f);
				}
			}
			if (_cIndex == 5)
			{
				if (_cCount % 10 == 0)
				{
					plus = RND->getFromIntTo(20, 51);
					_bullet->fire((_chairRC.left + _chairRC.right) / 2, (_chairRC.top + _chairRC.bottom) / 2, i * (2 * PI / 6) + plus, 6.f);
				}
			}
			if (_cIndex == 6)
			{
				if (_cCount % 10 == 0)
				{
					plus = RND->getFromIntTo(20, 51);
					_bullet->fire((_chairRC.left + _chairRC.right) / 2, (_chairRC.top + _chairRC.bottom) / 2, i * (2 * PI / 6) + plus, 6.f);
				}
			}
		}
	}
	else if (_bAS == BOSSSTUN && _cAS == CHAIRIDLE)//패턴3 후 스턴시 불렛 발사
	{
		if (_bIndex <= 23 && _bIndex > 22)
		{
			if (_cCount % 9 == 0)
			{
				for (int i = 0; i < 64; i++)
				{
					_bullet->fire((_chairRC.left + _chairRC.right) / 2, (_chairRC.top + _chairRC.bottom) / 2, i * (PI / 32), 6.f);
				}
			}
		}
	}
}

void boss::hitDamage(float damage)
{
	//보스 피격시 데미지 판정 함수
	_currentHP -= damage;
}

void boss::imageAppear()
{
	//보스방 입장시 등장시 이미지 삽입 함수
	if (_isAppear)
	{
		static int bindex;
		_appearTimer++;
		_bCount++;
		_bRc = RectMake(0, 300, _prisonerBack->getFrameWidth(), _prisonerBack->getFrameHeight());
		if (_appearTimer >= 150)
		{
			_appearTimer = 0;
			_isActive = true;
			_isAppear = false;
		}
		if (_bCount % 10 == 0)
		{
			bindex++;
			if (bindex > 3)
			{
				bindex = 0;
			}
		}
		_marineBack->setFrameX(bindex);
		_prisonerBack->setFrameX(bindex);
	}
}

void boss::bulletCollision()
{
	RECT temp;
	for (int i = 0; i < _bullet->getVBullet().size(); i++)
	{
		if (_bullet->getVBullet()[i].btype == BTYPE::ENEMY)continue;

		if (IntersectRect(&temp, &_chairRC, &(_bullet->getVBullet()[i].rc)) 
			&& _bullet->getVBullet()[i].btype  == BTYPE::PLAYER)
		{
			hitDamage(5);
			_bullet->removeBullet(i);
			if (_cAS == CHAIRIDLE && _bAS == BOSSIDLE)
			{
				_bAS = BOSSHIT;
				_cAS = CHAIRIDLE;
			}
		}
	}
}
