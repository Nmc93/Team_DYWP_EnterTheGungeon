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
	//���� ���������� �Ѿ�� �� ī��Ʈ
	_nextNum++;
	//ī��Ʈ�� �� á�ٸ� ���� ���������� �̵�
	if (_nextNum >= 200)
	{
		//������ ĳ���� Ȯ��

		SCENEMANAGER->changeScene("STAGE1");
	}
}
