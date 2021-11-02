#include "pch.h"
#include "mapSetting.h"

HRESULT mapSetting::init(int num)
{
	//페이즈 생성
	_phase[0].mapMin = 0;
	_phase[0].mapMax = 0;
	//시작맵
	_phase[1].mapMin = 1;
	_phase[1].mapMax = 1;
	//스테이지1
	_phase[2].mapMin = 2;
	_phase[2].mapMax = 7;
	//스테이지2
	_phase[3].mapMin = 2;
	_phase[3].mapMax = 7;
	//상점
	_phase[4].mapMin = 7;
	_phase[4].mapMax = 7;
	//보스
	_phase[5].mapMin = 8;
	_phase[5].mapMax = 8;

	//배열 변수
	aTest[0] = 0;
	aTest[1] = 0;
	aTest[2] = 0;
	aTest[3] = 0;

	//번호 변수 초기화
	_phaseNum = 1;
	_sectorNum = 0;
	_sectorSize = 0;

	//문 생성 관련 변수 초기화
	currentWay = 0;
	startWay = 0;
	nextWay = 0;

	//방 설치용 POINT
	ranXY = { 0 , 0 };

	//맵 검색 범위
	_cRange = { 0,0,0,0 };

	_tempSizeX = 0;
	_tempSizeY = 0;

    return S_OK;
}

void mapSetting::release()
{
}

void mapSetting::createTileMap(vector<TILE>& vtile,int xsize ,int ysize)
{
	//===============
	// 캔버스 생성===
	//===============

	_maxX = xsize;
	_maxY = ysize;
	//_leTo = { 0,0 };
	//_riBo = { xsize , ysize };
	_cRange = { 0 , 0 , xsize , ysize };;

	for (int y = 0; y < _maxY; ++y)
	{
		for (int x = 0; x < _maxX; ++x)
		{
			_tempTile.mpt	 = { static_cast<float>(x) , static_cast<float>(y) };
			_tempTile.num	 = y * _maxX + x;
			_tempTile.rc	 = RectMake(x * TILESIZE, y * TILESIZE, TILESIZE, TILESIZE);
			_tempTile.oFrame = { 0 , 0 };
			_tempTile.oState = SET_NULL;
			_tempTile.tFrame = { 0 , 0 };
			_tempTile.tState = MAP_UNMOVE;
			_tempTile.sector = 0;

			_vMap.push_back(_tempTile);
		}
	}

	//===============
	// 방 생성 ======
	//===============

	//맵 랜덤 세팅
	mapSet(1);
	
	//===============
	// 전달    ======
	//===============
	
	//생성된 맵을 전달
	vtile = _vMap;

	//뒷정리

}

