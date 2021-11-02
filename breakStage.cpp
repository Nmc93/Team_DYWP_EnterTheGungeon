#include "pch.h"
#include "breakStage.h"

HRESULT breakStage::init()
{

	//�� ���� �Լ�
	createTileMap();

	//ī�޶�
	_camera = new camera;
	_camera->init();

	//Z����
	_zOrder = new zOrder;

	//�÷��̾�
	_marine = new player;
	_prisoner = new player;
	_pNum = 0;

	_marine->setTile0(_vMap, TMAPSIZEX0, TMAPSIZEY0);
	_prisoner->setTile0(_vMap, TMAPSIZEX0, TMAPSIZEY0);
	
	//�÷��̾� ĳ���� ����
	for (int i = 0; i < _vMap.size(); ++i)
	{
		//�غ� �����̶�� �÷��̾� ��ǥ�� �غ� ��ȯ
		if (_vMap[i].oState == SET_MARINE)
		{
			_marine->init({ (_vMap[i].mpt.x * TILESIZE) + TILESIZE/2 ,(_vMap[i].mpt.y * TILESIZE) + TILESIZE / 2 },false,PlayerType::MARINE,PlayerGunType::NORMAL);
		}
		//�˼� �����̶�� �÷��̾� ��ǥ�� �˼� ��ȯ
		else if (_vMap[i].oState == SET_PRISONER)
		{
			_prisoner->init({ (_vMap[i].mpt.x * TILESIZE) + TILESIZE / 2,(_vMap[i].mpt.y * TILESIZE) + TILESIZE / 2 }, false, PlayerType::PRISONER,PlayerGunType::NORMAL);
		}
	}

	//�̺�Ʈ�� ����
	_isNext = false;
	_nextNum = 0;

	//Ű �Է� ����
	_isKey[0] = _isKey[1] = _isKey[2] = _isKey[3] = false;
	_nextInfo = 0;
	return S_OK;
}

void breakStage::release()
{
}

void breakStage::update()
{

	//===============================================================================
	//======= �̵�  =================================================================
	//===============================================================================
	
	//Ű�Է� �� �̵�ġ ���
	keySelect();
	
	//�÷��̾� ������Ʈ
	_marine->update();
	_prisoner->update();

	//ī�޶� ������Ʈ
	_camera->update();

	//�غ��� Ȱ��ȭ���̶�� �غ� ����ġ �Է�
	if (_marine->getActive())
	{
		_camera->setCmove(_marine->getCMove());
	}
	//�˼��� Ȱ��ȭ���̶�� �˼� ����ġ �Է�
	else if (_prisoner->getActive())
	{
		_camera->setCmove(_prisoner->getCMove());
	}

	//Ÿ�� ������Ʈ
	for (int i = 0; i < TMAPSIZEX0 * TMAPSIZEY0; ++i)
	{
		//Ÿ�� ������Ʈ
		_vMap[i].rc = RectMake((_vMap[i].mpt.x * TILESIZE) - (_camera->getCurrent().x + _camera->getCMouse().x)
			, (_vMap[i].mpt.y * TILESIZE) - (_camera->getCurrent().y + _camera->getCMouse().y)
			, TILESIZE, TILESIZE);
	}

	//====== Z���� ���� �Լ� ===================================================================
	
	//ī�޶� ������ ����
	_zOrder->setCmove(_camera->getCurrent());
	_zOrder->setCmouse(_camera->getCMouse());
	
	//�غ�
	_zOrder->pushObject(_marine);
	//�˼�
	_zOrder->pushObject(_prisoner);
	
	//������Ʈ
	_zOrder->update();

	//======= �̺�Ʈ ������Ʈ ==================================================================
	nextStage();

	//======= �Ŵ��� ������Ʈ ==================================================================
	KEYANIMANAGER->update();
	TIMEMANAGER->getElapsedTime();
}

