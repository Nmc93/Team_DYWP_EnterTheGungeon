#include "pch.h"
#include "aStar.h"

aStar::aStar()
{
}

aStar::~aStar()
{
}

//검사 준비
HRESULT aStar::init(vector<TILE> atile, int sizex, int sizey, int start, int end)
{
	//캔버스 가로 세로
	_canvasX = sizex;
	_canvasY = sizey;

	//받아온 타일을 캔버스에 복사
	for (int i = 0; i < atile.size(); ++i)
	{
		//받아온 캔버스의 기본적인 자료 복사
		_vtemp = new ATile;
		_vtemp->mpt		= atile[i].mpt;
		_vtemp->num		= atile[i].num;
		_vtemp->rc		= atile[i].rc;
		_vtemp->tState	= atile[i].tState;
		_vtemp->sector	= atile[i].sector;

		//ATile 고유값 초기화
		_vtemp->totalCost		= 0;
		_vtemp->costFromStart	= 0;
		_vtemp->costToGoal		= 0;
		_vtemp->pTile			= nullptr;
		_vtemp->isOpen			= true;
		_vtemp->testB			= false;
		
		//배열 번호가 시작 번호라면
		if (i == start)
		{
			_vtemp->type = MType::START;
		}
		//배열 번호가 종료 번호라면
		else if (i == end)
		{
			_vtemp->type = MType::END;
		}
		//배열 번호가 호드라면
		else
		{
			_vtemp->type = MType::EMPTY;
		}

		_vTotalMap.push_back(_vtemp);
	}

	//시작 타일 초기화
	_startTile = _vTotalMap[start];
	//종료 타일 초기화
	_endTile = _vTotalMap[end];
	//현재 타일 초기화(현재타일 -> 시작 타일)
	_currentTile = _startTile;

	waycheck(_currentTile, true);

	return S_OK;
}

void aStar::release()
{
}

void aStar::update()
{
}

void aStar::render()
{
	for (int i = 0; i < _vTotalMap.size(); ++i)
	{
		if (_vTotalMap[i]->testB) continue;

		Rectangle(getMemDC(), _vTotalMap[i]->rc);
	}
}

//검사예정목록 추가
vector<ATile*> aStar::addOpenList(ATile* currenttile, bool tilecheck)
{
	int startX = currenttile->mpt.x - 1;
	int startY = currenttile->mpt.y - 1;

	for (int y = 0; y < 3; ++y)
	{
		for (int x = 0; x < 3; ++x)
		{
			//true = 4방향 검색
			if (tilecheck)
			{
				//모서리 제외
				if (x == 0 && y == 0 || x == 2 && y == 0 || x == 2 && y == 2 || x == 0 && y == 2)
				{
					continue;
				}
			}

			//(현재 배열) + (증가 배열)
			int array = (startY * _canvasX) + startX + x + (y * _canvasX);
			ATile* node = _vTotalMap[array];

			//검사 노드가 못가는 길이라면 제외
			if (!node->isOpen) continue;
			//현재 검사 노드가 시작지점이거나 종료지점이면 제외
			if (node->type == MType::START) continue;
			if (node->type == MType::END)	continue;

			//제외 검사를 종료하고 이전 위치 타일을 갱신
			node->pTile = _currentTile;

			//중복 확인
			bool addObj = true;

			//검사예정목록 확인
			for (_viOpenList = _vOpenList.begin(); _viOpenList != _vOpenList.end(); ++_viOpenList)
			{
				//중복이라면 중복 체크 후 검사 종료
				if (*_viOpenList == node)
				{
					addObj = false;
					break;
				}
			}

			//검사했다고 티내기
			//if(node->type != MType::END) node->

			//중복이라면 다음 검사
			if (!addObj) continue;

			//중복이 아니라면 검사예정목록에 추가
			_vOpenList.push_back(node);

		}
	}

	return _vOpenList;
}

