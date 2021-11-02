/*
#include "pch.h"
#include "itemManager.h"
#include "player.h"

HRESULT itemManager::init()
{
	// �� �ʱ� ����
	setUp();
	_isFollow = false;

	return S_OK;
}

void itemManager::release()
{
}

void itemManager::update()
{
	/*if (KEYMANAGER->isOnceKeyDown('U')) followMoney();

	for (_viItem = _vItem.begin(); _viItem != _vItem.end(); _viItem++)
	{
		if ((*_viItem)->getItype() == ITYPE::DROPTAN)
		{ 
			float angle = getAngle((*_viItem)->getX(), (*_viItem)->getY(), _p->getX(), _p->getY());

			if ((*_viItem)->getIsFollow())
			{
				(*_viItem)->setX(10);
				(*_viItem)->setY((*_viItem)->getY() + sinf(angle) * 10);
			}
			else
			{
				(*_viItem)->setX((*_viItem)->getDx() - _cam->getX());
				(*_viItem)->setY((*_viItem)->getDy() - _cam->getY());
			}
		}

		(*_viItem)->update();
	}

	for (_viItem = _vItem.begin(); _viItem != _vItem.end();)
	{

		if ((*_viItem)->getItype() == ITYPE::DROPTAN && _isFollow)
		{
			float angle = getAngle(_p->getShadowXY().x, _p->getShadowXY().y, (*_viItem)->getX(), (*_viItem)->getY());

			// �����
			if (!(*_viItem)->getIsFollow())
			{
				(*_viItem)->setX((*_viItem)->getX() - cosf(angle) * 10);
				(*_viItem)->setY((*_viItem)->getY() + sinf(angle) * 10);
			}
		}

		(*_viItem)->update();

		// �浹 üũ
		RECT temp;
		RECT prc = _p->getBodyRc();
		RECT irc = (*_viItem)->getRect();
		if (IntersectRect(&temp, &prc, &irc)) _viItem = _vItem.erase(_viItem);
		else  ++_viItem;


	}
}

void itemManager::render()
{
	draw();

	for (_viItem = _vItem.begin(); _viItem != _vItem.end(); _viItem++)
	{
		(*_viItem)->render();
	}

	char str[128];
	sprintf_s(str, "_vItem.size : %d", (int)_vItem.size());
	TextOut(getMemDC(), 0, 400, str, strlen(str));

	sprintf_s(str, "_heart : %d", _p->getHeart());
	TextOut(getMemDC(), 0, 420, str, strlen(str));
}

void itemManager::draw()
{
	char str[128];

	// ��Ʈ
	HFONT font = CreateFont(35, 0, 0, 0, 2000, false, false, false, DEFAULT_CHARSET, 0, 0, 0, 50, TEXT("HY�ٴ�L"));
	HFONT oldFont = (HFONT)SelectObject(getMemDC(), font);

	// KEY
	sprintf_s(str, "%d", _p->getKey());
	TextOut(getMemDC(), 55, 92, str, strlen(str));

	// MONEY
	sprintf_s(str, "%d", _p->getMoney());
	TextOut(getMemDC(), 145, 92, str, strlen(str));

	SelectObject(getMemDC(), oldFont);
	DeleteObject(font);
}

void itemManager::setUp()
{
	// ��Ʈ
	for (int i = 0; i < _p->getHeart() / 2; i++)
	{
		heart* h = new heart;
		if (_vItem.empty()) h->init(10, 10);
		else h->init(_vItem[_vItem.size() - 1]->getRect().right, _vItem[_vItem.size() - 1]->getY());
		_vItem.push_back(h);
	}

	// �踮��
	for (int i = 0; i < _p->getBarrior(); i++)
	{
		barrior* b = new barrior;
		b->init(_vItem[_vItem.size() - 1]->getRect().right, _vItem[_vItem.size() - 1]->getRect().bottom);
		_vItem.push_back(b);
	}

	// ����ź
	for (int i = 0; i < _p->getBlankBullet(); i++)
	{
		blankBullet* bb = new blankBullet;
		if (i == 0) bb->init(0, _vItem[0]->getRect().bottom);
		else bb->init(_vItem[_vItem.size() - 1]->getRect().right, _vItem[0]->getRect().bottom);
		_vItem.push_back(bb);
	}

	// Ű
	key* k = new key;
	k->init(10, _vItem[_vItem.size() - 1]->getRect().bottom + 15);
	_vItem.push_back(k);

	// ź�� (��)
	money* m = new money;
	m->init(100, _vItem[_vItem.size() - 2]->getRect().bottom + 15);
	_vItem.push_back(m);
}

void itemManager::dropMoney(float x, float y)
{
	dropTan* dt = new dropTan;
	dt->init(x, y);
	dt->setDx(x + _cam->getX());
	dt->setDy(y + _cam->getY());
	_vItem.push_back(dt);
}

void itemManager::followMoney()
{
	for (_viItem = _vItem.begin(); _viItem != _vItem.end(); _viItem++)
	{
		if ((*_viItem)->getItype() == ITYPE::DROPTAN) (*_viItem)->setIsFollow(true);
	}
}

void itemManager::addHeart()
{
	// 1. ���� Ȯ��
	// 2. ��Ʈ �߰�

	// �� ��Ʈ, ���� ������ �б�
	for (int i = 0; i < _vItem.size(); i++)
	{
		// �� ��Ʈ
		if (_vItem[i]->getItype() == ITYPE::HEART)
		{
			if (_vItem[i]->getCurrentFrameX() != 0) _vItem[i]->setX(_vItem[i]->getX() + IMAGEMANAGER->findImage("HEART")->getFrameWidth());
		}

		// ����
		if (_vItem[i]->getItype() == ITYPE::BARRIOR)
		{
			_vItem[i]->setX(_vItem[i]->getX() + IMAGEMANAGER->findImage("HEART")->getFrameWidth());
		}
	}

	// ��Ʈ �߰�
	for (int i = _vItem.size() - 1; i >= 0; i--)
	{
		if (_vItem[i]->getItype() == ITYPE::HEART)
		{
			if (_vItem[i]->getCurrentFrameX() == 0)
			{
				heart* h = new heart;
				h->init(_vItem[i]->getRect().right, _vItem[0]->getY());
				_vItem.insert(_vItem.begin() + (i + 1), h);
				break;
			}
		}
	}
}

void itemManager::changeHeart(bool plus)
{
	// �� �� 2 : 6 5 / 1 : 4 3 / 0 : 2 1 / 0
	// ���� �� 0 : 1 / 1 : 2 3 / 2 : 4 5 / 3: 6 7

	// ���� ���� ��Ʈ�� ��ġ �˷��ֱ�
	int num = 0;
	if (plus) num = _p->getHeart() / 2;
	else num = (_p->getHeart() - 1) / 2;

	int count = -1;
	for (int i = 0; i < _vItem.size(); i++)
	{
		if (_vItem[i]->getItype() == ITYPE::HEART)
		{
			count++;
			if (count != num) continue;

			_vItem[i]->itemNum(plus);
		}
	}

}

void itemManager::changeBarrior(bool plus)
{
	int num = 0;

	// �踮�� �Ѱ��� �ֳ� ����
	bool obj = false;

	// ������ �� ��������
	for (int i = _vItem.size() - 1; i > 0; i--)
	{
		if (_vItem[i]->getItype() == ITYPE::BARRIOR)
		{
			num = i;
			obj = false;
			break;
		}
		else obj = true;
	}

	// �ϳ��� ���� ��쿡 �����ؾ� ��
	if (obj)
	{
		if (plus)
		{
			for (int i = _vItem.size() - 1; i > 0; i--)
			{
				if (_vItem[i]->getItype() == ITYPE::HEART)
				{
					barrior* b = new barrior;
					b->init(_vItem[i]->getRect().right, _vItem[0]->getRect().bottom);
					_vItem.push_back(b);
					break;
				}
			}
		}

		return;
	}

	// �������� ���õǾ� ����
	if (plus)
	{
		barrior* b = new barrior;
		b->init(_vItem[num]->getRect().right, _vItem[0]->getRect().bottom);
		_vItem.push_back(b);
	}
	else _vItem.erase(_vItem.begin() + num);
}

void itemManager::changeBlankBullet(bool plus)
{
	int num = 0;

	bool obj = false;

	for (int i = _vItem.size() - 1; i > 0; i--)
	{
		if (_vItem[i]->getItype() == ITYPE::BLANK_BULLET)
		{
			num = i;
			obj = false;
			break;
		}
		else obj = true;
	}

	// �ϳ��� ���� ��쿡 �����ؾ� ��
	if (obj)
	{
		if (plus)
		{
			blankBullet* bb = new blankBullet;
			bb->init(10, _vItem[0]->getRect().bottom);
			_vItem.push_back(bb);
		}
		return;
	}

	// �������� ���õǾ� ����
	if (plus)
	{
		blankBullet* bb = new blankBullet;
		bb->init(_vItem[num]->getRect().right, _vItem[0]->getRect().bottom);
		_vItem.push_back(bb);
	}
	else _vItem.erase(_vItem.begin() + num);
}

*/