//맵 세팅
void mapSetting::mapSet(int phasenum)
{
	//작업이 가능한 페이즈인지 확인 (5페이즈를 넘겼다면 종료)
	if (phasenum >= 6) return;

	//0.필요 변수 초기화=======================================================================

	int roomNum = 1;		//방 번호
	int direction = 0;	//방향[0:중립] [1 :LEFT] [2:RIGHT] [3:TOP] [4:BOTTOM]
	
	//0-1.랜덤 좌표 초기화
	ranXY = { 0 , 0 };

	//1. 맵 생성 준비==========================================================================

	//1-2.페이즈에 해당하는 방 선택(설치 준비)
	if (_phase[phasenum].mapMin != _phase[phasenum].mapMax)
	{
		while (true)
		{
			roomNum = RND->getFromIntTo(_phase[phasenum].mapMin, _phase[phasenum].mapMax);

			for (int i = 0; i < _closeMap.size(); ++i)
			{
				if (roomNum == _closeMap[i]) continue;
			}

			break;
		}
	}
	else if (_phase[phasenum].mapMin == _phase[phasenum].mapMax)
	{
		roomNum = _phase[phasenum].mapMin;
	}

	//1-2. 선택된 방과 방의 사이즈 불러오기(설계도 생성)
	DSS->addTileMap(roomNum, _vtemp, _tempSizeX, _tempSizeY, true);

	//1-3. 방을 복사해줄 지역 검색(땅찾기)
	while (true)
	{
		//방의 시작 좌표를 랜덤 생성0 6 -> 0~5
		ranXY = { RND->getFromIntTo(_cRange.left,_cRange.right),RND->getFromIntTo(_cRange.top,_cRange.bottom) };
		aTest[0] = ranXY.y * _maxX + ranXY.x;

		//방을 설치했을때 방이 캠버스를 넘어가면 재검색
		if (ranXY.x + _tempSizeX >= _maxX || ranXY.y + _tempSizeY >= _maxY)
		{
			continue;
		}
		else if (_vMap[aTest[0]].sector != 0)
		{
			continue;
		}
		//예외조건 더 추가해야함
		 
		//사용한 방을 사용한 목록에 추가
		_closeMap.push_back(roomNum);

		//재검색 조건을 모두 지나면 선택 종료
		break;
	}

	//2. 맵 생성===============================================================================

	//2-2. 준비된 맵과 맵 사이즈를 복사해줄 지역을 시작으로 캔버스에 붙여넣기
	for (int y = 0; y < _tempSizeY; ++y)
	{
		for (int x = 0; x < _tempSizeX; ++x)
		{
			//교환 맵 배열
			//int tempNum = y * _tempSizeX + x;
			//int mapNum = (y + ranXY.y) * _maxX + (x + ranXY.x);
			aTest[0] = (y + ranXY.y) * _maxX + (x + ranXY.x);
			aTest[1] = y * _tempSizeX + x;

			//_map[mapNum].mpt		= _vMap[tempNum].mpt;
			//_vMap[mapNum].num		= _vtemp[tempNum].num;
			_vMap[aTest[0]].oFrame	= _vtemp[aTest[1]].oFrame;
			_vMap[aTest[0]].oState	= _vtemp[aTest[1]].oState;
			_vMap[aTest[0]].rc		= _vtemp[aTest[1]].rc;
			_vMap[aTest[0]].tFrame	= _vtemp[aTest[1]].tFrame;
			_vMap[aTest[0]].tState	= _vtemp[aTest[1]].tState;
			_vMap[aTest[0]].sector = phasenum;
		}
	}

	_vtemp.clear();
	vector<TILE>().swap(_vtemp);
	
	//방 생성 완료
	
	//3. 다음 생성 위치 결정
	
	//3-1. 다음 방을 만들 방향을 선택하고 그 방향에 따른 방 랜덤 생성 지역 선택
	while (true)
	{
		//다음 방 방향 선택
		direction = RND->getFromIntTo(1, 5);

		//여유공간 검사
		if (direction == 1)	//LEFT
		{
			//사용할 배열을 미리 계산[0 : X에서 _tempSizeX 만큼 떨어진 Y 크기의 중간지점 ][1: X에서 _tempSizeX / 2 만큼 떨어진 Y 크기의 중간지점]
			aTest[0] = ranXY.y * _maxX + (ranXY.x - MMSIZE);
			aTest[1] = ranXY.y * _maxX + (ranXY.x - MMSIZE / 2);
			aTest[2] = (ranXY.y + _tempSizeY) * _maxX + (ranXY.x - MMSIZE);
			aTest[3] = (ranXY.y + _tempSizeY) * _maxX + (ranXY.x - MMSIZE / 2);

			//생성지점이 캔버스를 넘으면 재선택
			if (ranXY.x - MMSIZE - 10 <= 0)
			{
				continue;
			}
			//LEFT의 중심지역에 다른 지역이 있다면 재선택
			else if (_vMap[aTest[0]].sector != 0 || _vMap[aTest[1]].sector != 0 ||
				_vMap[aTest[2]].sector != 0 || _vMap[aTest[3]].sector != 0)
			{
				continue;
			}
			//여유공간이 충분하다면 지역 선택
			else
			{
				_cRange.left	= ranXY.x - MMSIZE - 10;		//left
				_cRange.right	= ranXY.x - MMSIZE - 6;			//right
				_cRange.top		= ranXY.y - (_tempSizeY / 2);	//top
				_cRange.bottom	= ranXY.y + (_tempSizeY / 2);	//bottom
			}
		}
		else if (direction == 2) // RIGHT
		{
			aTest[0] = ranXY.y * _maxX + (ranXY.x + MMSIZE * 2);
			aTest[1] = ranXY.y * _maxX + ((ranXY.x + MMSIZE) + MMSIZE / 2);
			aTest[2] = (ranXY.y + _tempSizeY / 2) * _maxX + (ranXY.x + MMSIZE * 2);
			aTest[3] = (ranXY.y + _tempSizeY / 2) * _maxX + ((ranXY.x + MMSIZE) + MMSIZE / 2);

			if (ranXY.x + (MMSIZE * 2) + 10 >= _maxX)
			{
				continue;
			}
			else if (_vMap[aTest[0]].sector != 0 || _vMap[aTest[1]].sector != 0 ||
					_vMap[aTest[2]].sector != 0 || _vMap[aTest[3]].sector != 0)
			{
				continue;
			}
			else
			{
				_cRange.left	= ranXY.x + MMSIZE + 6;			//left
				_cRange.right	= ranXY.x + MMSIZE + 10;		//right
				_cRange.top		= ranXY.y - (_tempSizeY / 2);	//top
				_cRange.bottom	= ranXY.y + (_tempSizeY / 2);	//bottom
			}
		}
		else if (direction == 3)
		{
			aTest[0] = ((ranXY.y - MMSIZE) * _maxX) + ranXY.x;
			aTest[1] = ((ranXY.y - MMSIZE / 2) * _maxX) + ranXY.x;
			aTest[2] = ((ranXY.y - MMSIZE) * _maxX) + (ranXY.x + _tempSizeX);
			aTest[3] = ((ranXY.y - MMSIZE / 2) * _maxX) + (ranXY.x + _tempSizeX);

			if (ranXY.y - MMSIZE - 10 <= 0)
			{
				continue;
			}
			else if (_vMap[aTest[0]].sector != 0 || _vMap[aTest[1]].sector != 0 ||
				_vMap[aTest[2]].sector != 0 || _vMap[aTest[3]].sector != 0)
			{
				continue;
			}
			else
			{
				_cRange.left	= ranXY.x - (_tempSizeX / 2);	//left
				_cRange.right	= ranXY.x + (_tempSizeX / 2);	//right
				_cRange.top		= ranXY.y - MMSIZE - 10;		//top
				_cRange.bottom	= ranXY.y - MMSIZE - 6;			//bottom
			}
		}
		else if (direction == 4)
		{
			aTest[0] = (((ranXY.y + MMSIZE * 2)) * _maxX) + ranXY.x;
			aTest[1] = (((ranXY.y + MMSIZE) + MMSIZE / 2) * _maxX) + ranXY.x;
			aTest[2] = (((ranXY.y + MMSIZE * 2)) * _maxX) + (ranXY.x + _tempSizeX);
			aTest[3] = (((ranXY.y + MMSIZE) + MMSIZE / 2) * _maxX) + (ranXY.x + _tempSizeX);

			if (ranXY.y + (MMSIZE * 2) + 10 >= _maxY)
			{
				continue;
			}
			else if (_vMap[aTest[0]].sector != 0 || _vMap[aTest[1]].sector != 0 ||
				_vMap[aTest[2]].sector != 0 || _vMap[aTest[3]].sector != 0)
			{
				continue;
			}
			else
			{
				_cRange.left	= ranXY.x - (_tempSizeX / 2);	//left
				_cRange.right	= ranXY.x + (_tempSizeX / 2);	//right
				_cRange.top		= ranXY.y + MMSIZE + 6;			//top
				_cRange.bottom	= ranXY.y + MMSIZE + 10;		//bottom
			}
		}
		else
		{
			continue;
		}

		//검사 종료 후 번호 확정
		break;
	}
	
	////방향에 따른 지역 검색
	//switch (direction)
	//{
	//	case 1://[LEFT]
	//	{
	//		_cRange.left	= ranXY.x - _tempSizeX - 10;	//left
	//		_cRange.right	= ranXY.x - _tempSizeX - 6;		//right
	//		_cRange.top		= ranXY.y - (_tempSizeY / 2);	//top
	//		_cRange.bottom	= ranXY.y + (_tempSizeY / 2);	//bottom
	//		break;
	//	}
	//	case 2://[RIGHT]
	//	{
	//		_cRange.left	= ranXY.x + _tempSizeX + 6;		//left
	//		_cRange.right	= ranXY.x + _tempSizeX + 10;	//right
	//		_cRange.top		= ranXY.y - (_tempSizeY / 2);	//top
	//		_cRange.bottom	= ranXY.y + (_tempSizeY / 2);	//bottom
	//		break;
	//	}
	//	case 3://[TOP]
	//	{
	//		_cRange.left	= ranXY.x - (_tempSizeX / 2);	//left
	//		_cRange.right	= ranXY.x + (_tempSizeX / 2);	//right
	//		_cRange.top		= ranXY.y - _tempSizeY - 10;	//top
	//		_cRange.bottom	= ranXY.y - _tempSizeY - 6;		//bottom
	//		break;
	//	}
	//	case 4://[BOTTOM]
	//	{
	//		_cRange.left	= ranXY.x - (_tempSizeX / 2);	//left
	//		_cRange.right	= ranXY.x + (_tempSizeX / 2);	//right
	//		_cRange.top		= ranXY.y + _tempSizeY + 6;		//top
	//		_cRange.bottom	= ranXY.y + _tempSizeY + 10;	//bottom
	//		break;
	//	}
	//}
	

	//재귀함수(만들어진 것들을 이용해서 다음 지역을 생성)
	_phaseNum++;
	mapSet(_phaseNum);
	
}

