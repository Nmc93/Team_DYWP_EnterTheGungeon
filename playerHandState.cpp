#include "pch.h"
#include "playerHandState.h"

/*
HRESULT gun::init(int x, int y)
{
	IMAGEMANAGER->addFrameImage("권총", "image/gun/normalGun.bmp", 1200, 200, 24, 4, true, RGB(255,0,255));
	IMAGEMANAGER->addFrameImage("산탄총", "image/gun/shotGun.bmp", 1728, 360, 24, 5, true, RGB(255,0,255));
	IMAGEMANAGER->addFrameImage("손권총", "image/gun/hand_normalGun.bmp", 1200, 200, 24, 4, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("손산탄총", "image/gun/hand_shotGun.bmp", 1728, 360, 24, 5, true, RGB(255, 0, 255));

	//_cam = _bullet->getCamera();

	_currentFrameX = _currentFrameY = _count = 0;
	_isReload = _isFire = false;

	return S_OK;
}

void gun::release()
{

}

// 캐릭터 렉트 중심
void gun::update(int x, int y)
{
	pos(x, y);

	if (KEYMANAGER->isOnceKeyDown('R')) _isReload = true;
	
	if (_isReload) reload();
	else
	{
		rotate(x, y);
	}

	if (_isFire) fireFrame();
}

void gun::render()
{
	_img->frameRender(getMemDC(), _rc.left, _rc.top, _currentFrameX, _currentFrameY);
}

void gun::rotate(int x, int y)
{
	_angle = atan2f(x - _ptMouse.x, y - _ptMouse.y);
	
	if (0 <= _angle) _currentFrameX = _angle / PI12;					// angle : 0 ~ 180 -> frame : 0 ~ 11
	else		     _currentFrameX = (PI + _angle) / PI12 + 12;		// 180 + (angle :-180 ~ 0) -> frame : 12 ~ 23
}

// 일반 총(권총)
HRESULT normalGun::init(int x, int y)
{
	gun::init(x, y);

	_x = x;
	_y = y;

	_img = IMAGEMANAGER->findImage("손권총");
	_rc = RectMake(_x, _y, _img->getFrameWidth(), _img->getFrameHeight());

	return S_OK;
}

void normalGun::release()
{
}

void normalGun::update(int x, int y)
{
	gun::update(x, y);
}

void normalGun::render()
{
	gun::render();

	if (KEYMANAGER->isToggleKey(VK_NUMPAD7)) Rectangle(getMemDC(), _rc);

	char str[128];
	sprintf_s(str, "angle : %f", _angle * 180 / PI);
	TextOut(getMemDC(), 500, 500, str, strlen(str));

	sprintf_s(str, "_currentFrameX : %d", _currentFrameX);
	TextOut(getMemDC(), 500, 520, str, strlen(str));
}

void normalGun::fire(BTYPE btype)
{
	_isFire = true;
	_btype = btype;
}

void normalGun::fireFrame()
{
	_count++;

	if (_count > 5)
	{
		_count = 0;
		_currentFrameY++;

		if (_currentFrameY > 3)
		{
			_currentFrameY = 0;
			_isFire = false;
		}

		// 총 쏘기
		if (_currentFrameY == 2)
		{
			_bullet->fire(_x + _cam->getX(), _y + _cam->getY(), _angle + PI / 2, 7.0f, _btype);
		}
	}
}

void normalGun::reload()
{
	_count++;

	if (_count > 10)
	{
		_count = 0;
		_currentFrameY++;

		if (_currentFrameY > _img->getMaxFrameY())
		{
			_currentFrameY = 0;
			_isReload = false;
		}
	}
}

void normalGun::pos(int x, int y)
{
	float angle = (11 - _currentFrameX) * PI12 / 2;
	if (_currentFrameX > 11) angle = (_currentFrameX - 12) * PI12 / 2;

	if (_currentFrameX < 12)
	{
		_x = x - 35 + (11 - _currentFrameX) * 2;			// 11 10 9 8 7 6 5 4 3
		_y = (y + 20) - sinf(angle) * 30;
	}
	else
	{
		_x = x + 35 - (_currentFrameX - 12) * 2;			// 12 13 14 15 16 17 18 19 20
		_y = (y + 20)- sinf(angle) * 30;
	}

	_rc = RectMakeCenter(_x, _y, _img->getFrameWidth(), _img->getFrameHeight());
}

HRESULT shotGun::init(int x, int y)
{
	gun::init(x, y);

	_x = x;
	_y = y;

	_img = IMAGEMANAGER->findImage("손산탄총");
	_rc = RectMakeCenter(_x, _y, _img->getFrameWidth(), _img->getFrameHeight());

	return S_OK;
}

void shotGun::release()
{
}

void shotGun::update(int x, int y)
{
	gun::update(x, y);
	
}

void shotGun::render()
{
	gun::render();
}

// 카메라 값
void shotGun::fire(BTYPE btype)
{
	_isFire = true;
	_btype = btype;
}

void shotGun::fireFrame()
{
	_count++;

	if (_count > 5)
	{
		_count = 0;
		_currentFrameY++;

		if (_currentFrameY > 3)
		{
			_currentFrameY = 0;
			_isFire = false;
		}

		// 총 쏘기
		if (_currentFrameY == 2)
		{
			float angle = _angle + PI / 2;

			for (int i = -4; i < 0; i++)
			{
				_bullet->fire(_x + _cam->getX(), _y + _cam->getY(), (angle - PI12 / 2) - i * PI12 / 2, 7.0f, _btype);
			}
		}
	}

}

void shotGun::reload()
{
	_currentFrameY = 4;

	_count++;

	if (_count > 40)
	{
		_count = 0;
		_currentFrameY = 0;

		_isReload = false;
	}
}

void shotGun::pos(int x, int y)
{
	float angle = (11 - _currentFrameX) * PI12 / 2;
	if (_currentFrameX > 11) angle = (_currentFrameX - 12) * PI12 / 2;

	if (_currentFrameX < 12)
	{
		_x = x - 25 + (11 - _currentFrameX) * 2;		// 11 10 9 8 7 6 5 4 3
		_y = (y + 20) - sinf(angle) * 30;
	}
	else
	{
		_x = x + 25 - (_currentFrameX - 12) * 2;			// 12 13 14 15 16 17 18 19 20
		_y = (y + 20) - sinf(angle) * 30;
	}

	_rc = RectMakeCenter(_x, _y, _img->getFrameWidth(), _img->getFrameHeight());
}
*/