#include "pch.h"
#include "itemManager.h"
#include "player.h"

HRESULT itemManager::init()
{
	// �� �ʱ� ����
	setUp();
	_isFollow = false;

	return S_OK;
}

void itemManager::release()
{
}

void itemManager::update()
{
		for (_viItem = _vItem.begin(); _viItem != _vItem.end();)
		{

			if ((*_viItem)->getItype() == ITYPE::DROPTAN && _isFollow)
			{
				float angle = getAngle(_p->getShadowXY().x, _p->getShadowXY().y, (*_viItem)->getX(), (*_viItem)->getY());

				// �����
				if (!(*_viItem)->getIsFollow())
				{
					(*_viItem)->setX((*_viItem)->getX() - cosf(angle) * 10);
					(*_viItem)->setY((*_viItem)->getY() + sinf(angle) * 10);
				}
			}

			(*_viItem)->update();

			// �浹 üũ
			RECT temp;
			RECT prc = _p->getBodyRc();
			RECT irc = (*_viItem)->getRect();
			if (IntersectRect(&temp, &prc, &irc)) _viItem = _vItem.erase(_viItem);
			else  ++_viItem;


		}


	/*for (int i=0; i<_vItem.size(); i++)
	{
		if (_vItem[i]->getItype() == ITYPE::DROPTAN)
		{
			float angle = getAngle(_p->getShadowXY().x, _p->getShadowXY().y, _vItem[i]->getX(), _vItem[i]->getY());

			// �����
			if (!_vItem[i]->getIsFollow())
			{
				_vItem[i]->setX(_vItem[i]->getX() - cosf(angle) * 10);
				_vItem[i]->setY(_vItem[i]->getY() + sinf(angle) * 10);
			}

			RECT temp;
			RECT prc = _p->getBodyRc();
			RECT irc = _vItem[i]->getRect();
			if (IntersectRect(&temp, &prc, &irc)) _vItem.erase(_vItem.begin() + i);

		}

		_vItem[i]->update();
	}*/
}

