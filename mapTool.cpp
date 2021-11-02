#include "pch.h"
#include "mapTool.h"

HRESULT mapTool::init()
{

	//����â �ʱ�ȭ
	_sampleInfo = false;
	_plusX = WINSIZEX/2;
	_activeRect = { (WINSIZEX / 2) + _plusX, 0, (WINSIZEX) + _plusX, WINSIZEY };

	//��ư �ʱ�ȭ
	createButton();

	//�ȷ�Ʈ �ʱ�ȭ
	paletteManage(0);

	//�����̵� �ʱ�ȭ
	_spuitType = S_MAP;
	_spuit.mpt = { 0,0 };
	_spuit.num = 0;
	_spuit.oFrame = {0,0};
	_spuit.rc = { 0,0,0,0 };
	_spuit.oState = SET_NULL;
	_spuit.tFrame = { 0,0 };
	_spuit.tState = MAP_MOVE;

	//���� Ȯ�ο� Ÿ�� �ʱ�ȭ
	_cTile.mpt = { 0,0 };
	_cTile.num = 0;
	_cTile.oFrame = { 0,0 };
	_cTile.rc = { 0,0,0,0 };
	_cTile.oState = SET_NULL;
	_cTile.tFrame = { 0,0 };
	_cTile.tState = MAP_MOVE;

	//ĵ���� �ʱ�ȭ
	_activeMap = false;
	_tMapSize = { 0 , 0 };

	return S_OK;
}

void mapTool::release()
{
}

void mapTool::update()
{
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		clickButton();	//��ư Ŭ��
	}
	if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
	{
		changeSpuit();	//�ȷ�Ʈ Ŭ��
		setTileData();	//Ÿ�� ��ĥ
	}
	if (KEYMANAGER->isOnceKeyDown(VK_TAB))
	{
		//����â ���� �ݱ�
		if(!_sampleInfo) _sampleInfo = true;
		else if(_sampleInfo) _sampleInfo = false;
	}

	if(KEYMANAGER->isStayKeyDown(VK_LEFT) && _activeMap)	_tX+= 10;
	if(KEYMANAGER->isStayKeyDown(VK_RIGHT) && _activeMap)	_tX-= 10;
	if(KEYMANAGER->isStayKeyDown(VK_UP) && _activeMap)	_tY+= 10;
	if(KEYMANAGER->isStayKeyDown(VK_DOWN) && _activeMap)	_tY-= 10;

	//���� ���� ����â
	moveSampleInfo();

	//Ÿ�ϸ� ���� ����â
	moveTileMap();

	//Ÿ�ϸ� �Ӽ� Ȯ��
	viewTileData();

	//����� ����
	selectArChive();

	//�׽�Ʈ
	//tileMapSave();
}

