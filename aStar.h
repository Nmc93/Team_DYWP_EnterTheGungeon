#pragma once
#include "gameNode.h"
#include <vector>

enum class MType
{
	EMPTY,
	START,
	END,
};

struct ATile
{
	POINTF		mpt;		//맵 좌표
	int			num;		//맵 번호
	RECT		rc;			//타일 렉트
	
	TileState	tState;		//타일 속성
	int			sector;		//지역 번호

	float		totalCost;		//F : 총 거리 비용(G+H)
	float		costFromStart;	//G : 이전타일부터 현재타일까지 비용
	float		costToGoal;		//H : 현재 타일부터 도착점 까지의 경로비용

	bool		isOpen;		//진입 가능여부
	ATile*		pTile;		//부모 타일
	MType		type;		//속성명

	bool		testB;		//테스트용 값
};

class aStar : public gameNode
{
private:

	//================== 캔버스 생성 ===========================
	
	vector<ATile*>				_vTotalMap;		//캔버스
	vector<ATile*>::iterator	_viTotalMap;	//캔버스 반복자
	ATile* _vtemp;	//캔버스 복사용 타일

	int _canvasX;	// 캔버스의 X길이
	int _canvasY;	// 캔버스의 Y길이


	vector<ATile*>				_vOpenList;		//검사예정목록
	vector<ATile*>::iterator	_viOpenList;	//검사예정목록 반복자

	vector<ATile*>				_vCloseList;	//검사종료목록
	vector<ATile*>::iterator	_viCloseList;	//검사종료목록 반복자

	ATile* _startTile;			//시작 타일
	ATile* _endTile;			//종료 타일
	ATile* _currentTile;		//현재 타일

	int _startNum;				//시작 타일 번호
	int _endNum;				//종료 타일 번호
	int _currentNum;			//현재 타일 번호

	vector<int>	_arrayNum;		//배열 번호 저장용

private:

	vector<ATile*> addOpenList(ATile* currenttile , bool tilecheck);

public:

	aStar();
	~aStar();
	//초기화 함수(캔버스,캔버스X,캔버스Y,시작배열,종료배열)
	virtual HRESULT init(vector<TILE> atile, int sizex , int sizey, int start, int end);	
	virtual void release();												//메모리 해제 함슈
	virtual void update();												//연산하는 함수
	virtual void render();												//그리기 함수
	
	//이동 검사 함수[T : 4방향][F : 8방향]
	void waycheck(ATile* tile, bool tilecheck);

	//검사 종료와 배열 초기화
	void closeCheck(vector<int>& arraynum);

	vector<int> getWay() { return _arrayNum; }

	//사용법
	//1. init를 넣는다.
	//2. waycheck을 넣는다.
	//3. 사용 후 closeCheck()로 초기화 시켜준다.

};