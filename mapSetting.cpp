#include "pch.h"
#include "mapSetting.h"

HRESULT mapSetting::init(int num)
{
	//������ ����
	_phase[0].mapMin = 0;
	_phase[0].mapMax = 0;
	//���۸�
	_phase[1].mapMin = 1;
	_phase[1].mapMax = 1;
	//��������1
	_phase[2].mapMin = 2;
	_phase[2].mapMax = 7;
	//��������2
	_phase[3].mapMin = 2;
	_phase[3].mapMax = 7;
	//����
	_phase[4].mapMin = 7;
	_phase[4].mapMax = 7;
	//����
	_phase[5].mapMin = 8;
	_phase[5].mapMax = 8;

	//�迭 ����
	aTest[0] = 0;
	aTest[1] = 0;
	aTest[2] = 0;
	aTest[3] = 0;

	//��ȣ ���� �ʱ�ȭ
	_phaseNum = 1;
	_sectorNum = 0;
	_sectorSize = 0;

	//�� ���� ���� ���� �ʱ�ȭ
	currentWay = 0;
	startWay = 0;
	nextWay = 0;

	//�� ��ġ�� POINT
	ranXY = { 0 , 0 };

	//�� �˻� ����
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
	// ĵ���� ����===
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
	// �� ���� ======
	//===============

	//�� ���� ����
	mapSet(1);
	
	//===============
	// ����    ======
	//===============
	
	//������ ���� ����
	vtile = _vMap;

	//������

}