void mapTool::render()
{
	//
	SetTextColor(getMemDC(), RGB(0, 0, 0));
	//���� ����
	SetBkMode(getMemDC(), TRANSPARENT);

	//=========================================================================
	//============================ ĵ���� ��� =================================
	//=========================================================================

	if (_activeMap)
	{
		for (int i = 0; i < _vTile.size(); ++i)
		{
			//if (_vTile[i].rc.left - _tX > WINSIZEX || _vTile[i].rc.top - _tY < 0) continue;
			if (_vTile[i].rc.left > WINSIZEX || _vTile[i].rc.right < 0 ||
				_vTile[i].rc.top > WINSIZEY || _vTile[i].rc.bottom < 0) continue;

			//Rectangle(getMemDC(), _vTile[i].rc);
			IMAGEMANAGER->findImage("MAPSPRITE")->frameRender(getMemDC(), _vTile[i].rc.left, _vTile[i].rc.top, _vTile[i].tFrame.x, _vTile[i].tFrame.y);
			IMAGEMANAGER->findImage("OBJSPRITE")->frameRender(getMemDC(), _vTile[i].rc.left, _vTile[i].rc.top, _vTile[i].oFrame.x, _vTile[i].oFrame.y);
		}
	}

	//=========================================================================
	//============================ �ȷ�Ʈ ��� =================================
	//=========================================================================

	//Rectangle(getMemDC(), _activeRect);
	IMAGEMANAGER->findImage("MAPBAR")->render(getMemDC(), _activeRect.left, _activeRect.top);
	HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(getMemDC(), myBrush);
	//�ȷ�Ʈ ���
	for (int i = 0; i < PSIZEX * PSIZEY; ++i)
	{
		Rectangle(getMemDC(), _palette[i].rc);
		if (_spuitType == S_MAP)
		{
			_paletteImg->frameRender(getMemDC(),
				_palette[i].rc.left,
				_palette[i].rc.top,
				_palette[i].tFrame.x,
				_palette[i].tFrame.y);
		}
		else if(_spuitType == S_OBJECT)
		{
			_paletteImg->frameRender(getMemDC(),
				_palette[i].rc.left,
				_palette[i].rc.top,
				_palette[i].oFrame.x,
				_palette[i].oFrame.y);
		}
	}
	SelectObject(getMemDC(), oldBrush);
	DeleteObject(myBrush);
	//=========================================================================
	//============================ ����â ��� =================================
	//=========================================================================

	char str[128];
	// �����̵� Ÿ���� ���� ���
	if (_spuitType == S_MAP)
	{
		sprintf_s(str, "����� ��ȣ [ X : %d ][ Y : %d ]", _spuit.tFrame.x, _spuit.tFrame.y);
		TextOut(getMemDC(), _activeRect.left + 15, WINSIZEY - 270, str, strlen(str));

		if(_spuit.tState == MAP_MOVE) sprintf_s(str, "Ÿ�� �Ӽ� : �̵�����");
		else sprintf_s(str, "Ÿ�� �Ӽ� : �̵��Ұ�");
		TextOut(getMemDC(), _activeRect.left + 15, WINSIZEY - 250, str, strlen(str));

		IMAGEMANAGER->findImage("MAPSPRITE")->frameRender(getMemDC(), _activeRect.left + 190, WINSIZEY - 280, _spuit.tFrame.x, _spuit.tFrame.y);
	}
	// �����̵� Ÿ���� ������Ʈ�� ���
	else// if (_spuitType == S_OBJECT)
	{
		sprintf_s(str, "����� ��ȣ [ X : %d ][ Y : %d ]", _spuit.oFrame.x, _spuit.oFrame.y);
		TextOut(getMemDC(), _activeRect.left + 15, WINSIZEY - 270, str, strlen(str));

		if (_spuit.oState == SET_PLAYER) sprintf_s(str, "��ġ ���� : �÷��̾�");
		else if (_spuit.oState == SET_MARINE) sprintf_s(str, "��ġ ���� : �غ�");
		else if (_spuit.oState == SET_PRISONER) sprintf_s(str, "��ġ ���� : �˼�");
		else if (_spuit.oState == SET_POTAL) sprintf_s(str, "��ġ ���� : ��Ż");
		else if(_spuit.oState == SET_ENEMY) sprintf_s(str, "��ġ ���� : �ǵ���");
		else if(_spuit.oState == SET_TABLE) sprintf_s(str, "��ġ ���� : ���̺�");
		else if(_spuit.oState == SET_DOOR) sprintf_s(str, "��ġ ���� : ���Ա�");
		else if(_spuit.oState == SET_BOX) sprintf_s(str, "��ġ ���� : ����");
		else if(_spuit.oState == SET_LEFT_WAY) sprintf_s(str, "��ġ ���� : �������");
		else if(_spuit.oState == SET_RIGHT_WAY) sprintf_s(str, "��ġ ���� : ���������");
		else if(_spuit.oState == SET_TOP_WAY) sprintf_s(str, "��ġ ���� : �������");
		else if(_spuit.oState == SET_BOTTOM_WAY) sprintf_s(str, "��ġ ���� : �Ʒ������");
		else if(_spuit.oState == SET_EVENT) sprintf_s(str, "��ġ ���� : �̺�Ʈ����");
		else if(_spuit.oState == SET_BOSS) sprintf_s(str, "��ġ ���� : �ǽ�");
		else sprintf_s(str, "�� ��ġ");
		TextOut(getMemDC(), _activeRect.left + 15, WINSIZEY - 250, str, strlen(str));
	}

	//���콺�� ���� ĵ������ ���� ���
	if (_activeMap)
	{
		//ĵ���� xy��ǥ
		sprintf_s(str, "ĵ���� ��ǥ        :[X : %.f][Y : %.f]", _cTile.mpt.x, _cTile.mpt.y);
		TextOut(getMemDC(), _activeRect.left + 250, WINSIZEY - 270, str, strlen(str));
		//ĵ���� �迭 ��ȣ
		sprintf_s(str, "��ȣ                     :%d", _cTile.num);
		TextOut(getMemDC(), _activeRect.left + 250, WINSIZEY - 250, str, strlen(str));
		//������Ʈ �̹��� ������ xy��ǥ
		sprintf_s(str, "������Ʈ ������:[X : %d][Y : %d]", _cTile.oFrame.x, _cTile.oFrame.y);
		TextOut(getMemDC(), _activeRect.left + 250, WINSIZEY - 230, str, strlen(str));
		//������Ʈ ����
		if (_cTile.oState == SET_PLAYER) sprintf_s(str, "��ġ ����            : �÷��̾�");
		else if (_cTile.oState == SET_MARINE) sprintf_s(str, "��ġ ����            : �غ�");
		else if (_cTile.oState == SET_PRISONER) sprintf_s(str, "��ġ ����            : �˼�");
		else if (_cTile.oState == SET_ENEMY) sprintf_s(str, "��ġ ����            : �ǵ���");
		else if (_cTile.oState == SET_DOOR) sprintf_s(str, "��ġ ����            : ���Ա�");
		else if (_cTile.oState == SET_TABLE) sprintf_s(str, "��ġ ����            : ���̺�");
		else if (_cTile.oState == SET_BOX) sprintf_s(str, "��ġ ����            : ����");
		else if (_cTile.oState == SET_POTAL) sprintf_s(str, "��ġ ����            : ��Ż");
		else if (_cTile.oState == SET_LEFT_WAY) sprintf_s(str, "��ġ ����            : �������");
		else if (_cTile.oState == SET_RIGHT_WAY) sprintf_s(str, "��ġ ����            : ���������");
		else if (_cTile.oState == SET_TOP_WAY) sprintf_s(str, "��ġ ����            : �������");
		else if (_cTile.oState == SET_BOTTOM_WAY) sprintf_s(str, "��ġ ����            : �Ʒ������");
		else if (_cTile.oState == SET_EVENT) sprintf_s(str, "��ġ ����            : �̺�Ʈ����");
		else if (_cTile.oState == SET_BOSS) sprintf_s(str, "��ġ ����            : ����");
		else sprintf_s(str, "��ġ ����            : �� ��ġ");
		TextOut(getMemDC(), _activeRect.left + 250, WINSIZEY - 210, str, strlen(str));
		//Ÿ�� ������
		sprintf_s(str, "Ÿ�� ������        :[X : %d][Y : %d]", _cTile.tFrame.x, _cTile.tFrame.y);
		TextOut(getMemDC(), _activeRect.left + 250, WINSIZEY - 190, str, strlen(str));
		//Ÿ�� �Ӽ�
		if (_cTile.tState == MAP_MOVE) sprintf_s(str, "Ÿ�� �Ӽ�            : �̵�����");
		else sprintf_s(str, "Ÿ�� �Ӽ�            : �̵��Ұ�");
		TextOut(getMemDC(), _activeRect.left + 250, WINSIZEY - 170, str, strlen(str));
	}

	sprintf_s(str, "%d�� ����� : [X : %d][Y : %d]", _archiveNum, _tMapSize.x, _tMapSize.y);
	TextOut(getMemDC(), _activeRect.left + 15, WINSIZEY - 230, str, strlen(str));
	
	HBRUSH myBrush1 = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush1 = (HBRUSH)SelectObject(getMemDC(), myBrush1);
	for (int i = 0; i < TBTSIZE; i++)
	{
		if (_bt[i].active)
		{
			Rectangle(getMemDC(), _bt[i].rc);
			TextOut(getMemDC(), _bt[i].rc.left, _bt[i].rc.top, _bt[i].name.c_str(), strlen(_bt[i].name.c_str()));
		}
	}
	SelectObject(getMemDC(), oldBrush1);
	DeleteObject(myBrush1);
}