//A* 검사 시작
void aStar::waycheck(ATile* currenttile, bool tilecheck)
{
	//총 비용과 교환용 타일 선언 및 초기화
	float tempTotalCost = 7000;
	ATile* tempTile = nullptr;

	for (int i = 0; i < addOpenList(currenttile, tilecheck).size(); ++i)
	{
		//1. FGH 값을 계산

		//H 값 연산 : 현재 타일부터 종료 타일까지의 비용
		_vOpenList[i]->costToGoal = 
			(abs(_endTile->mpt.x - _vOpenList[i]->mpt.x))+
			(abs(_endTile->mpt.y - _vOpenList[i]->mpt.y)) * 10;

		//G값 계산을 위한 좌표 준비
		POINTF leftTop1 = _vOpenList[i]->pTile->mpt;
		POINTF leftTop2 = _vOpenList[i]->mpt;

		//G 값 연산 : 이전 타일부터 현재 타일까지의 비용 계산
		_vOpenList[i]->costFromStart = (getDistance(leftTop1.x, leftTop1.y, leftTop2.x, leftTop2.y) > TILESIZE) ? 14 : 10;

		//F 값 연산 : 총 거리 계산 ( G + H )
		_vOpenList[i]->totalCost = _vOpenList[i]->costToGoal + _vOpenList[i]->costFromStart;

		//2.산출된 값을 통해 가장 작은 비용의 루트를 검색
		if (tempTotalCost > _vOpenList[i]->totalCost)
		{
			//비용이 작다면 예상 총 비용을 갱신
			tempTotalCost = _vOpenList[i]->totalCost;
			//교환용 타일에 해당 타일을 삽입
			tempTile = _vOpenList[i];
		}

		//타일 중복검사용 bool
		bool addObj = true;

		//3. 해당 타일과 오픈리스트의 모든 배열과 비교
		for (_viOpenList = _vOpenList.begin(); _viOpenList != _vOpenList.end(); ++_viOpenList)
		{
			//해당 타일과 같은 타일이 있다면 중복체크
			if (*_viOpenList == tempTile)
			{
				addObj = false;
				break;
			}
		}

		//이미 갔으므로 갈 수 없는 길 체크
		_vOpenList[i]->isOpen = false;
		//중복이면 여기서 다음 배열 검사
		if (!addObj) continue;

		//중복이 아닐 경우 오픈리스트에 삽입
		_vOpenList.push_back(tempTile);
	}

	//end위치에 도착했을 경우
	if (tempTile->type == MType::END)
	{
		//처음의 위치까지 추적
		while (_currentTile->pTile != NULL)
		{
			_currentTile->testB = true;
			_arrayNum.push_back(_currentTile->num);
		}
		//길찾기 종료
		return;
	}

	//교환용 타일을 검색 종료 목록에 삽입
	_vCloseList.push_back(tempTile);

	//오픈리스트 검색
	for (_viOpenList = _vOpenList.begin(); _viOpenList != _vOpenList.end(); ++_viOpenList)
	{
		//검사예정목록에 있다면
		if (*_viOpenList == tempTile)
		{
			//검사예정목록에서 삭제
			_viOpenList = _vOpenList.erase(_viOpenList);
			break;
		}
	}

	//현재타일에 삽입
	_currentTile = tempTile;

	//재귀함수
	waycheck(_currentTile, tilecheck);
}

//벡터 전달 및 초기화
void aStar::closeCheck(vector<int>& arraynum)
{

	//arraynum = _arrayNum;

	//캔버스 삭제
	_vTotalMap.clear();		
	vector<ATile*>().swap(_vTotalMap);
	//검사 예정 목록 삭제
	_vOpenList.clear();
	vector<ATile*>().swap(_vOpenList);
	//검사 종료 목록 삭제
	_vCloseList.clear();	
	vector<ATile*>().swap(_vCloseList);
	_arrayNum.clear();
	vector<int>().swap(_arrayNum);

}
