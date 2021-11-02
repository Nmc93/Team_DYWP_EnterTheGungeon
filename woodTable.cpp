#include "pch.h"
#include "woodTable.h"
#include "player.h"

HRESULT woodTable::init()
{
	tableImg();
	tableAni();

	_tableImg = IMAGEMANAGER->findImage("UPTABLE_IMG");
	_tableAni = KEYANIMANAGER->findAnimation("UPTABLE_IDLE");

	//_tableX = _player->getPlayerRc().right + 30;
	_tableY = WINSIZEY / 2;

	_tableRc = RectMakeCenter(_tableX, _tableY, _tableImg->getFrameWidth() - 93, _tableImg->getFrameHeight() - 30); //마젠타때문에 나눔

    return S_OK;
}

void woodTable::release()
{
}

void woodTable::update()
{

	if (KEYMANAGER->isStayKeyDown('E'))
	{
		tableReverseDir();
	}

	if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON) && !_isTableReverse) //안뒤집어져있고 우클릭 눌렀을때
	{
		tableDir(); //테이블 디렉션함수 호출
	}

	RECT temp;
	//RECT playerTemp = _player->getPlayerRc();

	//if (!IntersectRect(&temp, &playerTemp, &_tableRc) && _player->getIsTableUp() == true)
	//{
	//	_player->setIsTableUp(false);
	//	_player->changeSI(PlayerState::IDLE);
	//}

	tempMove();

}

void woodTable::render()
{

	_tableImg->aniRender(getMemDC(), _tableRc.left - 83, _tableRc.top - 32, _tableAni);

	if (KEYMANAGER->isToggleKey(VK_TAB))
	{
		Rectangle(getMemDC(), _tableRc);

		char str[128];
		sprintf_s(str, " _tableX , _tableY : %d , %d", _tableX, _tableY);
		TextOut(getMemDC(), 0, 420, str, strlen(str));

		sprintf_s(str, " _isTableReverse : %d", _isTableReverse);
		TextOut(getMemDC(), 0, 400, str, strlen(str));
	}

}

void woodTable::tableReverseDir()
{

	RECT temp;
	//RECT playerTemp = _player->getPlayerRc();

	//_player->setIsTablePush(true); //테이블 밀고있음

	//if (IntersectRect(&temp, &playerTemp, &_tableRc))  //플레이어가 테이블을 오른쪽으로 엎으면 마우스가 왼쪽에 있더라도 roll처럼 오른쪽을 보며 테이블엎는다.
	//{
	//	int width = temp.right - temp.left;
	//	int height = temp.bottom - temp.top;

	//	if (height > width)
	//	{
	//		//if (_player->getX() < _tableX)
	//		//{
	//		//	_player->setHeightDir(PlayerDir::DOWN);
	//		//	_player->setWidthDir(PlayerDir::RIGHT);
	//		//	_tableAni = KEYANIMANAGER->findAnimation("UPTABLE_REVERSE_RIGHT");
	//		//	_isTableReverse = true; //엎어진 테이블 더이상 위에서 슬라이드못함
	//		//}
	//		//else if (_player->getX() >= _tableX)
	//		//{
	//		//	_player->setHeightDir(PlayerDir::DOWN);
	//		//	_player->setWidthDir(PlayerDir::LEFT);
	//		//	_player->setAnimation(KEYANIMANAGER->findAnimation("PRISONER_TABLE_ANI_LEFT"));
	//		//	_tableAni = KEYANIMANAGER->findAnimation("UPTABLE_REVERSE_LEFT");
	//		//	_isTableReverse = true;
	//		//}
	//	}
	//	else
	//	{
	//		/*if (_player->getY() < _tableY)
	//		{
	//			_player->setHeightDir(PlayerDir::UP);
	//			_player->setAnimation(KEYANIMANAGER->findAnimation("PRISONER_TABLE_ANI_DOWN"));
	//			_tableAni = KEYANIMANAGER->findAnimation("UPTABLE_REVERSE_DOWN");
	//			_isTableReverse = true;
	//		}
	//		else if (_player->getY() >= _tableY)
	//		{
	//			_player->setHeightDir(PlayerDir::DOWN);
	//			_player->setAnimation(KEYANIMANAGER->findAnimation("PRISONER_TABLE_ANI_UP"));
	//			_tableAni = KEYANIMANAGER->findAnimation("UPTABLE_REVERSE_UP");
	//			_isTableReverse = true;
	//		}*/
	//	}
	//}

	_tableAni->start();

}

void woodTable::tableDir()
{

	/*RECT temp;
	RECT playerTemp = _player->getPlayerRc();

	if (IntersectRect(&temp, &playerTemp, &_tableRc))
	{
		_player->setIsTableUp(true);
		_player->changeSI(PlayerState::TABLESLIDE);

		int width = temp.right - temp.left;
		int height = temp.bottom - temp.top;

	}*/

}

void woodTable::tempMove()
{
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		_tableX += 1;
		_tableRc = RectMakeCenter(_tableX, _tableY, _tableImg->getFrameWidth() - 93, _tableImg->getFrameHeight());
	}
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		_tableX -= 1;
		_tableRc = RectMakeCenter(_tableX, _tableY, _tableImg->getFrameWidth() - 93, _tableImg->getFrameHeight());
	}
	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		_tableY -= 1;
		_tableRc = RectMakeCenter(_tableX, _tableY, _tableImg->getFrameWidth() - 93, _tableImg->getFrameHeight());
	}
	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		_tableY += 1;
		_tableRc = RectMakeCenter(_tableX, _tableY, _tableImg->getFrameWidth() - 93, _tableImg->getFrameHeight());
	}
}

void woodTable::tableAni()
{
	//테이블 세로 아이들
	KEYANIMANAGER->addCoordinateFrameAnimation("UPTABLE_IDLE", "UPTABLE_IMG", 0, 1, 7, false, true);

	//테이블 세로 엎기
	KEYANIMANAGER->addCoordinateFrameAnimation("UPTABLE_REVERSE_RIGHT", "UPTABLE_IMG", 1, 4, 7, false, false);
	KEYANIMANAGER->addCoordinateFrameAnimation("UPTABLE_REVERSE_LEFT", "UPTABLE_IMG", 17, 14, 7, false, false);
	KEYANIMANAGER->addCoordinateFrameAnimation("UPTABLE_REVERSE_UP", "UPTABLE_IMG", 19, 22, 7, false, false);
	KEYANIMANAGER->addCoordinateFrameAnimation("UPTABLE_REVERSE_DOWN", "UPTABLE_IMG", 28, 31, 7, false, false);
}

void woodTable::tableImg()
{
	IMAGEMANAGER->addFrameImage("UPTABLE_IMG", "image/upTable_Reverse.bmp", 1404, 564, 9, 4, true, RGB(255, 0, 255));
}
