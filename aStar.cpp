#include "pch.h"
#include "aStar.h"

aStar::aStar()
{
}

aStar::~aStar()
{
}

//�˻� �غ�
HRESULT aStar::init(vector<TILE> atile, int sizex, int sizey, int start, int end)
{
	//ĵ���� ���� ����
	_canvasX = sizex;
	_canvasY = sizey;

	//�޾ƿ� Ÿ���� ĵ������ ����
	for (int i = 0; i < atile.size(); ++i)
	{
		//�޾ƿ� ĵ������ �⺻���� �ڷ� ����
		_vtemp = new ATile;
		_vtemp->mpt		= atile[i].mpt;
		_vtemp->num		= atile[i].num;
		_vtemp->rc		= atile[i].rc;
		_vtemp->tState	= atile[i].tState;
		_vtemp->sector	= atile[i].sector;

		//ATile ������ �ʱ�ȭ
		_vtemp->totalCost		= 0;
		_vtemp->costFromStart	= 0;
		_vtemp->costToGoal		= 0;
		_vtemp->pTile			= nullptr;
		_vtemp->isOpen			= true;
		_vtemp->testB			= false;
		
		//�迭 ��ȣ�� ���� ��ȣ���
		if (i == start)
		{
			_vtemp->type = MType::START;
		}
		//�迭 ��ȣ�� ���� ��ȣ���
		else if (i == end)
		{
			_vtemp->type = MType::END;
		}
		//�迭 ��ȣ�� ȣ����
		else
		{
			_vtemp->type = MType::EMPTY;
		}

		_vTotalMap.push_back(_vtemp);
	}

	//���� Ÿ�� �ʱ�ȭ
	_startTile = _vTotalMap[start];
	//���� Ÿ�� �ʱ�ȭ
	_endTile = _vTotalMap[end];
	//���� Ÿ�� �ʱ�ȭ(����Ÿ�� -> ���� Ÿ��)
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

//�˻翹����� �߰�
vector<ATile*> aStar::addOpenList(ATile* currenttile, bool tilecheck)
{
	int startX = currenttile->mpt.x - 1;
	int startY = currenttile->mpt.y - 1;

	for (int y = 0; y < 3; ++y)
	{
		for (int x = 0; x < 3; ++x)
		{
			//true = 4���� �˻�
			if (tilecheck)
			{
				//�𼭸� ����
				if (x == 0 && y == 0 || x == 2 && y == 0 || x == 2 && y == 2 || x == 0 && y == 2)
				{
					continue;
				}
			}

			//(���� �迭) + (���� �迭)
			int array = (startY * _canvasX) + startX + x + (y * _canvasX);
			ATile* node = _vTotalMap[array];

			//�˻� ��尡 ������ ���̶�� ����
			if (!node->isOpen) continue;
			//���� �˻� ��尡 ���������̰ų� ���������̸� ����
			if (node->type == MType::START) continue;
			if (node->type == MType::END)	continue;

			//���� �˻縦 �����ϰ� ���� ��ġ Ÿ���� ����
			node->pTile = _currentTile;

			//�ߺ� Ȯ��
			bool addObj = true;

			//�˻翹����� Ȯ��
			for (_viOpenList = _vOpenList.begin(); _viOpenList != _vOpenList.end(); ++_viOpenList)
			{
				//�ߺ��̶�� �ߺ� üũ �� �˻� ����
				if (*_viOpenList == node)
				{
					addObj = false;
					break;
				}
			}

			//�˻��ߴٰ� Ƽ����
			//if(node->type != MType::END) node->

			//�ߺ��̶�� ���� �˻�
			if (!addObj) continue;

			//�ߺ��� �ƴ϶�� �˻翹����Ͽ� �߰�
			_vOpenList.push_back(node);

		}
	}

	return _vOpenList;
}

//A* �˻� ����
void aStar::waycheck(ATile* currenttile, bool tilecheck)
{
	//�� ���� ��ȯ�� Ÿ�� ���� �� �ʱ�ȭ
	float tempTotalCost = 7000;
	ATile* tempTile = nullptr;

	for (int i = 0; i < addOpenList(currenttile, tilecheck).size(); ++i)
	{
		//1. FGH ���� ���

		//H �� ���� : ���� Ÿ�Ϻ��� ���� Ÿ�ϱ����� ���
		_vOpenList[i]->costToGoal = 
			(abs(_endTile->mpt.x - _vOpenList[i]->mpt.x))+
			(abs(_endTile->mpt.y - _vOpenList[i]->mpt.y)) * 10;

		//G�� ����� ���� ��ǥ �غ�
		POINTF leftTop1 = _vOpenList[i]->pTile->mpt;
		POINTF leftTop2 = _vOpenList[i]->mpt;

		//G �� ���� : ���� Ÿ�Ϻ��� ���� Ÿ�ϱ����� ��� ���
		_vOpenList[i]->costFromStart = (getDistance(leftTop1.x, leftTop1.y, leftTop2.x, leftTop2.y) > TILESIZE) ? 14 : 10;

		//F �� ���� : �� �Ÿ� ��� ( G + H )
		_vOpenList[i]->totalCost = _vOpenList[i]->costToGoal + _vOpenList[i]->costFromStart;

		//2.����� ���� ���� ���� ���� ����� ��Ʈ�� �˻�
		if (tempTotalCost > _vOpenList[i]->totalCost)
		{
			//����� �۴ٸ� ���� �� ����� ����
			tempTotalCost = _vOpenList[i]->totalCost;
			//��ȯ�� Ÿ�Ͽ� �ش� Ÿ���� ����
			tempTile = _vOpenList[i];
		}

		//Ÿ�� �ߺ��˻�� bool
		bool addObj = true;

		//3. �ش� Ÿ�ϰ� ���¸���Ʈ�� ��� �迭�� ��
		for (_viOpenList = _vOpenList.begin(); _viOpenList != _vOpenList.end(); ++_viOpenList)
		{
			//�ش� Ÿ�ϰ� ���� Ÿ���� �ִٸ� �ߺ�üũ
			if (*_viOpenList == tempTile)
			{
				addObj = false;
				break;
			}
		}

		//�̹� �����Ƿ� �� �� ���� �� üũ
		_vOpenList[i]->isOpen = false;
		//�ߺ��̸� ���⼭ ���� �迭 �˻�
		if (!addObj) continue;

		//�ߺ��� �ƴ� ��� ���¸���Ʈ�� ����
		_vOpenList.push_back(tempTile);
	}

	//end��ġ�� �������� ���
	if (tempTile->type == MType::END)
	{
		//ó���� ��ġ���� ����
		while (_currentTile->pTile != NULL)
		{
			_currentTile->testB = true;
			_arrayNum.push_back(_currentTile->num);
		}
		//��ã�� ����
		return;
	}

	//��ȯ�� Ÿ���� �˻� ���� ��Ͽ� ����
	_vCloseList.push_back(tempTile);

	//���¸���Ʈ �˻�
	for (_viOpenList = _vOpenList.begin(); _viOpenList != _vOpenList.end(); ++_viOpenList)
	{
		//�˻翹����Ͽ� �ִٸ�
		if (*_viOpenList == tempTile)
		{
			//�˻翹����Ͽ��� ����
			_viOpenList = _vOpenList.erase(_viOpenList);
			break;
		}
	}

	//����Ÿ�Ͽ� ����
	_currentTile = tempTile;

	//����Լ�
	waycheck(_currentTile, tilecheck);
}

//���� ���� �� �ʱ�ȭ
void aStar::closeCheck(vector<int>& arraynum)
{

	//arraynum = _arrayNum;

	//ĵ���� ����
	_vTotalMap.clear();		
	vector<ATile*>().swap(_vTotalMap);
	//�˻� ���� ��� ����
	_vOpenList.clear();
	vector<ATile*>().swap(_vOpenList);
	//�˻� ���� ��� ����
	_vCloseList.clear();	
	vector<ATile*>().swap(_vCloseList);
	_arrayNum.clear();
	vector<int>().swap(_arrayNum);

}