//========================================================================
//===========================  ��ư ���� �Լ�  ============================
//========================================================================

//��ư ����
void mapTool::createButton()
{
	//�ڷΰ��� ��ư
	_bt[0].img = nullptr;
	_bt[0].name = " MENU";
	_bt[0].pt = { ((WINSIZEX / 2) + 50) + _plusX , WINSIZEY - 40 };
	_bt[0].rc = RectMakeCenter(_bt[0].pt.x, _bt[0].pt.y, 75, 35);
	_bt[0].active = true;

	//�ȷ�Ʈ ��ȯ ��ư
	_bt[1].img = nullptr;
	_bt[1].name = " MAP";
	_bt[1].pt = { ((WINSIZEX / 2) + 50) + _plusX , WINSIZEY - 90 };
	_bt[1].rc = RectMakeCenter(_bt[1].pt.x, _bt[1].pt.y, 75, 35);
	_bt[1].active = true;

	//ĵ���� ����
	_bt[2].img = nullptr;
	_bt[2].name = " D : MAP";
	_bt[2].pt = { ((WINSIZEX / 2) + 50) + _plusX , WINSIZEY - 140 };
	_bt[2].rc = RectMakeCenter(_bt[2].pt.x, _bt[2].pt.y, 75, 35);
	_bt[2].active = true;

	//ĵ���� Xũ�� ����
	_bt[3].img = nullptr;
	_bt[3].name = " [UP | DW]";
	_bt[3].pt = { ((WINSIZEX / 2) + 50) + _plusX , WINSIZEY - 190 };
	_bt[3].rc = RectMakeCenter(_bt[3].pt.x, _bt[3].pt.y, 75, 35);
	_bt[3].active = true;

	//����
	_bt[4].img = nullptr;
	_bt[4].name = " [SV | LD]";
	_bt[4].pt = { ((WINSIZEX / 2) + 125) + _plusX , WINSIZEY - 190 };
	_bt[4].rc = RectMakeCenter(_bt[4].pt.x, _bt[4].pt.y, 75, 35);
	_bt[4].active = false;
}

