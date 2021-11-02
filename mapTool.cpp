#include "pch.h"
#include "mapTool.h"

HRESULT mapTool::init()
{

	//정보창 초기화
	_sampleInfo = false;
	_plusX = WINSIZEX/2;
	_activeRect = { (WINSIZEX / 2) + _plusX, 0, (WINSIZEX) + _plusX, WINSIZEY };

	//버튼 초기화
	createButton();

	//팔레트 초기화
	paletteManage(0);

	//스포이드 초기화
	_spuitType = S_MAP;
	_spuit.mpt = { 0,0 };
	_spuit.num = 0;
	_spuit.oFrame = {0,0};
	_spuit.rc = { 0,0,0,0 };
	_spuit.oState = SET_NULL;
	_spuit.tFrame = { 0,0 };
	_spuit.tState = MAP_MOVE;

	//정보 확인용 타일 초기화
	_cTile.mpt = { 0,0 };
	_cTile.num = 0;
	_cTile.oFrame = { 0,0 };
	_cTile.rc = { 0,0,0,0 };
	_cTile.oState = SET_NULL;
	_cTile.tFrame = { 0,0 };
	_cTile.tState = MAP_MOVE;

	//캔버스 초기화
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
		clickButton();	//버튼 클릭
	}
	if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
	{
		changeSpuit();	//팔레트 클릭
		setTileData();	//타일 색칠
	}
	if (KEYMANAGER->isOnceKeyDown(VK_TAB))
	{
		//정보창 열고 닫기
		if(!_sampleInfo) _sampleInfo = true;
		else if(_sampleInfo) _sampleInfo = false;
	}

	if(KEYMANAGER->isStayKeyDown(VK_LEFT) && _activeMap)	_tX+= 10;
	if(KEYMANAGER->isStayKeyDown(VK_RIGHT) && _activeMap)	_tX-= 10;
	if(KEYMANAGER->isStayKeyDown(VK_UP) && _activeMap)	_tY+= 10;
	if(KEYMANAGER->isStayKeyDown(VK_DOWN) && _activeMap)	_tY-= 10;

	//샘플 관련 정보창
	moveSampleInfo();

	//타일맵 관련 정보창
	moveTileMap();

	//타일맵 속성 확인
	viewTileData();

	//저장소 선택
	selectArChive();

	//테스트
	//tileMapSave();
}