//맵 생성 2번째
void mapSetting::createTileMap2(vector<TILE>& vtile, int xsize, int ysize, int sectorsize, vector<int>& sectornum)
{
	//캔버스 최대크기 초기화
	_maxX = xsize;
	_maxY = ysize;

	//캔버스 섹터 범위
	_sectorSize = sectorsize;

	//검색 범위 초기화
	_cRange = { 0 , 0 , xsize , ysize };

	//섹터의 XY 번호
	int sectorX = 0;
	int sectorY = 0;

	//캔버스 생성
	for (int y = 0; y < _maxY; ++y)
	{
		sectorY = y / (_maxY / _sectorSize); // 5 or 8
		
		for (int x = 0; x < _maxX; ++x)
		{
			sectorX = x / (_maxX / _sectorSize); // 5 or 8

			_tempTile.mpt = { static_cast<float>(x) , static_cast<float>(y) };
			_tempTile.num = y * _maxX + x;
			_tempTile.rc = RectMake(x * TILESIZE, y * TILESIZE, TILESIZE, TILESIZE);
			_tempTile.oFrame = { 0 , 0 };
			_tempTile.oState = SET_NULL;
			_tempTile.tFrame = { 0 , 0 };
			_tempTile.tState = MAP_UNMOVE;
			_tempTile.sector = sectorY * _sectorSize + sectorX; // 5 or 8

			_vMap.push_back(_tempTile);
		}
	}

	// 캔버스 섹터 선택
	_sectorNum = RND->getFromIntTo(0, 25);
	//_closeSector.push_back(_sectorNum);
	// 맵 세팅
	mapset2(1);

	//종료 후 맵 삽입
	sectornum = _closeSector;
	vtile = _vMap;

}