HRESULT playerGun::init(POINTF pPt, GunType guntype)
{
	// 총 타입
	_gType = guntype;
	
	_gState = GunState::READY;

	// 총 이미지
	if (guntype == GunType::HANDGUN)
	{
		_gImg = IMAGEMANAGER->findImage("GUN");
		_maxBullet = 8;
		_maxDelay = 20;
		_accuracy = 0;
		_reloadTime = 15;
	}
	// 총 이미지
	else if (guntype == GunType::SHOTGUN)
	{
		_gImg = IMAGEMANAGER->findImage("PUMP");
		_maxBullet = 5;
		_maxDelay = 50;
		_accuracy = 0;
		_reloadTime = 20;
	}
	
	//그림자에서 얼마나 떠있는지
	_fly = 30;

	// 총의 각도
	_angle = PI/2;
	
	//플레이어의 중점좌표
	_pXY = pPt;

	// 그림자의 중점좌표
	_sXY.x = -cosf(_angle) * LENGTH + _pXY.x;
	_sXY.y = -sinf(_angle) * LENGTH + _pXY.y;
	
	// 총의 중점좌표
	_bXY.x = _sXY.x;
	_bXY.y = _sXY.y - _fly;				
	
	//총 보정값
	_plusX = 0;
	_plusY = 0;

	//총의 프레임 카운트 초기화
	_fCount = 0;

	// 총의 렉트
	_rc = RectMakeCenter(_bXY.x, _bXY.y,
		_gImg->getFrameWidth(), 
		_gImg->getFrameHeight());
	
	// 총기 이미지의 프레임
	_frame = { 0,0 };

	// 발사 딜레이
	_sCount = 0;
	// 재장전 카운트
	_rCount = 0;

	// 재장전 여부
	_isFire = _isReload = false;
	
	//총 해금상태
	isShotGun = DSS->getSGun();

	return S_OK;
}

void playerGun::release()
{

}