void mapTool::render()
{
	//
	SetTextColor(getMemDC(), RGB(0, 0, 0));
	//글자 세팅
	SetBkMode(getMemDC(), TRANSPARENT);

	//=========================================================================
	//============================ 캔버스 출력 =================================
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
	//============================ 팔레트 출력 =================================
	//=========================================================================

	//Rectangle(getMemDC(), _activeRect);
	IMAGEMANAGER->findImage("MAPBAR")->render(getMemDC(), _activeRect.left, _activeRect.top);
	HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(getMemDC(), myBrush);
	//팔레트 출력
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
	//============================ 정보창 출력 =================================
	//=========================================================================

	char str[128];
	// 스포이드 타입이 맵일 경우
	if (_spuitType == S_MAP)
	{
		sprintf_s(str, "저장된 번호 [ X : %d ][ Y : %d ]", _spuit.tFrame.x, _spuit.tFrame.y);
		TextOut(getMemDC(), _activeRect.left + 15, WINSIZEY - 270, str, strlen(str));

		if(_spuit.tState == MAP_MOVE) sprintf_s(str, "타일 속성 : 이동가능");
		else sprintf_s(str, "타일 속성 : 이동불가");
		TextOut(getMemDC(), _activeRect.left + 15, WINSIZEY - 250, str, strlen(str));

		IMAGEMANAGER->findImage("MAPSPRITE")->frameRender(getMemDC(), _activeRect.left + 190, WINSIZEY - 280, _spuit.tFrame.x, _spuit.tFrame.y);
	}
	// 스포이드 타입이 오브젝트일 경우
	else// if (_spuitType == S_OBJECT)
	{
		sprintf_s(str, "저장된 번호 [ X : %d ][ Y : %d ]", _spuit.oFrame.x, _spuit.oFrame.y);
		TextOut(getMemDC(), _activeRect.left + 15, WINSIZEY - 270, str, strlen(str));

		if (_spuit.oState == SET_PLAYER) sprintf_s(str, "설치 종류 : 플레이어");
		else if (_spuit.oState == SET_MARINE) sprintf_s(str, "설치 종류 : 해병");
		else if (_spuit.oState == SET_PRISONER) sprintf_s(str, "설치 종류 : 죄수");
		else if (_spuit.oState == SET_POTAL) sprintf_s(str, "설치 종류 : 포탈");
		else if(_spuit.oState == SET_ENEMY) sprintf_s(str, "설치 종류 : 건데드");
		else if(_spuit.oState == SET_TABLE) sprintf_s(str, "설치 종류 : 테이블");
		else if(_spuit.oState == SET_DOOR) sprintf_s(str, "설치 종류 : 출입구");
		else if(_spuit.oState == SET_BOX) sprintf_s(str, "설치 종류 : 상자");
		else if(_spuit.oState == SET_LEFT_WAY) sprintf_s(str, "설치 종류 : 왼쪽통로");
		else if(_spuit.oState == SET_RIGHT_WAY) sprintf_s(str, "설치 종류 : 오른쪽통로");
		else if(_spuit.oState == SET_TOP_WAY) sprintf_s(str, "설치 종류 : 위쪽통로");
		else if(_spuit.oState == SET_BOTTOM_WAY) sprintf_s(str, "설치 종류 : 아래쪽통로");
		else if(_spuit.oState == SET_EVENT) sprintf_s(str, "설치 종류 : 이벤트설정");
		else if(_spuit.oState == SET_BOSS) sprintf_s(str, "설치 종류 : 뽀스");
		else sprintf_s(str, "미 설치");
		TextOut(getMemDC(), _activeRect.left + 15, WINSIZEY - 250, str, strlen(str));
	}

	//마우스와 닿은 캔버스의 정보 출력
	if (_activeMap)
	{
		//캔버스 xy좌표
		sprintf_s(str, "캔버스 좌표        :[X : %.f][Y : %.f]", _cTile.mpt.x, _cTile.mpt.y);
		TextOut(getMemDC(), _activeRect.left + 250, WINSIZEY - 270, str, strlen(str));
		//캔버스 배열 번호
		sprintf_s(str, "번호                     :%d", _cTile.num);
		TextOut(getMemDC(), _activeRect.left + 250, WINSIZEY - 250, str, strlen(str));
		//오브젝트 이미지 프레임 xy좌표
		sprintf_s(str, "오브젝트 프레임:[X : %d][Y : %d]", _cTile.oFrame.x, _cTile.oFrame.y);
		TextOut(getMemDC(), _activeRect.left + 250, WINSIZEY - 230, str, strlen(str));
		//오브젝트 종류
		if (_cTile.oState == SET_PLAYER) sprintf_s(str, "설치 종류            : 플레이어");
		else if (_cTile.oState == SET_MARINE) sprintf_s(str, "설치 종류            : 해병");
		else if (_cTile.oState == SET_PRISONER) sprintf_s(str, "설치 종류            : 죄수");
		else if (_cTile.oState == SET_ENEMY) sprintf_s(str, "설치 종류            : 건데드");
		else if (_cTile.oState == SET_DOOR) sprintf_s(str, "설치 종류            : 출입구");
		else if (_cTile.oState == SET_TABLE) sprintf_s(str, "설치 종류            : 테이블");
		else if (_cTile.oState == SET_BOX) sprintf_s(str, "설치 종류            : 상자");
		else if (_cTile.oState == SET_POTAL) sprintf_s(str, "설치 종류            : 포탈");
		else if (_cTile.oState == SET_LEFT_WAY) sprintf_s(str, "설치 종류            : 왼쪽통로");
		else if (_cTile.oState == SET_RIGHT_WAY) sprintf_s(str, "설치 종류            : 오른쪽통로");
		else if (_cTile.oState == SET_TOP_WAY) sprintf_s(str, "설치 종류            : 위쪽통로");
		else if (_cTile.oState == SET_BOTTOM_WAY) sprintf_s(str, "설치 종류            : 아래쪽통로");
		else if (_cTile.oState == SET_EVENT) sprintf_s(str, "설치 종류            : 이벤트설정");
		else if (_cTile.oState == SET_BOSS) sprintf_s(str, "설치 종류            : 보스");
		else sprintf_s(str, "설치 종류            : 미 설치");
		TextOut(getMemDC(), _activeRect.left + 250, WINSIZEY - 210, str, strlen(str));
		//타일 프레임
		sprintf_s(str, "타일 프레임        :[X : %d][Y : %d]", _cTile.tFrame.x, _cTile.tFrame.y);
		TextOut(getMemDC(), _activeRect.left + 250, WINSIZEY - 190, str, strlen(str));
		//타일 속성
		if (_cTile.tState == MAP_MOVE) sprintf_s(str, "타일 속성            : 이동가능");
		else sprintf_s(str, "타일 속성            : 이동불가");
		TextOut(getMemDC(), _activeRect.left + 250, WINSIZEY - 170, str, strlen(str));
	}

	sprintf_s(str, "%d번 저장소 : [X : %d][Y : %d]", _archiveNum, _tMapSize.x, _tMapSize.y);
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
//===========================  버튼 관련 함수  ============================
//========================================================================

//버튼 생성
void mapTool::createButton()
{
	//뒤로가기 버튼
	_bt[0].img = nullptr;
	_bt[0].name = " MENU";
	_bt[0].pt = { ((WINSIZEX / 2) + 50) + _plusX , WINSIZEY - 40 };
	_bt[0].rc = RectMakeCenter(_bt[0].pt.x, _bt[0].pt.y, 75, 35);
	_bt[0].active = true;

	//팔레트 변환 버튼
	_bt[1].img = nullptr;
	_bt[1].name = " MAP";
	_bt[1].pt = { ((WINSIZEX / 2) + 50) + _plusX , WINSIZEY - 90 };
	_bt[1].rc = RectMakeCenter(_bt[1].pt.x, _bt[1].pt.y, 75, 35);
	_bt[1].active = true;

	//캔버스 생성
	_bt[2].img = nullptr;
	_bt[2].name = " D : MAP";
	_bt[2].pt = { ((WINSIZEX / 2) + 50) + _plusX , WINSIZEY - 140 };
	_bt[2].rc = RectMakeCenter(_bt[2].pt.x, _bt[2].pt.y, 75, 35);
	_bt[2].active = true;

	//캔버스 X크기 조정
	_bt[3].img = nullptr;
	_bt[3].name = " [UP | DW]";
	_bt[3].pt = { ((WINSIZEX / 2) + 50) + _plusX , WINSIZEY - 190 };
	_bt[3].rc = RectMakeCenter(_bt[3].pt.x, _bt[3].pt.y, 75, 35);
	_bt[3].active = true;

	//저장
	_bt[4].img = nullptr;
	_bt[4].name = " [SV | LD]";
	_bt[4].pt = { ((WINSIZEX / 2) + 125) + _plusX , WINSIZEY - 190 };
	_bt[4].rc = RectMakeCenter(_bt[4].pt.x, _bt[4].pt.y, 75, 35);
	_bt[4].active = false;
}

//버튼 상호작용
void mapTool::clickButton()
{
	//정보창 비활성화 시 스킵
	if (!_sampleInfo) return;

	//메인 메뉴 씬으로 나가는 버튼
	if (PtInRect(&_bt[0].rc, _ptMouse))
	{
		DSS->saveTileMap();
		SCENEMANAGER->changeScene("MAINMENU");
	}
	//팔레트와 스포이드 종류를 변경해주는 버튼
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
	//캔버스 생성&삭제 버튼
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
	//캔버스 X값 증가&감소 버튼
	else if (PtInRect(&_bt[3].rc, _ptMouse) && !_activeMap)
	{
		if (_ptMouse.x < _bt[3].pt.x && _archiveNum < 8) _archiveNum++;							//_tMapSize.x++;	//LEFT
		else if (_ptMouse.x >= _bt[3].pt.x && _archiveNum > 0) _archiveNum--;	//_tMapSize.x > 0) _tMapSize.x--;	//RIGHT
	}
	//캔버스 Y값 증가&감소 버튼
	else if (PtInRect(&_bt[4].rc, _ptMouse) && _activeMap)
	{
		//if (_ptMouse.x < _bt[4].pt.x) DSS->saveTileMap(_archiveNum,_vTile);//tileMapSave();	//LEFT
		//else if (_ptMouse.x >= _bt[4].pt.x) DSS->addTileMap(_archiveNum,_vTile , true);//tileMapload(0);	//RIGHT
	}
}

//========================================================================
//=========================  캔버스 관련 함수  ==========================
//========================================================================

//타일맵 생성
void mapTool::createTileMap()
{
	//타일맵 생성(타일맵이 이미 있다면 스킵)
	if (_activeMap) return;
	/*
	//타일맵 생성
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
	//생성 확인
	_activeMap = true;
}

//타일맵 삭제
void mapTool::deleteTileMap()
{
	//타일맵이 없다면 스킵
	if (!_activeMap) return;
	
	DSS->saveTileMap(_archiveNum, _vTile);

	//삭제 및 데이터 삭제
	_vTile.clear();
	vector<TILE>().swap(_vTile);

	//타일맵 이동거리 초기화
	_tX = _tY = 0;

	//정보 확인용 타일 초기화
	_cTile.mpt = { 0,0 };
	_cTile.num = 0;
	_cTile.oFrame = { 0,0 };
	_cTile.rc = { 0,0,0,0 };
	_cTile.oState = SET_NULL;
	_cTile.tFrame = { 0,0 };
	_cTile.tState = MAP_MOVE;

	//삭제 확인
	_activeMap = false;
}

//캔버스에 스포이드에 저장된 정보 전달
void mapTool::setTileData()
{
	//타일맵이 비활성화중이라면 스킵
	if (!_activeMap) return;
	//정보창위에 있다면 스킵
	if (PtInRect(&_activeRect,_ptMouse)) return;

	//캔버스 클릭
	for (int i = 0; i < _vTile.size(); ++i)
	{
		if (PtInRect(&_vTile[i].rc, _ptMouse))
		{
			//스포이드 타입에 따른 정보 전달
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

//특정 타일 정보 확인용
void mapTool::viewTileData()
{
	//타일맵이 비활성화중이라면 스킵
	if (!_activeMap) return;
	//정보창위에 있다면 스킵
	if (PtInRect(&_activeRect, _ptMouse)) return;

	int num = 0;
	bool check = false;

	//건든것 체크
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

	//정보 처리
	if (check)
	{
		_cTile = _vTile[num];
	}
	else
	{
		//정보 확인용 타일 초기화
		_cTile.mpt = { 0,0 };
		_cTile.num = 0;
		_cTile.oFrame = { 0,0 };
		_cTile.rc = { 0,0,0,0 };
		_cTile.oState = SET_NULL;
		_cTile.tFrame = { 0,0 };
		_cTile.tState = MAP_MOVE;
	}
}

//타일맵 이동
void mapTool::moveTileMap()
{
	//타일맵이 비활성화중이라면 스킵
	if (!_activeMap) return;

	for (int i = 0; i < _vTile.size(); ++i)
	{
		_vTile[i].rc = RectMake((_vTile[i].mpt.x * TILESIZE) + _tX, (_vTile[i].mpt.y * TILESIZE) + _tY, TILESIZE, TILESIZE);
	}
}

//========================================================================
//=========================  상호 작용 관련 함수  ==========================
//========================================================================

//팔레트 생성 및 관리 함수
void mapTool::paletteManage(int palletnum)
{
	switch (palletnum)
	{
		// 0 : 타일맵 팔레트
		// 1 : 오브젝트 팔레트
		case 0:
		{
			// 팔레트 이미지
			_paletteImg = IMAGEMANAGER->findImage("MAPSPRITE");
			//팔레트 값 갱신
			for (int y = 0; y < PSIZEY; ++y)
			{
				for (int x = 0; x < PSIZEX; ++x)
				{
					_palette[y * PSIZEX + x].mpt = { static_cast<float>(x),static_cast<float>(y) };
					_palette[y * PSIZEX + x].rc = RectMake(((x * TILESIZE) + _activeRect.left) + 10, (y * TILESIZE) + 50, TILESIZE, TILESIZE);// (((x * TILESIZE) + (WINSIZEX / 2)) + _plusX, (y * TILESIZE), TILESIZE, TILESIZE);
					_palette[y * PSIZEX + x].num = y * PSIZEX + x;
					_palette[y * PSIZEX + x].oFrame = { 0 , 0 };
					_palette[y * PSIZEX + x].tFrame = { x , y };
					//타일맵 속성 부여
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
					//타일맵 설치 오브젝트 부여
					_palette[y * PSIZEX + x].oState = SET_NULL;
				}
			}
			break;
		}
		case 1:
		{	
			// 팔레트 이미지
			_paletteImg = IMAGEMANAGER->findImage("OBJSPRITE");
			//팔레트 값 갱신
			for (int y = 0; y < PSIZEY; ++y)
			{
				for (int x = 0; x < PSIZEX; ++x)
				{
					_palette[y * PSIZEX + x].mpt = { static_cast<float>(x),static_cast<float>(y) };
					_palette[y * PSIZEX + x].rc = RectMake(((x * TILESIZE) + _activeRect.left) + 10, (y * TILESIZE) + 50, TILESIZE, TILESIZE);//(((x * TILESIZE) + (WINSIZEX / 2)) + _plusX, (y * TILESIZE), TILESIZE, TILESIZE);
					_palette[y * PSIZEX + x].num = y * PSIZEX + x;
					_palette[y * PSIZEX + x].oFrame = { x , y };
					_palette[y * PSIZEX + x].tFrame = { 0 , 0 };
					//타일맵 속성 부여
					_palette[y * PSIZEX + x].tState = MAP_UNMOVE;
					//타일맵 설치 오브젝트 부여
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

//스포이드 체인지
void mapTool::changeSpuit()
{
	//정보창 비활성화 시 스킵
	if (!_sampleInfo) return;

	//정보창이 활성화중일 시 팔레트와 스포이드의 상호작용
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

//정보창 이동
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

	//정보창 이동
	_activeRect = { (WINSIZEX / 2) + _plusX, 0, (WINSIZEX)+_plusX, WINSIZEY };
	
	//팔레트 이동
	for (int y = 0; y < PSIZEY; ++y)
	{
		for (int x = 0; x < PSIZEX; ++x)
		{
			_palette[y * PSIZEX + x].rc = RectMake(((x * TILESIZE) + _activeRect.left) + 17, (y * TILESIZE) + 15, TILESIZE, TILESIZE);
		}
	}

	//버튼 이동
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
//========================  타일맵 저장 관련 함수  =========================
//========================================================================

//저장소 선택 함수
void mapTool::selectArChive()
{
	
	//저장소 번호에 따른 세부 설정 변경
	switch (_archiveNum)
	{
		//틈
		case 0:
		{
			_tMapSize.x = TMAPSIZEX0;
			_tMapSize.y = TMAPSIZEY0;
			break;
		}
		//시작방
		case 1:
		{
			_tMapSize.x = TMAPSIZEX1;
			_tMapSize.y = TMAPSIZEY1;
			break;
		}
		//적 방1
		case 2:
		{
			_tMapSize.x = TMAPSIZEX2;
			_tMapSize.y = TMAPSIZEY2;
			break;
		}
		//적 방2
		case 3:
		{
			_tMapSize.x = TMAPSIZEX3;
			_tMapSize.y = TMAPSIZEY3;
			break;
		}
		//적 방3
		case 4:
		{
			_tMapSize.x = TMAPSIZEX4;
			_tMapSize.y = TMAPSIZEY4;
			break;
		}
		//적 방4
		case 5:
		{
			_tMapSize.x = TMAPSIZEX5;
			_tMapSize.y = TMAPSIZEY5;
			break;
		}
		//적 방5
		case 6:
		{
			_tMapSize.x = TMAPSIZEX6;
			_tMapSize.y = TMAPSIZEY6;
			break;
		}
		//상점
		case 7:
		{
			_tMapSize.x = TMAPSIZEX7;
			_tMapSize.y = TMAPSIZEY7;
			break;
		}
		//보스방
		case 8:
		{
			_tMapSize.x = TMAPSIZEX8;
			_tMapSize.y = TMAPSIZEY8;
			break;
		}
	}
}