//맵 세팅 2
void mapSetting::mapset2(int phasenum)
{
	//작업이 가능한 페이즈인지 확인 (5페이즈를 넘겼다면 길만들고 종료)
	if (phasenum >= 6)
	{
		createWay2();
		return;
	}

	//0.필요 변수 초기화=======================================================================
	
	//방 번호
	int roomNum = 1;		
	//방향[0:중립] [1 :LEFT] [2:RIGHT] [3:TOP] [4:BOTTOM]
	_direction = 0;
	//0-1.랜덤 좌표 초기화
	ranXY = { 0 , 0 };
	//방 중복 검사
	bool isRoom = false;

	//1. 맵 생성 준비==========================================================================

	//1-2.페이즈에 해당하는 방 선택(설치 준비)
	//min과 max가 다르다면
	if (_phase[phasenum].mapMin != _phase[phasenum].mapMax)
	{
		while (true)
		{
			//방 번호 생성
			roomNum = RND->getFromIntTo(_phase[phasenum].mapMin, _phase[phasenum].mapMax);

			//중복 방이 있는지 확인
			for (int i = 0; i < _closeMap.size(); ++i)
			{
				//방이 중복이라면
				if (roomNum == _closeMap[i])
				{
					//중복 체크 및 검사 종료
					isRoom = true;
					break;
				}
			}
			//중복일 경우 재검사
			if (isRoom)
			{
				isRoom = false;
				continue;
			}

			break;
		}
	}
	//min과 max가 같다면
	else if (_phase[phasenum].mapMin == _phase[phasenum].mapMax)
	{
		roomNum = _phase[phasenum].mapMin;
	}

	//1-2. 선택된 방과 방의 사이즈 불러오기(설계도 생성)
	DSS->addTileMap(roomNum, _vtemp, _tempSizeX, _tempSizeY, true);

	//1-3. 방을 복사해줄 지역 검색(땅찾기)
	while (true)
	{
		//방의 좌표를 선택, 각 섹터의 첫번째 배열 선택
		for (int i = 0; i < _vMap.size(); ++i)
		{
			//찾는 섹터를 발견
			if (_vMap[i].sector == _sectorNum)
			{
				//섹터의 좌표를 시작좌표로 선택
				ranXY = { static_cast<int>(_vMap[i].mpt.x),static_cast<int>(_vMap[i].mpt.y) };
				break;
			}
		}

		//땅까지 찾았으니 방 사용이 확정, 확정된 방을 사용한 방의 벡터에 삽입
		_closeMap.push_back(roomNum);
		//사용한 섹터를 사용한 목록에 추가
		_closeSector.push_back(_sectorNum);
		//재검색 조건을 모두 지나면 선택 종료
		break;
	}

	//2. 맵 생성===============================================================================

	//2-1. 준비된 맵과 맵 사이즈를 복사해줄 지역을 시작으로 캔버스에 붙여넣기
	for (int y = 0; y < _tempSizeY; ++y)
	{
		for (int x = 0; x < _tempSizeX; ++x)
		{
			//교환 맵 배열(y + 랜덤좌표 보정치) * 배열사이즈 + (x + 랜덤좌표 보정치) 
			aTest[0] = (y + ranXY.y) * _maxX + (x + ranXY.x);
			aTest[1] = y * _tempSizeX + x;

			//캔버스 <- 방 (붙여넣기)
			//_map[mapNum].mpt		= _vMap[tempNum].mpt;
			//_vMap[mapNum].num		= _vtemp[tempNum].num;
			_vMap[aTest[0]].oFrame	= _vtemp[aTest[1]].oFrame;
			_vMap[aTest[0]].oState	= _vtemp[aTest[1]].oState;
			_vMap[aTest[0]].rc		= _vtemp[aTest[1]].rc;
			_vMap[aTest[0]].tFrame	= _vtemp[aTest[1]].tFrame;
			_vMap[aTest[0]].tState	= _vtemp[aTest[1]].tState;
			//_vMap[aTest[0]].sector = phasenum;
		}
	}

	//방을 담은 벡터 초기화
	_vtemp.clear();
	vector<TILE>().swap(_vtemp);

	//3. 다음 맵 검사 ===============================================================================

	//3-1다음 생성할 방향 찾기(섹터와 검사 지역)
	while (true)
	{
		//방향 랜덤 설정
		_direction = RND->getFromIntTo(1,5);

		//방향에 따른 설정값
		//LEFT
		if (_direction == 1)
		{
			//_sectorSize로 나눠서 0일때 -> LEFT의 끝일때 재검사
			if (_sectorNum % _sectorSize == 0) continue;
			//중복 지역 검사
			else if (sectorCheck2(_sectorNum - 1)) continue;
			//지역번호 왼쪽으로 한칸
			_sectorNum--;
		}
		//RIGTH
		else if (_direction == 2)
		{
			//+1이 _sectorSize로 나눠서 0일때 -> RIGHT의 끝일때 재검사
			if((_sectorNum + 1) % _sectorSize == 0) continue;
			//중복 지역 검사
			else if (sectorCheck2(_sectorNum + 1)) continue;
			//지역번호 오른쪽으로 한칸
			_sectorNum++;
		}
		//TOP
		else if (_direction == 3)
		{
			//-5일때 0보다 작다면 -> TOP의 끝일때 재검사
			if (_sectorNum - _sectorSize < 0) continue;
			//중복 지역 검사
			else if (sectorCheck2(_sectorNum - _sectorSize)) continue;
			//지역번호 위쪽으로 한칸
			_sectorNum -= _sectorSize;
		}
		//DOWN
		else if (_direction == 4)
		{
			//+5일때 25보다 크거나 같다면 -> BOTTOM의 끝일때 재검사
			if (_sectorNum + _sectorSize >= ((_sectorSize* _sectorSize) - 1)) continue;	//63 24
			//중복 지역 검사
			else if (sectorCheck2(_sectorNum + _sectorSize)) continue;
			//지역번호 아래쪽으로 한칸
			_sectorNum += _sectorSize;
		}

		//생성
		//_closeSector.push_back(_sectorNum);
		break;
	}

	//종료 및 재귀
	_phaseNum++;
	mapset2(_phaseNum);
}