void playerGun::update()
{
	//총의 상태에 따른 변화
	gunState();
	//오더용 키값 생성
	keyMake();
	//총 해금 상태 저장
	DSS->setsGun(isShotGun);

	//총기 딜레이
	if (_sCount < 50) _sCount++;

	//그림자의 중점좌표 업데이트
	_sXY.x = -cosf(_angle) * LENGTH + _pXY.x;// -_plusX;
	_sXY.y = -sinf(_angle) * LENGTH + _pXY.y;// -_plusY;
	
	//추가 좌표
	//_plusX = -cosf(_angle) * LENGTH + 20 + _pXY.x;
	//_plusY = (-sinf(_angle) * LENGTH + 20 + _pXY.y) - _fly;

	_shotAngle = -atan2f(_sXY.y - _pXY.y, _sXY.x - _pXY.x);

	//총의 중점좌표
	_bXY.x = _sXY.x;
	_bXY.y = _sXY.y - _fly;

	//총의 렉트
	_rc = RectMakeCenter(_bXY.x, _bXY.y,
		_gImg->getFrameWidth(),
		_gImg->getFrameHeight());

	rotate();
}

void playerGun::render()
{

	char str[128];
	sprintf_s(str, "%0.4f", (_angle + PI) * 180 / PI);
	TextOut(getMemDC(), 300, 0, str, strlen(str));
	sprintf_s(str, "[BMAX:%d][:%d]", _maxBullet, _bullet);
	TextOut(getMemDC(), 300, 20, str, strlen(str));
	sprintf_s(str, "[DMAX:%d][:%d]", _maxDelay, _sCount);
	TextOut(getMemDC(), 300, 40, str, strlen(str));
	sprintf_s(str, "[DMAX:%d][:%d]", _maxDelay, _sCount);
	TextOut(getMemDC(), 300, 40, str, strlen(str));
}


//총의 이미지 프레임
void playerGun::rotate()
{
	float angle = (_angle + PI) * 180 / PI;

	//1 : ++ : RB
	//if (MIDDLEX < _ptMouse.x && MIDDLEY < _ptMouse.y)
	//{
	if (angle > 0.0f && angle <= 15.0f) _frame.x = 17;
	else if (angle > 15.0f && angle <= 30.0f) _frame.x = 16;
	else if (angle > 30.0f && angle <= 45.0f) _frame.x = 15;
	else if (angle > 45.0f && angle <= 55.0f) _frame.x = 14;
	else if (angle > 55.0f && angle <= 75.0f) _frame.x = 13;
	else if (angle > 75.0f && angle <= 90.0f) _frame.x = 12;
	//_plusX = -20;
	//_plusY = -10;
	//}
	//2:-+ : LB
	//else if (MIDDLEX > _ptMouse.x && MIDDLEY < _ptMouse.y)
	//{
	else if (angle > 90.0f && angle <= 105.0f) _frame.x = 11;
	else if (angle > 105.0f && angle <= 120.0f) _frame.x = 10;
	else if (angle > 120.0f && angle <= 135.0f) _frame.x = 9;
	else if (angle > 135.0f && angle <= 150.0f) _frame.x = 8;
	else if (angle > 150.0f && angle <= 165.0f) _frame.x = 7;
	else if (angle > 165.0f && angle <= 180.0f) _frame.x = 6;
	//_plusX = 20;
	//_plusY = -10;
	//}
	//3:-- : LT
	//else if (MIDDLEX > _ptMouse.x && MIDDLEY > _ptMouse.y)
	//{
	else if (angle > 180.0f && angle <= 195.0f) _frame.x = 5;
	else if (angle > 195.0f && angle <= 210.0f) _frame.x = 4;
	else if (angle > 210.0f && angle <= 225.0f) _frame.x = 3;
	else if (angle > 225.0f && angle <= 240.0f) _frame.x = 2;
	else if (angle > 240.0f && angle <= 255.0f) _frame.x = 1;
	else if (angle > 255.0f && angle <= 270.0f) _frame.x = 0;
	//_plusX = 20;
	//_plusY = 0;
	//}
	//4:+- : RT
	//else if (MIDDLEX < _ptMouse.x && MIDDLEY > _ptMouse.y)
	//{
	else if (angle > 270.0f && angle <= 285.0f) _frame.x = 23;
	else if(angle > 285.0f && angle <= 300.0f) _frame.x = 22;
	else if(angle > 300.0f && angle <= 315.0f) _frame.x = 21;
	else if(angle > 315.0f && angle <= 330.0f) _frame.x = 20;
	else if(angle > 330.0f && angle <= 345.0f) _frame.x = 19;
	else if(angle > 345.0f && angle <= 360.0f) _frame.x = 18;
	//_plusX = -20;
	//_plusY = 0;
	//}
}