void itemManager::render()
{
	draw();

	for (_viItem = _vItem.begin(); _viItem != _vItem.end(); _viItem++)
	{
		if ((*_viItem)->getItype() == ITYPE::DROPTAN) continue;
		
		(*_viItem)->render();
	}

	char str[128];
	sprintf_s(str, "_vItem.size : %d", (int)_vItem.size());
	TextOut(getMemDC(), 0, 400, str, strlen(str));

	sprintf_s(str, "_heart : %d", _p->getHeart());
	TextOut(getMemDC(), 0, 420, str, strlen(str));
}

void itemManager::draw()
{
	char str[128];

	// ��Ʈ
	HFONT font = CreateFont(35, 0, 0, 0, 2000, false, false, false, DEFAULT_CHARSET, 0, 0, 0, 50, TEXT("HY�ٴ�L"));
	HFONT oldFont = (HFONT)SelectObject(getMemDC(), font);

	// KEY
	sprintf_s(str, "%d", _p->getKey());
	TextOut(getMemDC(), 55, 92, str, strlen(str));

	// MONEY
	sprintf_s(str, "%d", _p->getMoney());
	TextOut(getMemDC(), 145, 92, str, strlen(str));

	SelectObject(getMemDC(), oldFont);
	DeleteObject(font);
}

void itemManager::setUp()
{
	// ��Ʈ
	for (int i = 0; i < _p->getHeart() / 2; i++)
	{
		heart* h = new heart;
		if (_vItem.empty()) h->init(10, 10);
		else h->init(_vItem[_vItem.size() - 1]->getRect().right, _vItem[_vItem.size() - 1]->getY());
		_vItem.push_back(h);
	}

	// �踮��
	for (int i = 0; i < _p->getBarrior(); i++)
	{
		barrior* b = new barrior;
		b->init(_vItem[_vItem.size() - 1]->getRect().right, _vItem[_vItem.size() - 1]->getRect().bottom);
		_vItem.push_back(b);
	}

	// ����ź
	for (int i = 0; i < _p->getBlankBullet(); i++)
	{
		blankBullet* bb = new blankBullet;
		if (i == 0) bb->init(0, _vItem[0]->getRect().bottom);
		else bb->init(_vItem[_vItem.size() - 1]->getRect().right, _vItem[0]->getRect().bottom);
		_vItem.push_back(bb);
	}

	// Ű
	key* k = new key;
	k->init(10, _vItem[_vItem.size() - 1]->getRect().bottom + 15);
	_vItem.push_back(k);

	// ź�� (��)
	money* m = new money;
	m->init(100, _vItem[_vItem.size() - 2]->getRect().bottom + 15);
	_vItem.push_back(m);
}