//�� ����
void mapSetting::mapSet(int phasenum)
{
	//�۾��� ������ ���������� Ȯ�� (5����� �Ѱ�ٸ� ����)
	if (phasenum >= 6) return;

	//0.�ʿ� ���� �ʱ�ȭ=======================================================================

	int roomNum = 1;		//�� ��ȣ
	int direction = 0;	//����[0:�߸�] [1 :LEFT] [2:RIGHT] [3:TOP] [4:BOTTOM]
	
	//0-1.���� ��ǥ �ʱ�ȭ
	ranXY = { 0 , 0 };

	//1. �� ���� �غ�==========================================================================

	//1-2.����� �ش��ϴ� �� ����(��ġ �غ�)
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

	//1-2. ���õ� ��� ���� ������ �ҷ�����(���赵 ����)
	DSS->addTileMap(roomNum, _vtemp, _tempSizeX, _tempSizeY, true);

	//1-3. ���� �������� ���� �˻�(��ã��)
	while (true)
	{
		//���� ���� ��ǥ�� ���� ����0 6 -> 0~5
		ranXY = { RND->getFromIntTo(_cRange.left,_cRange.right),RND->getFromIntTo(_cRange.top,_cRange.bottom) };
		aTest[0] = ranXY.y * _maxX + ranXY.x;

		//���� ��ġ������ ���� ķ������ �Ѿ�� ��˻�
		if (ranXY.x + _tempSizeX >= _maxX || ranXY.y + _tempSizeY >= _maxY)
		{
			continue;
		}
		else if (_vMap[aTest[0]].sector != 0)
		{
			continue;
		}
		//�������� �� �߰��ؾ���
		 
		//����� ���� ����� ��Ͽ� �߰�
		_closeMap.push_back(roomNum);

		//��˻� ������ ��� ������ ���� ����
		break;
	}

	//2. �� ����===============================================================================

	//2-2. �غ�� �ʰ� �� ����� �������� ������ �������� ĵ������ �ٿ��ֱ�
	for (int y = 0; y < _tempSizeY; ++y)
	{
		for (int x = 0; x < _tempSizeX; ++x)
		{
			//��ȯ �� �迭
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
	
	//�� ���� �Ϸ�
	
	//3. ���� ���� ��ġ ����
	
	//3-1. ���� ���� ���� ������ �����ϰ� �� ���⿡ ���� �� ���� ���� ���� ����
	while (true)
	{
		//���� �� ���� ����
		direction = RND->getFromIntTo(1, 5);

		//�������� �˻�
		if (direction == 1)	//LEFT
		{
			//����� �迭�� �̸� ���[0 : X���� _tempSizeX ��ŭ ������ Y ũ���� �߰����� ][1: X���� _tempSizeX / 2 ��ŭ ������ Y ũ���� �߰�����]
			aTest[0] = ranXY.y * _maxX + (ranXY.x - MMSIZE);
			aTest[1] = ranXY.y * _maxX + (ranXY.x - MMSIZE / 2);
			aTest[2] = (ranXY.y + _tempSizeY) * _maxX + (ranXY.x - MMSIZE);
			aTest[3] = (ranXY.y + _tempSizeY) * _maxX + (ranXY.x - MMSIZE / 2);

			//���������� ĵ������ ������ �缱��
			if (ranXY.x - MMSIZE - 10 <= 0)
			{
				continue;
			}
			//LEFT�� �߽������� �ٸ� ������ �ִٸ� �缱��
			else if (_vMap[aTest[0]].sector != 0 || _vMap[aTest[1]].sector != 0 ||
				_vMap[aTest[2]].sector != 0 || _vMap[aTest[3]].sector != 0)
			{
				continue;
			}
			//���������� ����ϴٸ� ���� ����
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

		//�˻� ���� �� ��ȣ Ȯ��
		break;
	}
	
	////���⿡ ���� ���� �˻�
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
	

	//����Լ�(������� �͵��� �̿��ؼ� ���� ������ ����)
	_phaseNum++;
	mapSet(_phaseNum);
	
}

//�� ���� 2��°
void mapSetting::createTileMap2(vector<TILE>& vtile, int xsize, int ysize, int sectorsize, vector<int>& sectornum)
{
	//ĵ���� �ִ�ũ�� �ʱ�ȭ
	_maxX = xsize;
	_maxY = ysize;

	//ĵ���� ���� ����
	_sectorSize = sectorsize;

	//�˻� ���� �ʱ�ȭ
	_cRange = { 0 , 0 , xsize , ysize };

	//������ XY ��ȣ
	int sectorX = 0;
	int sectorY = 0;

	//ĵ���� ����
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

	// ĵ���� ���� ����
	_sectorNum = RND->getFromIntTo(0, 25);
	//_closeSector.push_back(_sectorNum);
	// �� ����
	mapset2(1);

	//���� �� �� ����
	sectornum = _closeSector;
	vtile = _vMap;

}

//�� ���� 2
void mapSetting::mapset2(int phasenum)
{
	//�۾��� ������ ���������� Ȯ�� (5����� �Ѱ�ٸ� �游��� ����)
	if (phasenum >= 6)
	{
		createWay2();
		return;
	}

	//0.�ʿ� ���� �ʱ�ȭ=======================================================================
	
	//�� ��ȣ
	int roomNum = 1;		
	//����[0:�߸�] [1 :LEFT] [2:RIGHT] [3:TOP] [4:BOTTOM]
	_direction = 0;
	//0-1.���� ��ǥ �ʱ�ȭ
	ranXY = { 0 , 0 };
	//�� �ߺ� �˻�
	bool isRoom = false;

	//1. �� ���� �غ�==========================================================================

	//1-2.����� �ش��ϴ� �� ����(��ġ �غ�)
	//min�� max�� �ٸ��ٸ�
	if (_phase[phasenum].mapMin != _phase[phasenum].mapMax)
	{
		while (true)
		{
			//�� ��ȣ ����
			roomNum = RND->getFromIntTo(_phase[phasenum].mapMin, _phase[phasenum].mapMax);

			//�ߺ� ���� �ִ��� Ȯ��
			for (int i = 0; i < _closeMap.size(); ++i)
			{
				//���� �ߺ��̶��
				if (roomNum == _closeMap[i])
				{
					//�ߺ� üũ �� �˻� ����
					isRoom = true;
					break;
				}
			}
			//�ߺ��� ��� ��˻�
			if (isRoom)
			{
				isRoom = false;
				continue;
			}

			break;
		}
	}
	//min�� max�� ���ٸ�
	else if (_phase[phasenum].mapMin == _phase[phasenum].mapMax)
	{
		roomNum = _phase[phasenum].mapMin;
	}

	//1-2. ���õ� ��� ���� ������ �ҷ�����(���赵 ����)
	DSS->addTileMap(roomNum, _vtemp, _tempSizeX, _tempSizeY, true);

	//1-3. ���� �������� ���� �˻�(��ã��)
	while (true)
	{
		//���� ��ǥ�� ����, �� ������ ù��° �迭 ����
		for (int i = 0; i < _vMap.size(); ++i)
		{
			//ã�� ���͸� �߰�
			if (_vMap[i].sector == _sectorNum)
			{
				//������ ��ǥ�� ������ǥ�� ����
				ranXY = { static_cast<int>(_vMap[i].mpt.x),static_cast<int>(_vMap[i].mpt.y) };
				break;
			}
		}

		//������ ã������ �� ����� Ȯ��, Ȯ���� ���� ����� ���� ���Ϳ� ����
		_closeMap.push_back(roomNum);
		//����� ���͸� ����� ��Ͽ� �߰�
		_closeSector.push_back(_sectorNum);
		//��˻� ������ ��� ������ ���� ����
		break;
	}

	//2. �� ����===============================================================================

	//2-1. �غ�� �ʰ� �� ����� �������� ������ �������� ĵ������ �ٿ��ֱ�
	for (int y = 0; y < _tempSizeY; ++y)
	{
		for (int x = 0; x < _tempSizeX; ++x)
		{
			//��ȯ �� �迭(y + ������ǥ ����ġ) * �迭������ + (x + ������ǥ ����ġ) 
			aTest[0] = (y + ranXY.y) * _maxX + (x + ranXY.x);
			aTest[1] = y * _tempSizeX + x;

			//ĵ���� <- �� (�ٿ��ֱ�)
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

	//���� ���� ���� �ʱ�ȭ
	_vtemp.clear();
	vector<TILE>().swap(_vtemp);

	//3. ���� �� �˻� ===============================================================================

	//3-1���� ������ ���� ã��(���Ϳ� �˻� ����)
	while (true)
	{
		//���� ���� ����
		_direction = RND->getFromIntTo(1,5);

		//���⿡ ���� ������
		//LEFT
		if (_direction == 1)
		{
			//_sectorSize�� ������ 0�϶� -> LEFT�� ���϶� ��˻�
			if (_sectorNum % _sectorSize == 0) continue;
			//�ߺ� ���� �˻�
			else if (sectorCheck2(_sectorNum - 1)) continue;
			//������ȣ �������� ��ĭ
			_sectorNum--;
		}
		//RIGTH
		else if (_direction == 2)
		{
			//+1�� _sectorSize�� ������ 0�϶� -> RIGHT�� ���϶� ��˻�
			if((_sectorNum + 1) % _sectorSize == 0) continue;
			//�ߺ� ���� �˻�
			else if (sectorCheck2(_sectorNum + 1)) continue;
			//������ȣ ���������� ��ĭ
			_sectorNum++;
		}
		//TOP
		else if (_direction == 3)
		{
			//-5�϶� 0���� �۴ٸ� -> TOP�� ���϶� ��˻�
			if (_sectorNum - _sectorSize < 0) continue;
			//�ߺ� ���� �˻�
			else if (sectorCheck2(_sectorNum - _sectorSize)) continue;
			//������ȣ �������� ��ĭ
			_sectorNum -= _sectorSize;
		}
		//DOWN
		else if (_direction == 4)
		{
			//+5�϶� 25���� ũ�ų� ���ٸ� -> BOTTOM�� ���϶� ��˻�
			if (_sectorNum + _sectorSize >= ((_sectorSize* _sectorSize) - 1)) continue;	//63 24
			//�ߺ� ���� �˻�
			else if (sectorCheck2(_sectorNum + _sectorSize)) continue;
			//������ȣ �Ʒ������� ��ĭ
			_sectorNum += _sectorSize;
		}

		//����
		//_closeSector.push_back(_sectorNum);
		break;
	}

	//���� �� ���
	_phaseNum++;
	mapset2(_phaseNum);
}

//���� �˻�
bool mapSetting::sectorCheck2(int secnum)
{
	//���� ������ ����ŭ �˻�
	for (int i = 0; i < _closeSector.size(); ++i)
	{
		//���� ���Ϳ� �̵��ϱ�� ���õ� ���Ͱ� ���ٸ� ��
		if (_closeSector[i] == secnum)
		{
			return true;
		}
	}
	//���� �ʴٸ� ����
	return false;
}


//���� ��ο� �� ��� ã��(1:L 2:R 3:T 4:B)
void mapSetting::findWay(int sector, int nextsector)
{
	//�� ���� ���� �迭 Ȯ�� üũ
	bool isCurrentCheck = false;
	bool isNextChck = false;

	//���� ���� �˻�
	for (int i = 0; i < _vMap.size(); ++i)
	{
		//���� ���Ͱ� �ƴ϶�� ��ŵ
		if (_vMap[i].sector != sector) continue;

		//���ʿ� �´� ������ �� ���� Ÿ���� Ȯ��
		if (_direction == 1 && _vMap[i].oState == SET_LEFT_WAY)
		{
			//���� ������ �迭 ���
			currentWay = i;
			//ã���� �� üũ
			isCurrentCheck = true;
		}
		//�����ʿ� �´� ������ �� ���� Ÿ���� Ȯ��
		else if (_direction == 2 && _vMap[i].oState == SET_RIGHT_WAY)
		{
			currentWay = i;
			isCurrentCheck = true;
		}
		//���ʿ� �´� ������ �� ���� Ÿ���� Ȯ��
		else if (_direction == 3 && _vMap[i].oState == SET_TOP_WAY)
		{
			currentWay = i;
			isCurrentCheck = true;
		}
		//�Ʒ��ʿ� �´� ������ �� ���� Ÿ���� Ȯ��
		else if (_direction == 4 && _vMap[i].oState == SET_BOTTOM_WAY)
		{
			currentWay = i;
			isCurrentCheck = true;
		}

		//Ȯ���� �����ٸ� �˻� ĵ��
		if (isCurrentCheck)
		{
			//�˻� ����
			break;
		}
	}
	//���� ���� �˻�
	for (int j = 0; j < _vMap.size(); ++j)
	{
		//�������Ͱ� �ƴ϶�� ��ŵ
		if (_vMap[j].sector != nextsector) continue;

		//�������Ͱ� ���缽���� ���ʿ� ���� ��
		if (_direction == 1 && _vMap[j].oState == SET_RIGHT_WAY)
		{
			//���� ������ �迭 ���
			nextWay = j;
			//ã���� �� üũ
			isNextChck = true;
		}
		//���� ���Ͱ� ���缽���� �����ʿ� ������
		else if (_direction == 2 && _vMap[j].oState == SET_LEFT_WAY)
		{
			nextWay = j;
			isNextChck = true;
		}
		//���� ���Ͱ� ���缽���� ���ʿ� ������
		else if (_direction == 3 && _vMap[j].oState == SET_BOTTOM_WAY)
		{
			nextWay = j;
			isNextChck = true;
		}
		//���� ���Ͱ� ���缽���� �Ʒ��ʿ� ������
		else if (_direction == 4 && _vMap[j].oState == SET_TOP_WAY)
		{
			nextWay = j;
			isNextChck = true;
		}

		//��� Ȯ���� �����ٸ� �˻� ĵ��
		if (isNextChck)
		{
			//�˻� ����
			break;
		}
	}

	if (isNextChck && isCurrentCheck)
	{
		isNextChck = false;
		isCurrentCheck = false;
	}
}

//��� ���� 2
void mapSetting::bildWay2()
{
	//���� ���� ���� ����
	int start = currentWay;
	int current = currentWay;
	int end = nextWay;

	int pCurrent[11];

	//���� �ϼ��ɶ����� ���� �ݺ�
	while (true)
	{
		//ù ����
		if (current == start)
		{
			//LEFT �� ���� ��ġ
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
			//RIGHT �� ���� ��ġ
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
			//TOP ���� ���� ��ġ
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
			//BOTTOM ���� ���� ��ġ
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
		//������
		else if (current == end || (_vMap[current].tFrame.x != 0 && _vMap[current].tFrame.y != 0))
		{
			//LEFT �� ���� ��ġ
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
			//RIGHT �� ���� ��ġ
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
			//TOP ���� ���� ��ġ
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
			//BOTTOM ���� ���� ��ġ
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
			//�߰� ����
			//LEFT �� ���� ��ġ
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
			//RIGHT �� ���� ��ġ
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
			//TOP ���� ���� ��ġ
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
			//BOTTOM ���� ���� ��ġ
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

//�� ���� 2
void mapSetting::createWay2()
{
	//ù �濡�� ���� ���� ���������� ����
	for (int i = 0; i < _closeSector.size() - 1; i++)
	{
		//2. ���⿡ ���� �������� ������ Ȯ��
		int iPlus = 0;
		//����� ����[1:L][2:R][3:T][4:B]
		//_direction = 0;
		//====================== ���� ���� ���������� ���� ���� ���������� Ȯ�� ================
		//1.���� ã��
		//if (i == _closeSector.size() - 1)
		//{
		//	iPlus = i - 1;
		//	//�������� ����
		//	if (_closeSector[i] == _closeSector[iPlus] + 1)
		//	{
		//		_direction = 1;
		//		findWay(_closeSector[i], _closeSector[iPlus]);
		//	}
		//	//���� ���� ������
		//	else if (_closeSector[i] == _closeSector[iPlus] - 1)
		//	{
		//		_direction = 2;
		//		findWay(_closeSector[i], _closeSector[iPlus]);
		//	}
		//	//���� ���� ����
		//	else if (_closeSector[i] == _closeSector[iPlus] + _sectorSize)
		//	{
		//		_direction = 3;
		//		findWay(_closeSector[i], _closeSector[iPlus]);
		//	}
		//	//���� ���� �Ʒ���
		//	else if (_closeSector[i] == _closeSector[iPlus] - _sectorSize)
		//	{
		//		_direction = 4;
		//		findWay(_closeSector[i], _closeSector[iPlus]);
		//	}
		//}
		//else
		//{
		iPlus = i + 1;
		//�������� ����
		if (_closeSector[i] == _closeSector[iPlus] + 1)
		{
			_direction = 1;
			findWay(_closeSector[i], _closeSector[iPlus]);
		}
		//���� ���� ������
		else if (_closeSector[i] == _closeSector[iPlus] - 1)
		{
			_direction = 2;
			findWay(_closeSector[i], _closeSector[iPlus]);
		}
		//���� ���� ����
		else if (_closeSector[i] == _closeSector[iPlus] + _sectorSize)
		{
			_direction = 3;
			findWay(_closeSector[i], _closeSector[iPlus]);
		}
		//���� ���� �Ʒ���
		else if (_closeSector[i] == _closeSector[iPlus] - _sectorSize)
		{
			_direction = 4;
			findWay(_closeSector[i], _closeSector[iPlus]);
		}
		//}
		
		
		//3. �� ����
		bildWay2();
	}
}