void playerGun::gunState()
{
	_fCount++;

	if (_fCount % 5 == 0)
	{
		//재장전중이라면
		if (_gState == GunState::RELOAD)
		{
			_rCount++;
			if (_reloadTime <= _rCount)
			{
				_bullet = _maxBullet;
				_rCount = 0;
				_gState = GunState::READY;
			}
			if (_gType == GunType::HANDGUN)
			{
				_frame.y = 2;
			}
			else if (_gType == GunType::SHOTGUN)
			{
				_frame.y = 4;
			}
		}
		else if (_gState == GunState::SHOOT)
		{
			if (_gType == GunType::HANDGUN)
			{
				_frame.y++;
				if (_frame.y == 1)
				{
					_bull->fire(_bXY.x, _bXY.y, _shotAngle, 12.0f, BTYPE::PLAYER);
					_bullet--;
				}
				if (_frame.y >= _gImg->getMaxFrameY())
				{
					_gState = GunState::READY;
					_frame.y = 0;
				}
			}
			else if (_gType == GunType::SHOTGUN)
			{
				_frame.y++;
				if (_frame.y == 1)
				{
					for (float i = -0.4f; i < 0.4f; i += 0.2f)
					{
						_bull->fire(_bXY.x, _bXY.y, _shotAngle - i, 7.0f, BTYPE::PLAYER);
					}
					_bullet--;
				}
				if (_frame.y >= _gImg->getMaxFrameY() - 1)
				{
					_frame.y = 0;
					_gState = GunState::READY;
				}
			}
		}
		else if (_gState == GunState::READY)
		{
			_frame.y = 0;
		}
	}
}

//총 변경 함수
void playerGun::gunChage()
{
	//샷건이 해금상태라면
	if (isShotGun)
	{
		if (_gType == GunType::HANDGUN)
		{
			_gType = GunType::SHOTGUN;
		}
		else if (_gType == GunType::SHOTGUN)
		{
			_gType = GunType::HANDGUN;
		}
	}

	switch (_gType)
	{
		//권총
		case GunType::HANDGUN:
		{
			_gImg = IMAGEMANAGER->findImage("GUN");
			_maxBullet = 8;
			_maxDelay = 20;
			_bullet = _maxBullet;
			_accuracy = 0;
			_reloadTime = 15;
			break;
		}
		//샷건
		case GunType::SHOTGUN:
		{
			_gImg = IMAGEMANAGER->findImage("PUMP");
			_maxBullet = 5;
			_maxDelay = 50;
			_bullet = _maxBullet;
			_accuracy = 0;
			_reloadTime = 20;
			break;
		}
	}
}
//총알 장전
void playerGun::addReload()
{
	if (_gState != GunState::READY) return;
	if (_bullet >= _maxBullet) return;
	
	_gState = GunState::RELOAD;
}

void playerGun::keyMake()
{
	if (_frame.x == 2 || _frame.x == 1 || _frame.x == 0 ||
		_frame.x == 23 || _frame.x == 22 || _frame.x == 21)
	{
		_imageKey = _sXY.y - 10;
	}
	else
	{
		_imageKey = _sXY.y + 10;
	}
}

//총 발사
void playerGun::isFire()
{
	//재장전중이면 발사 실패
	if (_isReload && _gState == GunState::RELOAD && _gState == GunState::SHOOT ) return;

	if (_sCount < _maxDelay) return;
	//총알이 있다면
	if (_bullet > 0)
	{
		_gState = GunState::SHOOT;
		//총알 감소 후
		//총알 발사//-cosf(_angle) * LENGTH + _pXY.x, sinf(_angle) * LENGTH + _pXY.y
		//_bull->fire(_bXY.x, _bXY.y, _shotAngle, 5.0f, BTYPE::PLAYER);
		_sCount = 0;
	}
}