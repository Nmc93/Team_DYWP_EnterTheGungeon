#include "pch.h"
#include "mainMenu.h"

HRESULT mainMenu::init()
{
	createButton();

	IMAGEMANAGER->addFrameImage("menuImg", "image/menu/menu.bmp", 6480, WINSIZEY, 6, 1, true, RGB(255, 0, 255));
	_menuImg = IMAGEMANAGER->findImage("menuImg");
	_menuTimer = _menuIndex = 0;

	_bActive[0] = false;
	_bActive[1] = false;
	_bActive[2] = false;

	return S_OK;
}

void mainMenu::release()
{
}

void mainMenu::update()
{
	_sleepCount++;

	_menuTimer++;
	if (_menuTimer % 10 == 0)
	{
		_menuIndex++;
		if (_menuIndex > _menuImg->getMaxFrameX())
		{
			_menuIndex = 0;
		}
		_menuTimer = 0;
	}
	//마우스 버튼 입력
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		clickButton(true);
	}
	if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON))
	{
		_sleepCount = 0;
		clickButton(false);
	}

	if (KEYMANAGER->isOnceKeyDown(VK_TAB))
	{
		_bCount++;
		_sleepCount = 0;
	}

	if (KEYMANAGER->isOnceKeyDown(VK_NUMPAD9))
	{
		SCENEMANAGER->changeScene("STAGE1");
	}

	//히---든
	if (_bCount < 5 && _sleepCount > 10)
	{
		_bCount = 0;
	}
	else if (_bCount >= 5)
	{
		_bt[2].active = true;
	}
}

void mainMenu::render()
{
	_menuImg->frameRender(getMemDC(), 0, 0, _menuIndex, 0);
	
	SetBkMode(getMemDC(), TRANSPARENT);

	//버튼
	for (int i = 0; i < MBTSIZE; ++i)
	{
		//버튼이 활성화 상태일때만 보임
		if (_bt[i].active)
		{
			//폰트==================
			HFONT font2 = CreateFont(28, 0, 0, 0, 500, false, false, false,
				DEFAULT_CHARSET, OUT_STROKE_PRECIS, CLIP_DEFAULT_PRECIS,
				PROOF_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("궁서체"));
			HFONT oldFont2 = (HFONT)SelectObject(getMemDC(), font2);
			//폰트==================
			
			//Rectangle(getMemDC(), _bt[i].rc);
			if (!_bActive[i])
			{
				SetTextColor(getMemDC(), RGB(255, 255, 255));
				//TextOut(getMemDC(), _bt[i].rc.left, _bt[i].rc.top, _bt[i].name.c_str(), strlen(_bt[i].name.c_str()));
				DrawText(getMemDC(), _bt[i].name.c_str(),strlen(_bt[i].name.c_str()), &_bt[i].rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			else
			{
				SetTextColor(getMemDC(), RGB(100, 100, 100));
				//TextOut(getMemDC(), _bt[i].rc.left, _bt[i].rc.top, _bt[i].name.c_str(), strlen(_bt[i].name.c_str()));
				DrawText(getMemDC(), _bt[i].name.c_str(), strlen(_bt[i].name.c_str()), &_bt[i].rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}

			//폰트==================
			SelectObject(getMemDC(), oldFont2);
			DeleteObject(font2);
			//폰트==================
		}
	}
}

void mainMenu::createButton()
{
	_bt[0].img = nullptr;
	_bt[0].name = "플레이";
	_bt[0].pt = { 50 , WINSIZEY - 200 };
	_bt[0].rc = RectMake(_bt[0].pt.x, _bt[0].pt.y, 75, 35);
	_bt[0].active = true;

	_bt[1].img = nullptr;
	_bt[1].name = "옵션";
	_bt[1].pt = { 50 , _bt[0].pt.y + 50 };
	_bt[1].rc = RectMake(_bt[1].pt.x, _bt[1].pt.y, 75, 35);
	_bt[1].active = true;

	_bt[2].img = nullptr;
	_bt[2].name = "맵툴";
	_bt[2].pt = { 50 , _bt[1].pt.y + 50 };
	_bt[2].rc = RectMake(_bt[2].pt.x, _bt[2].pt.y, 75, 35);
	_bt[2].active = false;
}

void mainMenu::clickButton(bool push)
{
	//버튼을 눌렀을 때
	if(push)
	{ 
		for (int i = 0; i < MBTSIZE; ++i)
		{
			//게임 시작 버튼
			if (PtInRect(&_bt[i].rc, _ptMouse) && _bt[i].active)
			{
				_bActive[i] = true;
			}
			else
			{
				_bActive[i] = false;
			}
		}
	}
	//누른 버튼이 올라갔을때
	else if (!push)
	{
		//게임 시작 버튼
		if (PtInRect(&_bt[0].rc, _ptMouse) && _bt[0].active)
		{
			SCENEMANAGER->changeScene("BREAKSTAGE");
		}
		//옵션 버튼
		else if (PtInRect(&_bt[1].rc, _ptMouse) && _bt[1].active)
		{

		}
		//맵툴 버튼
		else if (PtInRect(&_bt[2].rc, _ptMouse) && _bt[2].active)
		{
			SCENEMANAGER->changeScene("MAPTOOL");
		}

		for (int i = 0; i < MBTSIZE; ++i)
		{
			_bActive[i] = false;
		}
	}
}