//섹터 검사
bool mapSetting::sectorCheck2(int secnum)
{
	//닫힌 섹터의 수만큼 검사
	for (int i = 0; i < _closeSector.size(); ++i)
	{
		//닫힌 섹터와 이동하기로 선택된 섹터가 같다면 참
		if (_closeSector[i] == secnum)
		{
			return true;
		}
	}
	//같지 않다면 거짓
	return false;
}


//시작 통로와 끝 통로 찾기(1:L 2:R 3:T 4:B)
void mapSetting::findWay(int sector, int nextsector)
{
	//문 생성 지역 배열 확인 체크
	bool isCurrentCheck = false;
	bool isNextChck = false;

	//현재 섹터 검색
	for (int i = 0; i < _vMap.size(); ++i)
	{
		//현재 섹터가 아니라면 스킵
		if (_vMap[i].sector != sector) continue;

		//왼쪽에 맞는 방향의 문 생성 타입을 확인
		if (_direction == 1 && _vMap[i].oState == SET_LEFT_WAY)
		{
			//조건 충족시 배열 기록
			currentWay = i;
			//찾았을 시 체크
			isCurrentCheck = true;
		}
		//오른쪽에 맞는 방향의 문 생성 타입을 확인
		else if (_direction == 2 && _vMap[i].oState == SET_RIGHT_WAY)
		{
			currentWay = i;
			isCurrentCheck = true;
		}
		//위쪽에 맞는 방향의 문 생성 타입을 확인
		else if (_direction == 3 && _vMap[i].oState == SET_TOP_WAY)
		{
			currentWay = i;
			isCurrentCheck = true;
		}
		//아랫쪽에 맞는 방향의 문 생성 타입을 확인
		else if (_direction == 4 && _vMap[i].oState == SET_BOTTOM_WAY)
		{
			currentWay = i;
			isCurrentCheck = true;
		}

		//확인이 끝났다면 검색 캔슬
		if (isCurrentCheck)
		{
			//검사 종료
			break;
		}
	}
	//다음 섹터 검색
	for (int j = 0; j < _vMap.size(); ++j)
	{
		//다음섹터가 아니라면 스킵
		if (_vMap[j].sector != nextsector) continue;

		//다음섹터가 현재섹터의 왼쪽에 있을 때
		if (_direction == 1 && _vMap[j].oState == SET_RIGHT_WAY)
		{
			//조건 충족시 배열 기록
			nextWay = j;
			//찾았을 시 체크
			isNextChck = true;
		}
		//다음 섹터가 현재섹터의 오른쪽에 있을때
		else if (_direction == 2 && _vMap[j].oState == SET_LEFT_WAY)
		{
			nextWay = j;
			isNextChck = true;
		}
		//다음 섹터가 현재섹터의 위쪽에 있을때
		else if (_direction == 3 && _vMap[j].oState == SET_BOTTOM_WAY)
		{
			nextWay = j;
			isNextChck = true;
		}
		//다음 섹터가 현재섹터의 아래쪽에 있을때
		else if (_direction == 4 && _vMap[j].oState == SET_TOP_WAY)
		{
			nextWay = j;
			isNextChck = true;
		}

		//모두 확인이 끝났다면 검색 캔슬
		if (isNextChck)
		{
			//검사 종료
			break;
		}
	}

	if (isNextChck && isCurrentCheck)
	{
		isNextChck = false;
		isCurrentCheck = false;
	}
}