//��ư ��ȣ�ۿ�
void mapTool::clickButton()
{
	//����â ��Ȱ��ȭ �� ��ŵ
	if (!_sampleInfo) return;

	//���� �޴� ������ ������ ��ư
	if (PtInRect(&_bt[0].rc, _ptMouse))
	{
		DSS->saveTileMap();
		SCENEMANAGER->changeScene("MAINMENU");
	}
	//�ȷ�Ʈ�� �����̵� ������ �������ִ� ��ư
	else if (PtInRect(&_bt[1].rc, _ptMouse))
	{
		if (_spuitType == S_MAP)
		{
			_spuitType = S_OBJECT;
			_bt[1].name = " OBJECT";
			paletteManage(1);
		}
		else if (_spuitType == S_OBJECT)
		{
			_spuitType = S_MAP;
			_bt[1].name = " MAP";
			paletteManage(0);
		}
	}
	//ĵ���� ����&���� ��ư
	else if (PtInRect(&_bt[2].rc, _ptMouse))
	{
		if (_activeMap)
		{
			_bt[2].name = " D : MAP";
			deleteTileMap();
		}
		else if (!_activeMap)
		{
			_bt[2].name = " C : MAP";
			createTileMap();
		}
	}
	//ĵ���� X�� ����&���� ��ư
	else if (PtInRect(&_bt[3].rc, _ptMouse) && !_activeMap)
	{
		if (_ptMouse.x < _bt[3].pt.x && _archiveNum < 8) _archiveNum++;							//_tMapSize.x++;	//LEFT
		else if (_ptMouse.x >= _bt[3].pt.x && _archiveNum > 0) _archiveNum--;	//_tMapSize.x > 0) _tMapSize.x--;	//RIGHT
	}
	//ĵ���� Y�� ����&���� ��ư
	else if (PtInRect(&_bt[4].rc, _ptMouse) && _activeMap)
	{
		//if (_ptMouse.x < _bt[4].pt.x) DSS->saveTileMap(_archiveNum,_vTile);//tileMapSave();	//LEFT
		//else if (_ptMouse.x >= _bt[4].pt.x) DSS->addTileMap(_archiveNum,_vTile , true);//tileMapload(0);	//RIGHT
	}
}

