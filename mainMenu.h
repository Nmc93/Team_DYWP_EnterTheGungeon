#pragma once
#include "gameNode.h"
#include <vector>

#define MBTSIZE 3

class mainMenu : public gameNode
{
private:
	//======================= 화면 ==========================

	BUTTON _bt[MBTSIZE];	//버튼
	bool _bActive[MBTSIZE];	//버튼 눌렀을때
	int _menuIndex;			//메뉴 프레임 카운트
	int _menuTimer;			//메뉴 타이머
	image* _menuImg;		//메뉴 이미지
	int	_sleepCount;		//인터페이스 숨기기 카운트
	
	//====히---든=====
	int _bCount;	//3번째 버튼 등장 조건 카운터

private:

	void createButton();			//버튼 생성
	void clickButton(bool push);	//버튼 상호작용

public:

	virtual HRESULT init();			//초기화 함수
	virtual void release();			//메모리 해제 함슈
	virtual void update();			//연산하는 함수
	virtual void render();			//그리기 함수
};