//통로 생성 2
void mapSetting::bildWay2()
{
	//시작 현재 종료 설정
	int start = currentWay;
	int current = currentWay;
	int end = nextWay;

	int pCurrent[11];

	//길이 완성될때까지 무한 반복
	while (true)
	{
		//첫 시작
		if (current == start)
		{
			//LEFT 로 전진 배치
			if (_direction == 1)
			{
				pCurrent[0] = current - (_maxX * 3);
				pCurrent[1] = current - (_maxX * 2);
				pCurrent[2] = current - _maxX;
				pCurrent[3] = current + _maxX;

				_vMap[pCurrent[0]].tFrame = { 7,3 };
				_vMap[pCurrent[0]].tState = MAP_UNMOVE;
				_vMap[pCurrent[1]].tFrame = { 0,2 };
				_vMap[pCurrent[1]].tState = MAP_UNMOVE;
				_vMap[pCurrent[2]].tFrame = { 0,3 };
				_vMap[pCurrent[2]].tState = MAP_UNMOVE;
				_vMap[current].tFrame = { 6,0 };
				_vMap[current].tState = MAP_MOVE;
				_vMap[pCurrent[3]].tFrame = { 7,1 };
				_vMap[pCurrent[3]].tState = MAP_MOVE;

				current--;
			}
			//RIGHT 로 전진 배치
			else if (_direction == 2)
			{
				pCurrent[0] = current - (_maxX * 3);
				pCurrent[1] = current - (_maxX * 2);
				pCurrent[2] = current - _maxX;
				pCurrent[3] = current + _maxX;

				_vMap[pCurrent[0]].tFrame = { 5,3 };
				_vMap[pCurrent[0]].tState = MAP_UNMOVE;
				_vMap[pCurrent[1]].tFrame = { 0,2 };
				_vMap[pCurrent[1]].tState = MAP_UNMOVE;
				_vMap[pCurrent[2]].tFrame = { 0,3 };
				_vMap[pCurrent[2]].tState = MAP_UNMOVE;
				_vMap[current].tFrame = { 6,0 };
				_vMap[current].tState = MAP_MOVE;
				_vMap[pCurrent[3]].tFrame = { 5,1 };
				_vMap[pCurrent[3]].tState = MAP_MOVE;

				current++;
			}
			//TOP 으로 전진 배치
			else if (_direction == 3)
			{
				pCurrent[0] = current - 1;
				pCurrent[1] = current + 1;
				pCurrent[2] = current + 2;
				pCurrent[3] = current - 1 + _maxX;
				pCurrent[4] = current + _maxX;
				pCurrent[5] = current + 1 + _maxX;
				pCurrent[6] = current + 2 + _maxX;
				pCurrent[7] = current - 1 + (_maxX * 2);
				pCurrent[7] = current - 1 + (_maxX * 2);
				pCurrent[8] = current + (_maxX * 2);
				pCurrent[9] = current + 1 + (_maxX * 2);
				pCurrent[10] = current + 2 + (_maxX * 2);

				//1
				_vMap[pCurrent[0]].tFrame = { 7,3 };
				_vMap[pCurrent[0]].tState = MAP_UNMOVE;
				_vMap[current].tFrame = { 6,0 };
				_vMap[current].tState = MAP_MOVE;
				_vMap[pCurrent[1]].tFrame = { 6,0 };
				_vMap[pCurrent[1]].tState = MAP_MOVE;
				_vMap[pCurrent[2]].tFrame = { 5,3 };
				_vMap[pCurrent[2]].tState = MAP_UNMOVE;
				//2
				_vMap[pCurrent[3]].tFrame = { 0,2 };
				_vMap[pCurrent[3]].tState = MAP_UNMOVE;
				_vMap[pCurrent[4]].tFrame = { 6,0 };
				_vMap[pCurrent[4]].tState = MAP_MOVE;
				_vMap[pCurrent[5]].tFrame = { 6,0 };
				_vMap[pCurrent[5]].tState = MAP_MOVE;
				_vMap[pCurrent[6]].tFrame = { 0,2 };
				_vMap[pCurrent[6]].tState = MAP_UNMOVE;
				//3
				_vMap[pCurrent[7]].tFrame = { 0,3 };
				_vMap[pCurrent[7]].tState = MAP_UNMOVE;
				_vMap[pCurrent[8]].tFrame = { 6,0 };
				_vMap[pCurrent[8]].tState = MAP_MOVE;
				_vMap[pCurrent[9]].tFrame = { 6,0 };
				_vMap[pCurrent[9]].tState = MAP_MOVE;
				_vMap[pCurrent[10]].tFrame = { 0,3 };
				_vMap[pCurrent[10]].tState = MAP_UNMOVE;

				current -= _maxX;
			}
			//BOTTOM 으로 전진 배치
			else if (_direction == 4)
			{
				pCurrent[0] = current - 1 - _maxX;
				pCurrent[1] = current - _maxX;
				pCurrent[2] = current + 1 - _maxX;
				pCurrent[3] = current + 2 - _maxX;
				pCurrent[4] = current - 1;
				pCurrent[5] = current + 1;
				pCurrent[6] = current + 2;
				
				//2
				_vMap[pCurrent[0]].tFrame = { 7,1 };
				_vMap[pCurrent[0]].tState = MAP_MOVE;
				_vMap[pCurrent[1]].tFrame = { 6,0 };
				_vMap[pCurrent[1]].tState = MAP_MOVE;
				_vMap[pCurrent[2]].tFrame = { 6,0 };
				_vMap[pCurrent[2]].tState = MAP_MOVE;
				_vMap[pCurrent[3]].tFrame = { 5,1 };
				_vMap[pCurrent[3]].tState = MAP_MOVE;
				//1
				_vMap[pCurrent[4]].tFrame = { 7,2 };
				_vMap[pCurrent[4]].tState = MAP_UNMOVE;
				_vMap[current].tFrame = { 6,0 };
				_vMap[current].tState = MAP_MOVE;
				_vMap[pCurrent[5]].tFrame = { 6,0 };
				_vMap[pCurrent[5]].tState = MAP_MOVE;
				_vMap[pCurrent[6]].tFrame = { 5,2 };
				_vMap[pCurrent[6]].tState = MAP_UNMOVE;

				current += _maxX;
			}

			continue;
		}
		//마지막
		else if (current == end || (_vMap[current].tFrame.x != 0 && _vMap[current].tFrame.y != 0))
		{
			//LEFT 로 전진 배치
			if (_direction == 1)
			{
				pCurrent[0] = current - (_maxX * 3);
				pCurrent[1] = current - (_maxX * 2);
				pCurrent[2] = current - _maxX;
				pCurrent[3] = current + _maxX;

				_vMap[pCurrent[0]].tFrame = { 5,3 };
				_vMap[pCurrent[0]].tState = MAP_UNMOVE;
				_vMap[pCurrent[1]].tFrame = { 0,2 };
				_vMap[pCurrent[1]].tState = MAP_UNMOVE;
				_vMap[pCurrent[2]].tFrame = { 0,3 };
				_vMap[pCurrent[2]].tState = MAP_UNMOVE;
				_vMap[current].tFrame = { 6,0 };
				_vMap[current].tState = MAP_MOVE;
				_vMap[pCurrent[3]].tFrame = { 5,1 };
				_vMap[pCurrent[3]].tState = MAP_MOVE;
			}
			//RIGHT 로 전진 배치
			else if (_direction == 2)
			{
				pCurrent[0] = current - (_maxX * 3);
				pCurrent[1] = current - (_maxX * 2);
				pCurrent[2] = current - _maxX;
				pCurrent[3] = current + _maxX;


				_vMap[pCurrent[0]].tFrame = { 7,3 };
				_vMap[pCurrent[0]].tState = MAP_UNMOVE;
				_vMap[pCurrent[1]].tFrame = { 0,2 };
				_vMap[pCurrent[1]].tState = MAP_UNMOVE;
				_vMap[pCurrent[2]].tFrame = { 0,3 };
				_vMap[pCurrent[2]].tState = MAP_UNMOVE;
				_vMap[current].tFrame = { 6,0 };
				_vMap[current].tState = MAP_MOVE;
				_vMap[pCurrent[3]].tFrame = { 7,1 };
				_vMap[pCurrent[3]].tState = MAP_MOVE;
			}
			//TOP 으로 전진 배치
			else if (_direction == 3)
			{
				pCurrent[0] = current - 1 - _maxX;
				pCurrent[1] = current - _maxX;
				pCurrent[2] = current + 1 - _maxX;
				pCurrent[3] = current + 2 - _maxX;
				pCurrent[4] = current - 1;
				pCurrent[5] = current + 1;
				pCurrent[6] = current + 2;

				_vMap[pCurrent[0]].tFrame = { 7,1 };
				_vMap[pCurrent[0]].tState = MAP_MOVE;
				_vMap[pCurrent[1]].tFrame = { 6,0 };
				_vMap[pCurrent[1]].tState = MAP_MOVE;
				_vMap[pCurrent[2]].tFrame = { 6,0 };
				_vMap[pCurrent[2]].tState = MAP_MOVE;
				_vMap[pCurrent[3]].tFrame = { 5,1 };
				_vMap[pCurrent[3]].tState = MAP_MOVE;
				//1
				_vMap[pCurrent[4]].tFrame = { 7,2 };
				_vMap[pCurrent[4]].tState = MAP_UNMOVE;
				_vMap[current].tFrame = { 6,0 };
				_vMap[current].tState = MAP_MOVE;
				_vMap[pCurrent[5]].tFrame = { 6,0 };
				_vMap[pCurrent[5]].tState = MAP_MOVE;
				_vMap[pCurrent[6]].tFrame = { 5,2 };
				_vMap[pCurrent[6]].tState = MAP_UNMOVE;
			}
			//BOTTOM 으로 전진 배치
			else if (_direction == 4)
			{
				pCurrent[0] = current - 1;
				pCurrent[1] = current + 1;
				pCurrent[2] = current + 2;
				pCurrent[3] = current - 1 + _maxX;
				pCurrent[4] = current + _maxX;
				pCurrent[5] = current + 1 + _maxX;
				pCurrent[6] = current + 2 + _maxX;
				pCurrent[7] = current - 1 + (_maxX * 2);
				pCurrent[8] = current + (_maxX * 2);
				pCurrent[9] = current + 1 + (_maxX * 2);
				pCurrent[10] = current + 2 + (_maxX * 2);

				//1
				_vMap[pCurrent[0]].tFrame = { 7,3 };
				_vMap[pCurrent[0]].tState = MAP_UNMOVE;
				_vMap[current].tFrame = { 6,0 };
				_vMap[current].tState = MAP_MOVE;
				_vMap[pCurrent[1]].tFrame = { 6,0 };
				_vMap[pCurrent[1]].tState = MAP_MOVE;
				_vMap[pCurrent[2]].tFrame = { 5,3 };
				_vMap[pCurrent[2]].tState = MAP_UNMOVE;
				//2
				_vMap[pCurrent[3]].tFrame = { 0,2 };
				_vMap[pCurrent[3]].tState = MAP_UNMOVE;
				_vMap[pCurrent[4]].tFrame = { 6,0 };
				_vMap[pCurrent[4]].tState = MAP_MOVE;
				_vMap[pCurrent[5]].tFrame = { 6,0 };
				_vMap[pCurrent[5]].tState = MAP_MOVE;
				_vMap[pCurrent[6]].tFrame = { 0,2 };
				_vMap[pCurrent[6]].tState = MAP_UNMOVE;
				//3
				_vMap[pCurrent[7]].tFrame = { 0,3 };
				_vMap[pCurrent[7]].tState = MAP_UNMOVE;
				_vMap[pCurrent[8]].tFrame = { 6,0 };
				_vMap[pCurrent[8]].tState = MAP_MOVE;
				_vMap[pCurrent[9]].tFrame = { 6,0 };
				_vMap[pCurrent[9]].tState = MAP_MOVE;
				_vMap[pCurrent[10]].tFrame = { 0,3 };
				_vMap[pCurrent[10]].tState = MAP_UNMOVE;

			}

			_direction = 0;
			break;
		}
		else
		{
			//중간 진행
			//LEFT 로 전진 배치
			if (_direction == 1)
			{
				pCurrent[0] = current - (_maxX * 3);
				pCurrent[1] = current - (_maxX * 2);
				pCurrent[2] = current - _maxX;
				pCurrent[3] = current + _maxX;

				_vMap[pCurrent[0]].tFrame = { 6,3 };
				_vMap[pCurrent[0]].tState = MAP_UNMOVE;
				_vMap[pCurrent[1]].tFrame = { 0,2 };
				_vMap[pCurrent[1]].tState = MAP_UNMOVE;
				_vMap[pCurrent[2]].tFrame = { 0,3 };
				_vMap[pCurrent[2]].tState = MAP_UNMOVE;
				_vMap[current].tFrame = { 6,0 };
				_vMap[current].tState = MAP_MOVE;
				_vMap[pCurrent[3]].tFrame = { 6,1 };
				_vMap[pCurrent[3]].tState = MAP_UNMOVE;

				current--;
			}
			//RIGHT 로 전진 배치
			else if (_direction == 2)
			{
				pCurrent[0] = current - (_maxX * 3);
				pCurrent[1] = current - (_maxX * 2);
				pCurrent[2] = current - _maxX;
				pCurrent[3] = current + _maxX;

				_vMap[pCurrent[0]].tFrame = { 6,3 };
				_vMap[pCurrent[0]].tState = MAP_UNMOVE;
				_vMap[pCurrent[1]].tFrame = { 0,2 };
				_vMap[pCurrent[1]].tState = MAP_UNMOVE;
				_vMap[pCurrent[2]].tFrame = { 0,3 };
				_vMap[pCurrent[2]].tState = MAP_UNMOVE;
				_vMap[current].tFrame = { 6,0 };
				_vMap[current].tState = MAP_MOVE;
				_vMap[pCurrent[3]].tFrame = { 6,1 };
				_vMap[pCurrent[3]].tState = MAP_UNMOVE;

				current++;
			}
			//TOP 으로 전진 배치
			else if (_direction == 3)
			{
				pCurrent[0] = current - 1;
				pCurrent[1] = current + 1;
				pCurrent[2] = current + 2;

				_vMap[pCurrent[0]].tFrame = { 7,2 };
				_vMap[pCurrent[0]].tState = MAP_UNMOVE;

				_vMap[current].tFrame = { 6,0 };
				_vMap[current].tState = MAP_MOVE;
				_vMap[pCurrent[1]].tFrame = { 6,0 };
				_vMap[pCurrent[1]].tState = MAP_MOVE;

				_vMap[pCurrent[2]].tFrame = { 5,2 };
				_vMap[pCurrent[2]].tState = MAP_UNMOVE;

				current -= _maxX;
			}
			//BOTTOM 으로 전진 배치
			else if (_direction == 4)
			{
				pCurrent[0] = current - 1;
				pCurrent[1] = current + 1;
				pCurrent[2] = current + 2;

				_vMap[pCurrent[0]].tFrame = { 7,2 };
				_vMap[pCurrent[0]].tState = MAP_UNMOVE;

				_vMap[current].tFrame = { 6,0 };
				_vMap[current].tState = MAP_MOVE;
				_vMap[pCurrent[1]].tFrame = { 6,0 };
				_vMap[pCurrent[1]].tState = MAP_MOVE;

				_vMap[pCurrent[2]].tFrame = { 5,2 };
				_vMap[pCurrent[2]].tState = MAP_UNMOVE;

				current += _maxX;
			}

		}
	}
}

