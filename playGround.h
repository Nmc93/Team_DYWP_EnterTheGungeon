#pragma once
#include "gameNode.h"
#include "mainMenu.h"
#include "mapTool.h"
#include "breakStage.h"
#include "nextStage.h"
#include "stage1.h"
#include "enemy.h"
#include "camera.h"

class playGround : public gameNode
{
private:
	enemy* _em;

public:
	playGround();
	~playGround();

	virtual HRESULT init();			//초기화 함수
	virtual void release();			//메모리 해제 함슈
	virtual void update();			//연산하는 함수
	virtual void render();			//그리기 함수

	void addImage();				//초기 이미지 생성 함수
};