//========================================================================
//=========================  ĵ���� ���� �Լ�  ==========================
//========================================================================

//Ÿ�ϸ� ����
void mapTool::createTileMap()
{
	//Ÿ�ϸ� ����(Ÿ�ϸ��� �̹� �ִٸ� ��ŵ)
	if (_activeMap) return;
	/*
	//Ÿ�ϸ� ����
	for (int y = 0; y < _tMapSize.y; ++y)
	{
		for (int x = 0; x < _tMapSize.x; ++x)
		{
			_tile.mpt = { static_cast<float>(x),static_cast<float>(y) };
			_tile.num = y * PSIZEX + x;
			_tile.oFrame = { 0 , 0 };
			_tile.oState = SET_NULL;
			_tile.tFrame = { 0 , 0 };
			_tile.tState = MAP_UNMOVE;
			_tile.rc = RectMake(x * TILESIZE, y * TILESIZE, TILESIZE, TILESIZE);
			_vTile.push_back(_tile);
		}
	}) + _tX
	*/
	DSS->addTileMap(_archiveNum, _vTile , true);
	//���� Ȯ��
	_activeMap = true;
}

//Ÿ�ϸ� ����
void mapTool::deleteTileMap()
{
	//Ÿ�ϸ��� ���ٸ� ��ŵ
	if (!_activeMap) return;
	
	DSS->saveTileMap(_archiveNum, _vTile);

	//���� �� ������ ����
	_vTile.clear();
	vector<TILE>().swap(_vTile);

	//Ÿ�ϸ� �̵��Ÿ� �ʱ�ȭ
	_tX = _tY = 0;

	//���� Ȯ�ο� Ÿ�� �ʱ�ȭ
	_cTile.mpt = { 0,0 };
	_cTile.num = 0;
	_cTile.oFrame = { 0,0 };
	_cTile.rc = { 0,0,0,0 };
	_cTile.oState = SET_NULL;
	_cTile.tFrame = { 0,0 };
	_cTile.tState = MAP_MOVE;

	//���� Ȯ��
	_activeMap = false;
}

//ĵ������ �����̵忡 ����� ���� ����
void mapTool::setTileData()
{
	//Ÿ�ϸ��� ��Ȱ��ȭ���̶�� ��ŵ
	if (!_activeMap) return;
	//����â���� �ִٸ� ��ŵ
	if (PtInRect(&_activeRect,_ptMouse)) return;

	//ĵ���� Ŭ��
	for (int i = 0; i < _vTile.size(); ++i)
	{
		if (PtInRect(&_vTile[i].rc, _ptMouse))
		{
			//�����̵� Ÿ�Կ� ���� ���� ����
			if (_spuitType == S_MAP)
			{
				_vTile[i].tFrame = _spuit.tFrame;
				_vTile[i].tState = _spuit.tState;
			}
			else if (_spuitType == S_OBJECT)
			{
				_vTile[i].oFrame = _spuit.oFrame;
				_vTile[i].oState = _spuit.oState;
			}
		}
	}
}

