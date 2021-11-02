#include "pch.h"
#include "nextStage.h"

HRESULT nextStage::init()
{
	_nextNum = 0;
	return S_OK;
}

void nextStage::release()
{
}

void nextStage::update()
{



	stageUp();
}

void nextStage::render()
{
}

void nextStage::stageUp()
{
	//다음 스테이지로 넘어가기 전 카운트
	_nextNum++;
	//카운트가 다 찼다면 다음 스테이지로 이동
	if (_nextNum >= 200)
	{
		//저장할 캐릭터 확인

		SCENEMANAGER->changeScene("STAGE1");
	}
}