void itemManager::dropMoney(POINTF dropXY)
{
	item * dt = new dropTan;
	dt->init(dropXY.x, dropXY.y);
	_vItem.push_back(dt);
}

void itemManager::addHeart()
{
	// 1. ���� Ȯ��
	// 2. ��Ʈ �߰�

	// �� ��Ʈ, ���� ������ �б�
	for (int i = 0; i < _vItem.size(); i++)
	{
		// �� ��Ʈ
		if (_vItem[i]->getItype() == ITYPE::HEART)
		{
			if (_vItem[i]->getCurrentFrameX() != 0) _vItem[i]->setX(_vItem[i]->getX() + IMAGEMANAGER->findImage("HEART")->getFrameWidth());
		}

		// ����
		if (_vItem[i]->getItype() == ITYPE::BARRIOR)
		{
			_vItem[i]->setX(_vItem[i]->getX() + IMAGEMANAGER->findImage("HEART")->getFrameWidth());
		}
	}

	// ��Ʈ �߰�
	for (int i = _vItem.size() - 1; i >= 0; i--)
	{
		if (_vItem[i]->getItype() == ITYPE::HEART)
		{
			if (_vItem[i]->getCurrentFrameX() == 0)
			{
				heart* h = new heart;
				h->init(_vItem[i]->getRect().right, _vItem[0]->getY());
				_vItem.insert(_vItem.begin() + (i + 1), h);
				break;
			}
		}
	}
}

void itemManager::changeHeart(bool plus)
{
	// �� �� 2 : 6 5 / 1 : 4 3 / 0 : 2 1 / 0
	// ���� �� 0 : 1 / 1 : 2 3 / 2 : 4 5 / 3: 6 7

	// ���� ���� ��Ʈ�� ��ġ �˷��ֱ�
	int num = 0;
	if (plus) num = _p->getHeart() / 2;
	else num = (_p->getHeart() - 1) / 2;

	int count = -1;
	for (int i = 0; i < _vItem.size(); i++)
	{
		if (_vItem[i]->getItype() == ITYPE::HEART)
		{
			count++;
			if (count != num) continue;

			_vItem[i]->itemNum(plus);
		}
	}

}

void itemManager::changeBarrior(bool plus)
{
	int num = 0;

	// �踮�� �Ѱ��� �ֳ� ����
	bool obj = false;

	// ������ �� ��������
	for (int i = _vItem.size() - 1; i > 0; i--)
	{
		if (_vItem[i]->getItype() == ITYPE::BARRIOR)
		{
			num = i;
			obj = false;
			break;
		}
		else obj = true;
	}

	// �ϳ��� ���� ��쿡 �����ؾ� ��
	if (obj)
	{
		if (plus)
		{
			for (int i = _vItem.size() - 1; i > 0; i--)
			{
				if (_vItem[i]->getItype() == ITYPE::HEART)
				{
					barrior* b = new barrior;
					b->init(_vItem[i]->getRect().right, _vItem[0]->getRect().bottom);
					_vItem.push_back(b);
					break;
				}
			}
		}

		return;
	}

	// �������� ���õǾ� ����
	if (plus)
	{
		barrior* b = new barrior;
		b->init(_vItem[num]->getRect().right, _vItem[0]->getRect().bottom);
		_vItem.push_back(b);
	}
	else _vItem.erase(_vItem.begin() + num);
}

void itemManager::changeBlankBullet(bool plus)
{
	int num = 0;

	bool obj = false;

	for (int i = _vItem.size() - 1; i > 0; i--)
	{
		if (_vItem[i]->getItype() == ITYPE::BLANK_BULLET)
		{
			num = i;
			obj = false;
			break;
		}
		else obj = true;
	}

	// �ϳ��� ���� ��쿡 �����ؾ� ��
	if (obj)
	{
		if (plus)
		{
			blankBullet* bb = new blankBullet;
			bb->init(10, _vItem[0]->getRect().bottom);
			_vItem.push_back(bb);
		}
		return;
	}

	// �������� ���õǾ� ����
	if (plus)
	{
		blankBullet* bb = new blankBullet;
		bb->init(_vItem[num]->getRect().right, _vItem[0]->getRect().bottom);
		_vItem.push_back(bb);
	}
	else _vItem.erase(_vItem.begin() + num);
}