//Ư�� Ÿ�� ���� Ȯ�ο�
void mapTool::viewTileData()
{
	//Ÿ�ϸ��� ��Ȱ��ȭ���̶�� ��ŵ
	if (!_activeMap) return;
	//����â���� �ִٸ� ��ŵ
	if (PtInRect(&_activeRect, _ptMouse)) return;

	int num = 0;
	bool check = false;

	//�ǵ�� üũ
	for (int i = 0; i < _vTile.size(); ++i)
	{
		if (PtInRect(&_vTile[i].rc, _ptMouse))
		{
			//_cTile = _vTile[i];
			num = i;
			check = true;
			break;
		}
	}

	//���� ó��
	if (check)
	{
		_cTile = _vTile[num];
	}
	else
	{
		//���� Ȯ�ο� Ÿ�� �ʱ�ȭ
		_cTile.mpt = { 0,0 };
		_cTile.num = 0;
		_cTile.oFrame = { 0,0 };
		_cTile.rc = { 0,0,0,0 };
		_cTile.oState = SET_NULL;
		_cTile.tFrame = { 0,0 };
		_cTile.tState = MAP_MOVE;
	}
}

//Ÿ�ϸ� �̵�
void mapTool::moveTileMap()
{
	//Ÿ�ϸ��� ��Ȱ��ȭ���̶�� ��ŵ
	if (!_activeMap) return;

	for (int i = 0; i < _vTile.size(); ++i)
	{
		_vTile[i].rc = RectMake((_vTile[i].mpt.x * TILESIZE) + _tX, (_vTile[i].mpt.y * TILESIZE) + _tY, TILESIZE, TILESIZE);
	}
}

//========================================================================
//=========================  ��ȣ �ۿ� ���� �Լ�  ==========================
//========================================================================