//길 생성 2
void mapSetting::createWay2()
{
	//첫 방에서 길을 내서 닿을때까지 직진
	for (int i = 0; i < _closeSector.size() - 1; i++)
	{
		//2. 방향에 따른 시작점과 도착점 확인
		int iPlus = 0;
		//통로의 방향[1:L][2:R][3:T][4:B]
		//_direction = 0;
		//====================== 현재 방의 시작지점과 다음 방의 도착지점을 확인 ================
		//1.방향 찾기
		//if (i == _closeSector.size() - 1)
		//{
		//	iPlus = i - 1;
		//	//다음방은 왼쪽
		//	if (_closeSector[i] == _closeSector[iPlus] + 1)
		//	{
		//		_direction = 1;
		//		findWay(_closeSector[i], _closeSector[iPlus]);
		//	}
		//	//다음 방은 오른쪽
		//	else if (_closeSector[i] == _closeSector[iPlus] - 1)
		//	{
		//		_direction = 2;
		//		findWay(_closeSector[i], _closeSector[iPlus]);
		//	}
		//	//다음 방은 위쪽
		//	else if (_closeSector[i] == _closeSector[iPlus] + _sectorSize)
		//	{
		//		_direction = 3;
		//		findWay(_closeSector[i], _closeSector[iPlus]);
		//	}
		//	//다음 방은 아래쪽
		//	else if (_closeSector[i] == _closeSector[iPlus] - _sectorSize)
		//	{
		//		_direction = 4;
		//		findWay(_closeSector[i], _closeSector[iPlus]);
		//	}
		//}
		//else
		//{
		iPlus = i + 1;
		//다음방은 왼쪽
		if (_closeSector[i] == _closeSector[iPlus] + 1)
		{
			_direction = 1;
			findWay(_closeSector[i], _closeSector[iPlus]);
		}
		//다음 방은 오른쪽
		else if (_closeSector[i] == _closeSector[iPlus] - 1)
		{
			_direction = 2;
			findWay(_closeSector[i], _closeSector[iPlus]);
		}
		//다음 방은 위쪽
		else if (_closeSector[i] == _closeSector[iPlus] + _sectorSize)
		{
			_direction = 3;
			findWay(_closeSector[i], _closeSector[iPlus]);
		}
		//다음 방은 아래쪽
		else if (_closeSector[i] == _closeSector[iPlus] - _sectorSize)
		{
			_direction = 4;
			findWay(_closeSector[i], _closeSector[iPlus]);
		}
		//}
		
		
		//3. 길 제작
		bildWay2();
	}
}
