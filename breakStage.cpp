#include "pch.h"
#include "breakStage.h"

HRESULT breakStage::init()
{

	//맵 생성 함수
	createTileMap();

	//카메라
	_camera = new camera;
	_camera->init();

	//Z오더
	_zOrder = new zOrder;

	//플레이어
	_marine = new player;
	_prisoner = new player;
	_pNum = 0;

	_marine->setTile0(_vMap, TMAPSIZEX0, TMAPSIZEY0);
	_prisoner->setTile0(_vMap, TMAPSIZEX0, TMAPSIZEY0);
	
	//플레이어 캐릭터 생성
	for (int i = 0; i < _vMap.size(); ++i)
	{
		//해병 비콘이라면 플레이어 좌표에 해병 소환
		if (_vMap[i].oState == SET_MARINE)
		{
			_marine->init({ (_vMap[i].mpt.x * TILESIZE) + TILESIZE/2 ,(_vMap[i].mpt.y * TILESIZE) + TILESIZE / 2 },false,PlayerType::MARINE,PlayerGunType::NORMAL);
		}
		//죄수 비콘이라면 플레이어 좌표에 죄수 수환
		else if (_vMap[i].oState == SET_PRISONER)
		{
			_prisoner->init({ (_vMap[i].mpt.x * TILESIZE) + TILESIZE / 2,(_vMap[i].mpt.y * TILESIZE) + TILESIZE / 2 }, false, PlayerType::PRISONER,PlayerGunType::NORMAL);
		}
	}

	//이벤트용 변수
	_isNext = false;
	_nextNum = 0;

	//키 입력 변수
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
	//======= 이동  =================================================================
	//===============================================================================
	
	//키입력 및 이동치 계산
	keySelect();
	
	//플레이어 업데이트
	_marine->update();
	_prisoner->update();

	//카메라 업데이트
	_camera->update();

	//해병이 활성화중이라면 해병 보정치 입력
	if (_marine->getActive())
	{
		_camera->setCmove(_marine->getCMove());
	}
	//죄수가 활성화중이라면 죄수 보정치 입력
	else if (_prisoner->getActive())
	{
		_camera->setCmove(_prisoner->getCMove());
	}

	//타일 업데이트
	for (int i = 0; i < TMAPSIZEX0 * TMAPSIZEY0; ++i)
	{
		//타일 업데이트
		_vMap[i].rc = RectMake((_vMap[i].mpt.x * TILESIZE) - (_camera->getCurrent().x + _camera->getCMouse().x)
			, (_vMap[i].mpt.y * TILESIZE) - (_camera->getCurrent().y + _camera->getCMouse().y)
			, TILESIZE, TILESIZE);
	}

	//====== Z오더 관련 함수 ===================================================================
	
	//카메라 보정값 전달
	_zOrder->setCmove(_camera->getCurrent());
	_zOrder->setCmouse(_camera->getCMouse());
	
	//해병
	_zOrder->pushObject(_marine);
	//죄수
	_zOrder->pushObject(_prisoner);
	
	//업데이트
	_zOrder->update();

	//======= 이벤트 업데이트 ==================================================================
	nextStage();

	//======= 매니저 업데이트 ==================================================================
	KEYANIMANAGER->update();
	TIMEMANAGER->getElapsedTime();
}

void breakStage::render()
{
	//윗돌맹이
	for (int i = 0; i < TMAPSIZEX0 * TMAPSIZEY0; ++i)
	{
		//화면 밖으로 벗어나면 미출력
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
	//============================================ 이 사이에 그리시오 =====================================================
	//=====================================================================================================================

	//Z오더 랜더
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
		// 카메라 좌표
		_camera->render();
	}
	//_bullet->render();	
	
	//=====================================================================================================================
	//============================================ 이 사이에 그리시오 =====================================================
	//=====================================================================================================================
	//아래 돌맹이
	for (int i = 0; i < TMAPSIZEX0 * TMAPSIZEY0; ++i)
	{
		//화면 밖으로 벗어나면 미출력
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

	//해병 선택
	RECT trc = _marine->getBodyRc();
	if (!_marine->getActive() && select)
	{
		//해병 선택택이
		_marine->setActive(true);
		_camera->setIsSelect(true);

		//해병이 선택됐을때 죄수가 활성화 상태라면
		if (_prisoner->getActive())
		{
			_prisoner->playerMove(PlayerDir::CENTER);
			_prisoner->setActive(false);
		}
	}
	//죄수 선택
	trc = _prisoner->getBodyRc();
	if (!_prisoner->getActive() && !select)
	{
		//죄수 선택택이
		_prisoner->setActive(true);
		_camera->setIsSelect(true);

		//해병이 선택됐을때 죄수가 활성화 상태라면
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


//다음 맵으로 넘어가는 함수
void breakStage::nextStage()
{
	if (_marine->getState() == PlayerState::FALL ||
		_prisoner->getState() == PlayerState::FALL)
	{
		_isNext = true;
	}

	//다음 스테이지로 넘어가는 이벤트가 활성화
	if (_isNext)
	{
		//다음 스테이지로 넘어가기 전 카운트
		_nextNum++;
		//카운트가 다 찼다면 다음 스테이지로 이동
		if (_nextNum >= 150)
		{
			//저장할 캐릭터 확인
			int p = 0;
			if (_marine->getActive()) p = 0;
			else if(_prisoner->getActive()) p = 1;

			//저장 및 씬 넘기기
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
	//키입력

	//정보창 변경
	if (KEYMANAGER->isOnceKeyDown(VK_TAB))
	{
		_nextInfo++;
		if (_nextInfo >= 3) _nextInfo = 0;
	}
	//마우스 왼짝버튼
	if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
	{
		//selectPlayer();
	}
	//LEFT키
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

	//키사용

	//스페이스바
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