//�ȷ�Ʈ ���� �� ���� �Լ�
void mapTool::paletteManage(int palletnum)
{
	switch (palletnum)
	{
		// 0 : Ÿ�ϸ� �ȷ�Ʈ
		// 1 : ������Ʈ �ȷ�Ʈ
		case 0:
		{
			// �ȷ�Ʈ �̹���
			_paletteImg = IMAGEMANAGER->findImage("MAPSPRITE");
			//�ȷ�Ʈ �� ����
			for (int y = 0; y < PSIZEY; ++y)
			{
				for (int x = 0; x < PSIZEX; ++x)
				{
					_palette[y * PSIZEX + x].mpt = { static_cast<float>(x),static_cast<float>(y) };
					_palette[y * PSIZEX + x].rc = RectMake(((x * TILESIZE) + _activeRect.left) + 10, (y * TILESIZE) + 50, TILESIZE, TILESIZE);// (((x * TILESIZE) + (WINSIZEX / 2)) + _plusX, (y * TILESIZE), TILESIZE, TILESIZE);
					_palette[y * PSIZEX + x].num = y * PSIZEX + x;
					_palette[y * PSIZEX + x].oFrame = { 0 , 0 };
					_palette[y * PSIZEX + x].tFrame = { x , y };
					//Ÿ�ϸ� �Ӽ� �ο�
					if ((x == 1 && y == 0) || (x == 2 && y == 0) || (x == 3 && y == 0) || (x == 4 && y == 0) || (x == 5 && y == 0) || 
						(x == 6 && y == 0) || (x == 7 && y == 0) || (x == 8 && y == 0) || (x == 9 && y == 0) || (x == 1 && y == 1) ||
						(x == 2 && y == 1) || (x == 3 && y == 1) || (x == 4 && y == 1) || (x == 5 && y == 1) || (x == 6 && y == 1) || 
						(x == 7 && y == 1) || (x == 8 && y == 1) || (x == 9 && y == 1) || (x == 1 && y == 2) || (x == 2 && y == 2) ||
						(x == 3 && y == 2) || (x == 4 && y == 2) || (x == 8 && y == 2) || (x == 9 && y == 2) || (x == 1 && y == 3) ||
						(x == 2 && y == 3) || (x == 3 && y == 3) || (x == 4 && y == 3) || (x == 8 && y == 3) || (x == 9 && y == 3) ||
						(x == 2 && y == 4) || (x == 3 && y == 4) || (x == 4 && y == 4) || (x == 5 && y == 4) || (x == 6 && y == 4) ||
						(x == 7 && y == 4) || (x == 8 && y == 4) || (x == 9 && y == 4) || (x == 5 && y == 7))
					{
						_palette[y * PSIZEX + x].tState = MAP_MOVE;
					}
					else
					{
						_palette[y * PSIZEX + x].tState = MAP_UNMOVE;
					}
					//Ÿ�ϸ� ��ġ ������Ʈ �ο�
					_palette[y * PSIZEX + x].oState = SET_NULL;
				}
			}
			break;
		}
		case 1:
		{	
			// �ȷ�Ʈ �̹���
			_paletteImg = IMAGEMANAGER->findImage("OBJSPRITE");
			//�ȷ�Ʈ �� ����
			for (int y = 0; y < PSIZEY; ++y)
			{
				for (int x = 0; x < PSIZEX; ++x)
				{
					_palette[y * PSIZEX + x].mpt = { static_cast<float>(x),static_cast<float>(y) };
					_palette[y * PSIZEX + x].rc = RectMake(((x * TILESIZE) + _activeRect.left) + 10, (y * TILESIZE) + 50, TILESIZE, TILESIZE);//(((x * TILESIZE) + (WINSIZEX / 2)) + _plusX, (y * TILESIZE), TILESIZE, TILESIZE);
					_palette[y * PSIZEX + x].num = y * PSIZEX + x;
					_palette[y * PSIZEX + x].oFrame = { x , y };
					_palette[y * PSIZEX + x].tFrame = { 0 , 0 };
					//Ÿ�ϸ� �Ӽ� �ο�
					_palette[y * PSIZEX + x].tState = MAP_UNMOVE;
					//Ÿ�ϸ� ��ġ ������Ʈ �ο�
					if (x == 1 && y == 0)
					{
						_palette[y * PSIZEX + x].oState = SET_MARINE;
					}
					else if (x == 1 && y == 1)
					{
						_palette[y * PSIZEX + x].oState = SET_PRISONER;
					}
					else if (x == 1 && y == 2)
					{
						_palette[y * PSIZEX + x].oState = SET_PLAYER;
					}
					else if (x == 2 && y == 0)
					{
						_palette[y * PSIZEX + x].oState = SET_ENEMY;
					}
					else if (x == 3 && y == 0)
					{
						_palette[y * PSIZEX + x].oState = SET_DOOR;
					}
					else if (x == 4 && y == 1)
					{
						_palette[y * PSIZEX + x].oState = SET_TABLE;
					}
					else if (x == 5 && y == 1)
					{
						_palette[y * PSIZEX + x].oState = SET_BOX;
					}
					else if (x == 3 && y == 1)
					{
						_palette[y * PSIZEX + x].oState = SET_POTAL;
					}
					else if (x == 3 && y == 2)
					{
						_palette[y * PSIZEX + x].oState = SET_LEFT_WAY;
					}
					else if (x == 3 && y == 3)
					{
						_palette[y * PSIZEX + x].oState = SET_RIGHT_WAY;
					}
					else if (x == 3 && y == 4)
					{
						_palette[y * PSIZEX + x].oState = SET_TOP_WAY;
					}
					else if (x == 3 && y == 5)
					{
						_palette[y * PSIZEX + x].oState = SET_BOTTOM_WAY;
					}
					else if (x == 4 && y == 0)
					{
						_palette[y * PSIZEX + x].oState = SET_EVENT;
					}
					else if (x == 2 && y == 1)
					{
						_palette[y * PSIZEX + x].oState = SET_BOSS;
					}
					else
					{
						_palette[y * PSIZEX + x].oState = SET_NULL;
					}
				}
			}
			break;
		}
	}
}

//�����̵� ü����
void mapTool::changeSpuit()
{
	//����â ��Ȱ��ȭ �� ��ŵ
	if (!_sampleInfo) return;

	//����â�� Ȱ��ȭ���� �� �ȷ�Ʈ�� �����̵��� ��ȣ�ۿ�
	for (int y = 0; y < PSIZEY; ++y)
	{
		for (int x = 0; x < PSIZEX; ++x)
		{
			if (PtInRect(&_palette[y * PSIZEX + x].rc,_ptMouse))
			{
				if (_spuitType == S_MAP)
				{
					_spuit.tFrame = _palette[y * PSIZEX + x].tFrame;
					_spuit.tState = _palette[y * PSIZEX + x].tState;
				}
				else if (_spuitType == S_OBJECT)
				{
					_spuit.oFrame = _palette[y * PSIZEX + x].oFrame;
					_spuit.oState = _palette[y * PSIZEX + x].oState;
				}
			}
		}
	}
}

