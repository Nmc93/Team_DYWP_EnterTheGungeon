#include "pch.h"
#include "bullet.h"


HRESULT bullet::init(const char* imageName, int bulletMax, float range)
{
	_imageName = imageName;
	_bulletMax = bulletMax;
	_range = range;

	_end = false;

	return S_OK;
}

void bullet::release()
{
}

void bullet::update()
{
	move();

	bool obj = true;

	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		if (_viBullet->isFire) continue;
		obj = false;
		_viBullet->isFire = true;
		_timer++;
		break;
	}

	//책의 총알이 전부 끝났는지
	if (obj)
	{
		int count = 0;
		for (int i = 0; i < _vBullet.size(); i++)
		{
			count++;
			if (_vBullet[i].btype == BTYPE::BOOK) _vBullet[i].isFired = true;
			if (count > _timer) break;
		}

		_timer = 0;
		_end = true;
	}
	else _end = false;
}

void bullet::render()
{
	bool obj = true;

	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		if (_viBullet->isFire) continue;
		obj = false;
		_viBullet->isFire = true;
		_timer++;
		break;
	}
	/*
	//책의 총알이 전부 끝났는지
	if (obj)
	{
		int count = 0;
		for (int i = 0; i < _vBullet.size(); i++)
		{
			count++;

			if (_vBullet[i].btype == BTYPE::BOOK)
			{
				_vBullet[i].isFired = true;
			}

			if (count > _timer) break;
		}

		_timer = 0;
		_end = true;
	}
	else _end = false;

	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		if (!_viBullet->isFire) continue;
		_viBullet->bulletImage->render(getMemDC(), _viBullet->rc.left, _viBullet->rc.top);
	}
	*/
}

void bullet::fire(float x, float y, float angle, float speed, BTYPE btype)
{
	if (_bulletMax < _vBullet.size()) return;

	tagBullet bullet;
	ZeroMemory(&bullet, sizeof(tagBullet));
	bullet.bulletImage = IMAGEMANAGER->findImage(_imageName);
	bullet.speed = speed;
	bullet.radius = bullet.bulletImage->getWidth() / 2;
	bullet.x = bullet.fireX = x;
	bullet.y = bullet.fireY = y;
	bullet.angle = angle;
	bullet.btype = btype;

	if (btype == BTYPE::BOOK)
	{
		bullet.isFire = false;

		bullet.rc = RectMakeCenter(bullet.x, bullet.y,
			bullet.bulletImage->getWidth(),
			bullet.bulletImage->getHeight());
	}
	else
	{
		bullet.isFire = true;

		bullet.rc = RectMakeCenter(bullet.x, bullet.y,
			bullet.bulletImage->getWidth(),
			bullet.bulletImage->getHeight());
	}

	_vBullet.push_back(bullet);
}

void bullet::move()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		if (_viBullet->btype == BTYPE::BOOK)
		{
			if (_end || _viBullet->isFired)
			{
				_viBullet->x += cosf(_viBullet->angle) * _viBullet->speed;
				_viBullet->y += -sinf(_viBullet->angle) * _viBullet->speed;
			}
		}
		else
		{
			_viBullet->x += cosf(_viBullet->angle) * _viBullet->speed;
			_viBullet->y += -sinf(_viBullet->angle) * _viBullet->speed;
		}

		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y,
			_viBullet->bulletImage->getWidth(),
			_viBullet->bulletImage->getHeight());

		if (getDistance(_viBullet->fireX, _viBullet->fireY, _viBullet->x, _viBullet->y) > _range)
		{
			_viBullet = _vBullet.erase(_viBullet);
		}
		else
		{
			++_viBullet;
		}
	}
}

void bullet::removeBullet(int arrNum)
{
	_vBullet.erase(_vBullet.begin() + arrNum);
}