void breakStage::render()
{
	//��������
	for (int i = 0; i < TMAPSIZEX0 * TMAPSIZEY0; ++i)
	{
		//ȭ�� ������ ����� �����
		if (_vMap[i].rc.left > WINSIZEX || _vMap[i].rc.right < 0 ||
			_vMap[i].rc.top > WINSIZEY || _vMap[i].rc.bottom < 0) continue;

		if (!(_vMap[i].tFrame.x == 5 && _vMap[i].tFrame.y == 1) && 
			!(_vMap[i].tFrame.x == 6 && _vMap[i].tFrame.y == 1) &&
			!(_vMap[i].tFrame.x == 7 && _vMap[i].tFrame.y == 1))
		{
			//Rectangle(getMemDC(), _vMap[i].rc);
			IMAGEMANAGER->findImage("MAPSPRITE")->frameRender(getMemDC(), _vMap[i].rc.left, _vMap[i].rc.top, _vMap[i].tFrame.x, _vMap[i].tFrame.y);
			//IMAGEMANAGER->findImage("OBJSPRITE")->frameRender(getMemDC(), _map[i].rc.left, _map[i].rc.top, _map[i].oFrame.x, _map[i].oFrame.y);
		}
	}
	//=====================================================================================================================
	//============================================ �� ���̿� �׸��ÿ� =====================================================
	//=====================================================================================================================

	//Z���� ����
	_zOrder->render();
	if (_nextInfo == 1)
	{
		if (_marine->getActive())
		{
			_marine->render();
		}
		else if (_prisoner->getActive())
		{
			_prisoner->render();
		}
	}
	else if (_nextInfo == 2)
	{
		// ī�޶� ��ǥ
		_camera->render();
	}
	//_bullet->render();	
	
	//=====================================================================================================================
	//============================================ �� ���̿� �׸��ÿ� =====================================================
	//=====================================================================================================================
	//�Ʒ� ������
	for (int i = 0; i < TMAPSIZEX0 * TMAPSIZEY0; ++i)
	{
		//ȭ�� ������ ����� �����
		if (_vMap[i].rc.left > WINSIZEX || _vMap[i].rc.right < 0 ||
			_vMap[i].rc.top > WINSIZEY || _vMap[i].rc.bottom < 0) continue;

		if (!(_vMap[i].tFrame.x == 5 && _vMap[i].tFrame.y == 1) &&
			!(_vMap[i].tFrame.x == 6 && _vMap[i].tFrame.y == 1) &&
			!(_vMap[i].tFrame.x == 7 && _vMap[i].tFrame.y == 1)) continue;
		
		//Rectangle(getMemDC(), _vMap[i].rc);
		IMAGEMANAGER->findImage("MAPSPRITE")->frameRender(getMemDC(), _vMap[i].rc.left, _vMap[i].rc.top, _vMap[i].tFrame.x, _vMap[i].tFrame.y);
		//IMAGEMANAGER->findImage("OBJSPRITE")->frameRender(getMemDC(), _map[i].rc.left, _map[i].rc.top, _map[i].oFrame.x, _map[i].oFrame.y);
	}
	char str[128];
	SetBkMode(getMemDC(), TRANSPARENT);
	SetTextColor(getMemDC(), RGB(0, 0, 0));
	sprintf_s(str, "%0.4f", (_marine->getAngle()+PI)*180 / PI);
	TextOut(getMemDC(), 0, 0, str, strlen(str));
	sprintf_s(str, "x:%0.2f  y:%0.2f",_marine->getShadowXY().x / TILESIZE, _marine->getShadowXY().y / TILESIZE);
	TextOut(getMemDC(), 0, 20, str, strlen(str));
	
}

void breakStage::selectPlayer(bool select)
{
	//_marine = new player;
	//_prisoner = new player;

	//�غ� ����
	RECT trc = _marine->getBodyRc();
	if (!_marine->getActive() && select)
	{
		//�غ� ��������
		_marine->setActive(true);
		_camera->setIsSelect(true);

		//�غ��� ���õ����� �˼��� Ȱ��ȭ ���¶��
		if (_prisoner->getActive())
		{
			_prisoner->playerMove(PlayerDir::CENTER);
			_prisoner->setActive(false);
		}
	}
	//�˼� ����
	trc = _prisoner->getBodyRc();
	if (!_prisoner->getActive() && !select)
	{
		//�˼� ��������
		_prisoner->setActive(true);
		_camera->setIsSelect(true);

		//�غ��� ���õ����� �˼��� Ȱ��ȭ ���¶��
		if (_marine->getActive())
		{
			_marine->playerMove(PlayerDir::CENTER);
			_marine->setActive(false);
		}
	}
}

void breakStage::createTileMap()
{
	DSS->addTileMap(0, _vMap, true);

	/*
	for (int i = 0; i < TMAPSIZEX0 * TMAPSIZEY0; ++i)
	{
		_map[i].mpt = _vMap[i].mpt;
		_map[i].rc = _vMap[i].rc;
		_map[i].num = _vMap[i].num;
		_map[i].oFrame = _vMap[i].oFrame;
		_map[i].oState = _vMap[i].oState;
		_map[i].tFrame = _vMap[i].tFrame;
		_map[i].tState = _vMap[i].tState;
	}
	*/
}


//���� ������ �Ѿ�� �Լ�
void breakStage::nextStage()
{
	if (_marine->getState() == PlayerState::FALL ||
		_prisoner->getState() == PlayerState::FALL)
	{
		_isNext = true;
	}

	//���� ���������� �Ѿ�� �̺�Ʈ�� Ȱ��ȭ
	if (_isNext)
	{
		//���� ���������� �Ѿ�� �� ī��Ʈ
		_nextNum++;
		//ī��Ʈ�� �� á�ٸ� ���� ���������� �̵�
		if (_nextNum >= 150)
		{
			//������ ĳ���� Ȯ��
			int p = 0;
			if (_marine->getActive()) p = 0;
			else if(_prisoner->getActive()) p = 1;

			//���� �� �� �ѱ��
			DSS->setPlayerType(p);
			SCENEMANAGER->changeScene("STAGE1");
		}
	}
}

