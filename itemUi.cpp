#include "pch.h"
#include "itemUI.h"

HRESULT heart::init(int x, int y)
{
	IMAGEMANAGER->addFrameImage("HEART", "image/ui/heart.bmp", 153, 39, 3, 1, true, RGB(255, 0, 255));
	_img = IMAGEMANAGER->findImage("HEART");

	_iType = ITYPE::HEART;

	_x = x;
	_y = y;

	_rc = RectMake(_x, _y, _img->getFrameWidth(), _img->getFrameHeight());

	_hType = HTYPE::ONE;
	changeItem();

	return S_OK;
}

void heart::release()
{

}

void heart::update()
{
	_rc = RectMake(_x, _y, _img->getFrameWidth(), _img->getFrameHeight());
}

void heart::render()
{
	_img->frameRender(getMemDC(), _rc.left, _rc.top, _curretFrameX, 0);
}

void heart::itemNum(bool plus)
{
	if (plus)
	{
		if (_hType == HTYPE::HALF) _hType = HTYPE::ONE;
		else if (_hType == HTYPE::ZERO) _hType = HTYPE::HALF;
	}
	else
	{
		if (_hType == HTYPE::ONE) _hType = HTYPE::HALF;
		else if (_hType == HTYPE::HALF) _hType = HTYPE::ZERO;
	}

	changeItem();
}


void heart::changeItem()
{
	switch (_hType)
	{
	case HTYPE::ONE:
		_curretFrameX = 0;
		break;

	case HTYPE::HALF:
		_curretFrameX = 1;
		break;

	case HTYPE::ZERO:
		_curretFrameX = 2;
		break;
	}
}

HRESULT barrior::init(int x, int y)
{
	IMAGEMANAGER->addImage("BARRIOR", "image/ui/barrior.bmp", 26, 28, true, RGB(255, 0, 255));
	_img = IMAGEMANAGER->findImage("BARRIOR");

	_iType = ITYPE::BARRIOR;

	_x = x + 10;
	_y = y - _img->getHeight();

	_rc = RectMake(_x, _y, _img->getWidth(), _img->getHeight());

	return S_OK;
}
void barrior::release()
{
}
void barrior::update()
{
	_rc = RectMake(_x, _y, _img->getWidth(), _img->getHeight());
}

void barrior::render()
{
	_img->render(getMemDC(), _rc.left, _rc.top);
}


HRESULT blankBullet::init(int x, int y)
{
	IMAGEMANAGER->addImage("BLANK_BULLET", "image/ui/blank_bullet.bmp", 24, 24, true, RGB(255, 0, 255));
	_img = IMAGEMANAGER->findImage("BLANK_BULLET");

	_iType = ITYPE::BLANK_BULLET;

	_x = x + 10;
	_y = y + 10;

	_rc = RectMake(_x, _y, _img->getWidth(), _img->getHeight());

	return S_OK;
}
void blankBullet::release()
{
}
void blankBullet::update()
{
}
void blankBullet::render()
{
	_img->render(getMemDC(), _rc.left, _rc.top);
}

HRESULT key::init(int x, int y)
{
	IMAGEMANAGER->addImage("KEY", "image/ui/key.bmp", 32, 28, true, RGB(255, 0, 255));
	_img = IMAGEMANAGER->findImage("KEY");

	_iType = ITYPE::KEY;

	_x = x;
	_y = y;

	_rc = RectMake(_x, _y, _img->getWidth(), _img->getHeight());

	return S_OK;
}

void key::release()
{
}

void key::update()
{
}

void key::render()
{
	_img->render(getMemDC(), _rc.left, _rc.top);
}

HRESULT money::init(int x, int y)
{
	IMAGEMANAGER->addImage("TAN", "image/ui/tan.bmp", 26, 28, true, RGB(255, 0, 255));
	_img = IMAGEMANAGER->findImage("TAN");

	_iType = ITYPE::MONEY;

	_x = x;
	_y = y;

	_rc = RectMake(_x, _y, _img->getWidth(), _img->getHeight());

	return S_OK;
}
void money::release()
{
}
void money::update()
{
}
void money::render()
{
	_img->render(getMemDC(), _rc.left, _rc.top);
}

HRESULT dropTan::init(int x, int y)
{
	IMAGEMANAGER->addImage("DROPTAN", "image/ui/tan.bmp", 13, 14, true, RGB(255, 0, 255));
	_img = IMAGEMANAGER->findImage("DROPTAN");

	_iType = ITYPE::DROPTAN;

	_x = x;
	_y = y;

	_rc = RectMake(_x, _y, _img->getWidth(), _img->getHeight());

	return S_OK;
}

void dropTan::release()
{
}

void dropTan::update()
{
	_rc = RectMake(_x, _y, _img->getWidth(), _img->getHeight());
}

void dropTan::render()
{
	_img->render(getMemDC(), _rc.left, _rc.top);
}