//����â �̵�
void mapTool::moveSampleInfo()
{
	if (_sampleInfo)
	{
		if (_plusX > 10) _plusX -= 10;
	}
	else if (!_sampleInfo)
	{
		if (_plusX < (WINSIZEX/2)) _plusX += 10;
	}

	//����â �̵�
	_activeRect = { (WINSIZEX / 2) + _plusX, 0, (WINSIZEX)+_plusX, WINSIZEY };
	
	//�ȷ�Ʈ �̵�
	for (int y = 0; y < PSIZEY; ++y)
	{
		for (int x = 0; x < PSIZEX; ++x)
		{
			_palette[y * PSIZEX + x].rc = RectMake(((x * TILESIZE) + _activeRect.left) + 17, (y * TILESIZE) + 15, TILESIZE, TILESIZE);
		}
	}

	//��ư �̵�
	_bt[0].pt = { ((_activeRect.left) + 50), WINSIZEY - 40 };
	_bt[0].rc = RectMakeCenter(_bt[0].pt.x, _bt[0].pt.y, 75, 35);
	_bt[1].pt = { ((_activeRect.left) + 50), WINSIZEY - 90 };
	_bt[1].rc = RectMakeCenter(_bt[1].pt.x, _bt[1].pt.y, 75, 35);
	_bt[2].pt = { ((WINSIZEX / 2) + 50) + _plusX , WINSIZEY - 140 };
	_bt[2].rc = RectMakeCenter(_bt[2].pt.x, _bt[2].pt.y, 75, 35);
	_bt[3].pt = { ((WINSIZEX / 2) + 50) + _plusX , WINSIZEY - 190 };
	_bt[3].rc = RectMakeCenter(_bt[3].pt.x, _bt[3].pt.y, 75, 35);
	_bt[4].pt = { ((WINSIZEX / 2) + 125) + _plusX , WINSIZEY - 190 };
	_bt[4].rc = RectMakeCenter(_bt[4].pt.x, _bt[4].pt.y, 75, 35);

}

//========================================================================
//========================  Ÿ�ϸ� ���� ���� �Լ�  =========================
//========================================================================

//����� ���� �Լ�
void mapTool::selectArChive()
{
	
	//����� ��ȣ�� ���� ���� ���� ����
	switch (_archiveNum)
	{
		//ƴ
		case 0:
		{
			_tMapSize.x = TMAPSIZEX0;
			_tMapSize.y = TMAPSIZEY0;
			break;
		}
		//���۹�
		case 1:
		{
			_tMapSize.x = TMAPSIZEX1;
			_tMapSize.y = TMAPSIZEY1;
			break;
		}
		//�� ��1
		case 2:
		{
			_tMapSize.x = TMAPSIZEX2;
			_tMapSize.y = TMAPSIZEY2;
			break;
		}
		//�� ��2
		case 3:
		{
			_tMapSize.x = TMAPSIZEX3;
			_tMapSize.y = TMAPSIZEY3;
			break;
		}
		//�� ��3
		case 4:
		{
			_tMapSize.x = TMAPSIZEX4;
			_tMapSize.y = TMAPSIZEY4;
			break;
		}
		//�� ��4
		case 5:
		{
			_tMapSize.x = TMAPSIZEX5;
			_tMapSize.y = TMAPSIZEY5;
			break;
		}
		//�� ��5
		case 6:
		{
			_tMapSize.x = TMAPSIZEX6;
			_tMapSize.y = TMAPSIZEY6;
			break;
		}
		//����
		case 7:
		{
			_tMapSize.x = TMAPSIZEX7;
			_tMapSize.y = TMAPSIZEY7;
			break;
		}
		//������
		case 8:
		{
			_tMapSize.x = TMAPSIZEX8;
			_tMapSize.y = TMAPSIZEY8;
			break;
		}
	}
}