void breakStage::keySelect()
{
	if (_marine->getActive() && !_marine->getIsRoll() ||
		_prisoner->getActive() && !_prisoner->getIsRoll())
	{
		_isKey[0] = _isKey[1] = _isKey[2] = _isKey[3] = false;
	}
	//Ű�Է�

	//����â ����
	if (KEYMANAGER->isOnceKeyDown(VK_TAB))
	{
		_nextInfo++;
		if (_nextInfo >= 3) _nextInfo = 0;
	}
	//���콺 ��¦��ư
	if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
	{
		//selectPlayer();
	}
	//LEFTŰ
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		selectPlayer(true);
	}
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		selectPlayer(false);
	}
	//LEFT
	if (KEYMANAGER->isStayKeyDown('A') && !_marine->getIsRoll() && !_prisoner->getIsRoll())
	{
		_isKey[0] = true;
	}
	//RIGHT
	if (KEYMANAGER->isStayKeyDown('D') && !_marine->getIsRoll() && !_prisoner->getIsRoll())
	{
		_isKey[1] = true;
	}
	//TOP
	if (KEYMANAGER->isStayKeyDown('W') && !_marine->getIsRoll() && !_prisoner->getIsRoll())
	{
		_isKey[2] = true;
	}
	//BOTTOM
	if (KEYMANAGER->isStayKeyDown('S') && !_marine->getIsRoll() && !_prisoner->getIsRoll())
	{
		_isKey[3] = true;
	}

	//Ű���

	//�����̽���
	if (KEYMANAGER->isStayKeyDown(VK_SPACE) && (_isKey[0] || _isKey[1] || _isKey[2] || _isKey[3]))
	{
		if (!_marine->getIsRoll() && !_prisoner->getIsRoll())
		{
			if (_isKey[0] && _isKey[2])
			{
				if(_marine->getActive()) _marine->setFrame({ 8,5 });
				else if (_prisoner->getActive()) _prisoner->setFrame({ 8,5 });
			}
			else if (_isKey[1] && _isKey[2])
			{
				if (_marine->getActive()) _marine->setFrame({ 0,4 });
				else if (_prisoner->getActive()) _prisoner->setFrame({ 0,4 });
			}
			else if (_isKey[0] && _isKey[3])
			{
				if (_marine->getActive()) _marine->setFrame({ 8,2 });
				else if (_prisoner->getActive()) _prisoner->setFrame({ 8,2 });
			}
			else if (_isKey[1] && _isKey[3])
			{
				if (_marine->getActive()) _marine->setFrame({ 0,1 });
				else if (_prisoner->getActive()) _prisoner->setFrame({ 0,1 });
			}
			else if (_isKey[0])
			{
				if (_marine->getActive()) _marine->setFrame({ 8,2 });
				else if (_prisoner->getActive()) _prisoner->setFrame({ 8,2 });
			}
			else if (_isKey[1])
			{
				if (_marine->getActive()) _marine->setFrame({ 0,1 });
				else if (_prisoner->getActive()) _prisoner->setFrame({ 0,1 });
			}
			else if (_isKey[2])
			{
				if (_marine->getActive()) _marine->setFrame({ 0,3 });
				else if (_prisoner->getActive()) _prisoner->setFrame({ 0,3 });
			}
			else if (_isKey[3])
			{
				if (_marine->getActive()) _marine->setFrame({ 0,0 });
				else if (_prisoner->getActive()) _prisoner->setFrame({ 0,0 });
			}
			if (_marine->getActive()) _marine->addIsRoll();
			else if (_prisoner->getActive()) _prisoner->addIsRoll();
		}
	}
	//LT
	else if (_isKey[0] && _isKey[2])
	{
		_marine->playerMove(PlayerDir::LT);
		_prisoner->playerMove(PlayerDir::LT);
	}
	//RT
	else if (_isKey[1] && _isKey[2])
	{
		_marine->playerMove(PlayerDir::RT);
		_prisoner->playerMove(PlayerDir::RT);
	}
	//LB
	else if (_isKey[0] && _isKey[3])
	{
		_marine->playerMove(PlayerDir::LB);
		_prisoner->playerMove(PlayerDir::LB);
	}
	//RB
	else if (_isKey[1] && _isKey[3])
	{
		_marine->playerMove(PlayerDir::RB);
		_prisoner->playerMove(PlayerDir::RB);
	}
	//L
	else if (_isKey[0])
	{
		_marine->playerMove(PlayerDir::L);
		_prisoner->playerMove(PlayerDir::L);
	}
	//R
	else if (_isKey[1])
	{
		_marine->playerMove(PlayerDir::R);
		_prisoner->playerMove(PlayerDir::R);
	}
	//T
	else if (_isKey[2])
	{
		_marine->playerMove(PlayerDir::T);
		_prisoner->playerMove(PlayerDir::T);
	}
	//B
	else if (_isKey[3])
	{
		_marine->playerMove(PlayerDir::B);
		_prisoner->playerMove(PlayerDir::B);
	}
	else if (!_isKey[0] && !_isKey[1] && !_isKey[2] && !_isKey[3])
	{
		_marine->playerMove(PlayerDir::CENTER);
		_prisoner->playerMove(PlayerDir::CENTER);
	}
}