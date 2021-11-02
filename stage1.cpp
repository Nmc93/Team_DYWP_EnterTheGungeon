#include "pch.h"
#include "stage1.h"

HRESULT stage1::init()
{
	_sN = DSS->getStage();
	
	_mapStage[0].min = 1;
	_mapStage[0].max = 8;
	_mapStage[1].min = 1;
	_mapStage[1].max = 8;
	_mapStage[2].min = 8;
	_mapStage[2].max = 8;

	//���������������������������̽�
	_spaceImg = IMAGEMANAGER->findImage("SPACE");
	_spaceXY = { -400 , -400 };
	_nCount = 0;
	//�ʻ���
	createMap();
	_sponCheck = 0;

	//ī�޶�
	_camera = new camera;
	_camera->init();

	//Z����
	_zOrder = new zOrder;

	//�÷��̾�
	_player = new player;
	selectPlayer();

	_bullet = new bullet;
	_bullet->init("BULLET", 1000, 1024.0f);

	_gun = new playerGun;
	_gun->setBullet(_bullet);

	//������UI
	_itemManager = new itemManager;
	_itemManager->setPlayer(_player);
	_itemManager->init();

	//======================================================

	_node[0] = 0;		//�� ��ȯ�� ���
	_node[1] = 0;		//�� ��ȯ�� ���
	_node[2] = RND->getInt(2);		//�� ��ȯ�� ���
	_nodeEnd = false;	//�� ��ȯ�� �����°�?

	//============ �̺�Ʈ�� ���� �� �Է� ���� ================

	//�̺�Ʈ�� ����
	isNext = false;
	nextNum = 0;
	
	//�̺�Ʈ ���� ����
	_event = true;
	//�̺�Ʈ �ߺ� Ƚ��
	_eCont = 0;		
	//�̺�Ʈ ���� üũ
	_stageEnd = false;

	//���� �̼�ȯ
	_isboss = false;

	//Ű �Է� ����
	_isKey[0] = _isKey[1] = _isKey[2] = _isKey[3] = false;

	return S_OK;
}

void stage1::release()
{
}

void stage1::update()
{
	_player->setTile0(_vMap, _mapSizeX, _mapSizeY);
	//======= �Է� �� ��� ============================================================

	changeTile();

	if (KEYMANAGER->isOnceKeyDown('U'))
	{
		isNext = true;
	}

	//Ű�Է�
	keySelect();

	//�÷��̾��� �ڵ� Ÿ���� �븻�� �ƴ� ���
	if (_player->getgunType() != PlayerGunType::NORMAL && !_player->getIsRoll())
	{
		_gun->update();
		_gun->setAngle(_player->getAngle());
		//_gun->setSAngle()
		_gun->setPXY(_player->getShadowXY());
	}
	
	//�÷��̾� ������Ʈ
	_player->update();

	//���� ������Ʈ
	if (_isboss)
	{
		_boss->update();
		_boss->setPlayerXY(_player->getShadowXY().x, _player->getShadowXY().y);
	}

	//���ʹ� ���
	for (int i = 0; i < _vEnemy.size(); ++i)
	{
		_vEnemy[i]->update();
		_vEnemy[i]->setPlayerRc(_player->getShadowRc());
		_vEnemy[i]->setPlayerXY(_player->getShadowXY());
	}

	//�Ѿ� ������Ʈ
	_bullet->update();
	_itemManager->update();

	//ī�޶� ������Ʈ �� �÷��̾��� �̵�ġ ���
	_camera->update();
	_camera->setCmove(_player->getCMove());

	//========== Ÿ�� ���� =============================================================
	//collision()
	//======= z���� ���� �Լ� ===========================================================

	//ī�޶� ������ ����
	_zOrder->setCmove(_camera->getCurrent());
	_zOrder->setCmouse(_camera->getCMouse());

	//�غ�
	_zOrder->pushObject(_player);
	
	if (_player->getgunType() != PlayerGunType::NORMAL && !_player->getIsRoll())
	{
		_zOrder->pushObject(_gun);
	}

	//����
	if (_isboss)
	{
		_zOrder->pushObject(_boss);
	}

	//��
	for (int i = 0; i < _vEnemy.size(); ++i)
	{
		_zOrder->pushObject(_vEnemy[i]);
	}
	//�Ѿ�
	for (int i = 0; i < _bullet->getVBullet().size(); ++i)
	{
		_zOrder->pushObject(_bullet->getVBullet()[i]);
	}
	//ź��
	for (int i = 0; i < _itemManager->getItem().size(); ++i)
	{
		if (_itemManager->getItem()[i]->getItype() != ITYPE::DROPTAN) continue;
		_zOrder->pushObject((_itemManager->getItem()[i]));
	}

	//����
	_zOrder->update();
	
	//=========== �׽�Ʈ ==========================

	//======= �̺�Ʈ ������Ʈ ==================================================================
	collision();
	playGame();		//���� ���� ����
	nextStage();	//���� �������� ����
	stageEvent(_sN);//�������� �̺�Ʈ Ȱ��ȭ
	//======= �Ŵ��� ������Ʈ ==================================================================
	KEYANIMANAGER->update();
	TIMEMANAGER->getElapsedTime();
}

void stage1::render()
{

	_spaceImg->render(getMemDC(), _spaceXY.x - ((_camera->getCurrent().x + _camera->getCMouse().x) / 8 ), _spaceXY.y - (_camera->getCurrent().y + _camera->getCMouse().y) / 8);

	//��������
	for (int i = 0; i < _vMap.size(); ++i)
	{
		//ȭ�� ������ ����� �����
		if (_vMap[i].rc.left - (_camera->getCurrent().x + _camera->getCMouse().x) > WINSIZEX || _vMap[i].rc.right - (_camera->getCurrent().x + _camera->getCMouse().x) < 0 ||
			_vMap[i].rc.top - (_camera->getCurrent().y + _camera->getCMouse().y) > WINSIZEY || _vMap[i].rc.bottom - (_camera->getCurrent().y + _camera->getCMouse().y) < 0) continue;

		if (!(_vMap[i].tFrame.x == 5 && _vMap[i].tFrame.y == 1) &&
			!(_vMap[i].tFrame.x == 6 && _vMap[i].tFrame.y == 1) &&
			!(_vMap[i].tFrame.x == 7 && _vMap[i].tFrame.y == 1))
		{
			//Rectangle(getMemDC(), _vMap[i].rc);
			IMAGEMANAGER->findImage("MAPSPRITE")->frameRender(getMemDC(), 
				_vMap[i].rc.left - (_camera->getCurrent().x + _camera->getCMouse().x),
				_vMap[i].rc.top - (_camera->getCurrent().y + _camera->getCMouse().y),
				_vMap[i].tFrame.x,
				_vMap[i].tFrame.y);
			//IMAGEMANAGER->findImage("OBJSPRITE")->frameRender(getMemDC(), _map[i].rc.left, _map[i].rc.top, _map[i].oFrame.x, _map[i].oFrame.y);
		}
	}
	//=====================================================================================================================
	//============================================ �� ���̿� �׸��ÿ� =====================================================
	//=====================================================================================================================
	
	//_bullet->render();


	_zOrder->render();

	if (_infoNum == 1)
	{
		_player->render();
	}
	else if (_infoNum == 2)
	{
		_camera->render();
	}
	//������ UI���
	_itemManager->render();

	//���� ����� �̹���
	if (_isboss)
	{
		_boss->render();
	}

	//for (int i = 0; i < _vEnemy.size(); ++i)
	//{
	//	_vEnemy[i]->render();
	//}
	
	//=====================================================================================================================
	//============================================ �� ���̿� �׸��ÿ� =====================================================
	//=====================================================================================================================
	//�Ʒ� ������
	for (int i = 0; i < _vMap.size(); ++i)
	{
		//ȭ�� ������ ����� �����
		if (_vMap[i].rc.left - (_camera->getCurrent().x + _camera->getCMouse().x) > WINSIZEX || _vMap[i].rc.right - (_camera->getCurrent().x + _camera->getCMouse().x) < 0 ||
			_vMap[i].rc.top - (_camera->getCurrent().y + _camera->getCMouse().y) > WINSIZEY || _vMap[i].rc.bottom - (_camera->getCurrent().y + _camera->getCMouse().y) < 0) continue;

		if (!(_vMap[i].tFrame.x == 5 && _vMap[i].tFrame.y == 1) &&
			!(_vMap[i].tFrame.x == 6 && _vMap[i].tFrame.y == 1) &&
			!(_vMap[i].tFrame.x == 7 && _vMap[i].tFrame.y == 1)) continue;

		//Rectangle(getMemDC(), _vMap[i].rc);
		IMAGEMANAGER->findImage("MAPSPRITE")->frameRender(getMemDC(),
			_vMap[i].rc.left - (_camera->getCurrent().x + _camera->getCMouse().x),
			_vMap[i].rc.top - (_camera->getCurrent().y + _camera->getCMouse().y),
			_vMap[i].tFrame.x,
			_vMap[i].tFrame.y);
		//IMAGEMANAGER->findImage("OBJSPRITE")->frameRender(getMemDC(), _map[i].rc.left, _map[i].rc.top, _map[i].oFrame.x, _map[i].oFrame.y);
	}

	_gun->render();

}

//�ʻ���
void stage1::createMap()
{
	//���������� ���� ������ ����
	if (_mapStage[_sN].min == _mapStage[_sN].max)
	{
		//���� ����
		DSS->addTileMap(_mapStage[_sN].min, _vMap, _mapSizeX, _mapSizeY, false);
	}
	else
	{
		//�ʹ� �� ��������
		int setStage = RND->getFromIntTo(_mapStage[_sN].min, _mapStage[_sN].max);
		//���� ����
		DSS->addTileMap(setStage, _vMap, _mapSizeX, _mapSizeY, false);
	}

	/*
	for (int i = 0; i < _vMap.size(); ++i)
	{
		if (_vMap[i].tState == MAP_MOVE)
		{
			_upMap.push_back(_vMap[i]);
		}
		else if (_vMap[i].tState == MAP_UNMOVE)
		{
			_downMap.push_back(_vMap[i]);
		}
	}
	*/
		//vector<TILE>	_upMap;	//����� ����
		//vector<TILE>	_downMap;//�Ʒ���
	
		//RND->getFromIntTo();
	//�� �ҷ�����
}

//�÷��̾� ����
void stage1::selectPlayer()
{
	//�÷��̾� �ʼ���

	//�÷��̾� ĳ���� ����
	for (int i = 0; i < _vMap.size(); ++i)
	{
		if (_vMap[i].oState == SET_PLAYER)
		{
			//�غ��� ������ �Դ���?
			//if (DSS->getPlayerType() == 0)
			//{
				//ĳ���� ���� ���ܿ� ĳ���͸� ��ȯ
			_player->init({ (_vMap[i].mpt.x * TILESIZE) + TILESIZE / 2 ,(_vMap[i].mpt.y * TILESIZE) + TILESIZE / 2 }, false, PlayerGunType::NORMAL);
			_player->setFly(500);
			//}
			//�ƴϸ� �˼��� ������ �Դ���
			//else if (DSS->getPlayerType() == 1)
			//{
				//���ϵ���
			//	_player->init({ (_vMap[i].mpt.x * TILESIZE) + TILESIZE / 2 ,(_vMap[i].mpt.y * TILESIZE) + TILESIZE / 2 }, false, PlayerType::PRISONER,PlayerGunType::NORMAL);
			//	_player->setFly(500);
			//}
		}
	}
}

//ĳ���� �÷��� ����
void stage1::playGame()
{
	//�÷��̾ Ȱ��ȭ ���¶�� ����
	if (_player->getActive()) return;

	//������ ������, ���� �������� Ȱ��ȭ
	if (_player->getFly() > 30)
	{
		_player->setFly(_player->getFly() - 2);
	}
	else
	{
		_player->setFly(30);
		_player->setActive(true);
		_player->setgunType(PlayerGunType::TWOHAND);
		_gun->init(_player->getShadowXY(),GunType::HANDGUN);
		_camera->setIsSelect(true);
	}
}

//�浹üũ
void stage1::collision()
{
	RECT tRc;
	RECT bRc;
	RECT cRc;

	for (int j = 0; j < _bullet->getVBullet().size(); ++j)
	{
		bool isgone = false;;
		//�Ѿ˰� �÷��̾�
		bRc = _bullet->getVBullet()[j].rc;
		cRc = _player->getBodyRc();
		if (IntersectRect(&tRc, &bRc, &cRc)
			&& _bullet->getVBullet()[j].btype != BTYPE::PLAYER
			&& !_player->getIsRoll())
		{
			//�Ѳ�
			if (_player->getBarrior() != 0) _itemManager->changeBarrior(false);
			else _itemManager->changeHeart(false);

			_player->hit();
			_bullet->removeBullet(j);
			continue;
		}
		//�Ѿ˰� ��
		for (int i = 0; i < _vEnemy.size(); ++i)
		{
			//�÷��̾��� �Ѿ��� �ƴ϶�� �˻� ����
			if (_bullet->getVBullet()[j].btype != BTYPE::PLAYER) continue;
			cRc = _vEnemy[i]->getBRect();
			if (IntersectRect(&tRc, &bRc, &cRc) && !_vEnemy[i]->getEnemyDie())
			{
				// ���� �¾��� ��
				if (!_vEnemy[i]->getEnemyDie()) _vEnemy[i]->changeState(ENEMYSTATE::HIT);

				// �׾��� �� �ٷ� ���� ���ؼ�
				if (_vEnemy[i]->getEnemyDie())
				{
					if (!_vEnemy[i]->getDropTan())
					{
						_itemManager->dropMoney(_vEnemy[i]->getShadow());
						_vEnemy[i]->setDropTan(true);
					}
				}
			
				_bullet->removeBullet(j);
				isgone = true;
				break;
			}
		}
		//������ �Ѿ��� ������ٸ�
		if (isgone) continue;

	}
}

//��ư ����
void stage1::keySelect()
{
	if (_player->getActive() && !_player->getIsRoll())
	{
		_isKey[0] = _isKey[1] = _isKey[2] = _isKey[3] = false;
	}
	//Ű�Է�

	if (KEYMANAGER->isOnceKeyDown(VK_TAB))
	{
		//����â ����
		_infoNum++;
		if (_infoNum >= 3) _infoNum = 0;
	}

	if (KEYMANAGER->isOnceKeyDown('Q'))
	{
		_itemManager->changeBlankBullet(false);
	}
	//���콺 ��¦��ư
	if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
	{
		//selectPlayer();
		_gun->isFire();
	}
	if (KEYMANAGER->isOnceKeyDown('R'))
	{
		//selectPlayer();
		_gun->addReload();
	}
	if (KEYMANAGER->isOnceKeyDown(VK_LCONTROL))
	{
		_gun->gunChage();
	}

	//LEFT
	if (KEYMANAGER->isStayKeyDown('A') && !_player->getIsRoll())
	{
		_isKey[0] = true;
	}
	//RIGHT
	if (KEYMANAGER->isStayKeyDown('D') && !_player->getIsRoll())
	{
		_isKey[1] = true;
	}
	//TOP
	if (KEYMANAGER->isStayKeyDown('W') && !_player->getIsRoll())
	{
		_isKey[2] = true;
	}
	//BOTTOM
	if (KEYMANAGER->isStayKeyDown('S') && !_player->getIsRoll())
	{
		_isKey[3] = true;
	}

	//Ű���

	//�����̽���
	if (KEYMANAGER->isStayKeyDown(VK_SPACE) && (_isKey[0] || _isKey[1] || _isKey[2] || _isKey[3]))
	{
		if (!_player->getIsRoll())
		{
			if (_isKey[0] && _isKey[2])
			{
				_player->setFrame({ 8,5 });
			}
			else if (_isKey[1] && _isKey[2])
			{
				_player->setFrame({ 0,4 });
			}
			else if (_isKey[0] && _isKey[3])
			{
				_player->setFrame({ 8,2 });
			}
			else if (_isKey[1] && _isKey[3])
			{
				_player->setFrame({ 0,1 });
			}
			else if (_isKey[0])
			{
				_player->setFrame({ 8,2 });
			}
			else if (_isKey[1])
			{
				_player->setFrame({ 0,1 });
			}
			else if (_isKey[2])
			{
				_player->setFrame({ 0,3 });
			}
			else if (_isKey[3])
			{
				_player->setFrame({ 0,0 });
			}
			_player->addIsRoll();
		}
	}
	//LT
	else if (_isKey[0] && _isKey[2])
	{
		_player->playerMove(PlayerDir::LT);
		if (_player->getgunType() != PlayerGunType::NORMAL)
		{
			_gun->update();
			_gun->setAngle(_player->getAngle());
			_gun->setPXY(_player->getShadowXY());
		}
	}
	//RT
	else if (_isKey[1] && _isKey[2])
	{
		_player->playerMove(PlayerDir::RT);
		if (_player->getgunType() != PlayerGunType::NORMAL)
		{
			_gun->update();
			_gun->setAngle(_player->getAngle());
			_gun->setPXY(_player->getShadowXY());
		}
	}
	//LB
	else if (_isKey[0] && _isKey[3])
	{
		_player->playerMove(PlayerDir::LB);
		if (_player->getgunType() != PlayerGunType::NORMAL)
		{
			_gun->update();
			_gun->setAngle(_player->getAngle());
			_gun->setPXY(_player->getShadowXY());
		}
	}
	//RB
	else if (_isKey[1] && _isKey[3])
	{
		_player->playerMove(PlayerDir::RB);
		if (_player->getgunType() != PlayerGunType::NORMAL)
		{
			_gun->update();
			_gun->setAngle(_player->getAngle());
			_gun->setPXY(_player->getShadowXY());
		}
	}
	//L
	else if (_isKey[0])
	{
		_player->playerMove(PlayerDir::L);
		if (_player->getgunType() != PlayerGunType::NORMAL)
		{
			_gun->update();
			_gun->setAngle(_player->getAngle());
			_gun->setPXY(_player->getShadowXY());
		}
	}
	//R
	else if (_isKey[1])
	{
		_player->playerMove(PlayerDir::R);
		if (_player->getgunType() != PlayerGunType::NORMAL)
		{
			_gun->update();
			_gun->setAngle(_player->getAngle());
			_gun->setPXY(_player->getShadowXY());
		}
	}
	//T
	else if (_isKey[2])
	{
		_player->playerMove(PlayerDir::T);
		if (_player->getgunType() != PlayerGunType::NORMAL)
		{
			_gun->update();
			_gun->setAngle(_player->getAngle());
			_gun->setPXY(_player->getShadowXY());
		}
	}
	//B
	else if (_isKey[3])
	{
		_player->playerMove(PlayerDir::B);
		if (_player->getgunType() != PlayerGunType::NORMAL)
		{
			_gun->update();
			_gun->setAngle(_player->getAngle());
			_gun->setPXY(_player->getShadowXY());
		}
	}
	else if (!_isKey[0] && !_isKey[1] && !_isKey[2] && !_isKey[3])
	{
		_player->playerMove(PlayerDir::CENTER);
	}
}

//�� ����
void stage1::createEnemy(POINTF exy)
{
	//���� ������ ���� ����[0 : �Ϲ�][1 : ����][2 : å]
	int ranNum = RND->getInt(3);
	//ranNum = 2;
	//�� ������
	enemy* em = NULL;

	switch (ranNum)
	{
	case 0:    //�Ϲ� �ǵ���
		em = new normalEnemy;
		em->init(ENEMYTYPE::NORMAL, exy);
		em->setBullet(_bullet);
		break;

	case 1:    //���� �ǵ���
		em = new shotgunEnemy;
		em->init(ENEMYTYPE::SHOTGUN, exy);
		em->setBullet(_bullet);
		break;

	case 2:    //���ƴ��� å
		em = new bookEnemy;
		em->init(ENEMYTYPE::BOOK, exy);
		//em->setBullet(_bookBullet);
		em->setBullet(_bullet);
		break;
	}

	//em->setCamera(_camera);
	_vEnemy.push_back(em);
}

//�������� �̺�Ʈ
void stage1::stageEvent(int stage)
{
	//�÷��̾ �̺�Ʈ ������ ���� �� ����
	if (!_player->getEvent() || _stageEnd) return;


	switch (stage)
	{
		case 0:
		{
			if (_event)
			{
				//�̺�Ʈ ���� �� �ߺ� ��ȯ ����
				_event = false;
				//���� ������ŭ ���͸� ��ȯ
				for (int i = 0; i < _vMap.size(); ++i)
				{
					if (_vMap[i].oState != SET_ENEMY) continue;
					//��
					createEnemy({ (_vMap[i].mpt.x * TILESIZE) + TILESIZE / 2,(_vMap[i].mpt.y * TILESIZE) + TILESIZE / 2 });
					_eCont++;
				}
			}
			int eSize = 0;

			for (int i = 0; i < _vEnemy.size(); ++i)
			{
				if(_vEnemy[i]->getEnemyState() == ENEMYSTATE::DIE) eSize++;
			}

			if (eSize >= _eCont)
			{
				_itemManager->setIsFollow(true);
				_stageEnd = true;
			}

			break;
		}
		case 1:
		{
			if (_event)
			{
				//�̺�Ʈ ���� �� �ߺ� ��ȯ ����
				_event = false;
				//���� ������ŭ ���͸� ��ȯ
				for (int i = 0; i < _vMap.size(); ++i)
				{
					if (_vMap[i].oState != SET_ENEMY) continue;
					//��
					createEnemy({ (_vMap[i].mpt.x * TILESIZE) + TILESIZE / 2,(_vMap[i].mpt.y * TILESIZE) + TILESIZE / 2 });
					_eCont++;
				}
			}
			int eSize = 0;

			for (int i = 0; i < _vEnemy.size(); ++i)
			{
				if (_vEnemy[i]->getEnemyState() == ENEMYSTATE::DIE) eSize++;
			}

			if (eSize >= _eCont)
			{
				_itemManager->setIsFollow(true);
				_stageEnd = true;
			}
			
			break;
		}
		case 2:
		{
			if (_event)
			{
				//�̺�Ʈ ���� �� �ߺ� ��ȯ ����
				_event = false;
				//�̺�Ʈ ���� �� �ߺ� ��ȯ ����
				//���� ������ŭ ���͸� ��ȯ
				for (int i = 0; i < _vMap.size(); ++i)
				{
					if (_vMap[i].oState != SET_BOSS) continue;
					//��
					_sponCheck++;
					_isboss = true;
					_boss = new boss;
					_boss->setBullet(_bullet);
					//_boss->init((_vMap[i].mpt.x * TILESIZE) + TILESIZE / 2,(_vMap[i].mpt.y * TILESIZE) + TILESIZE / 2);
					_boss->init((_vMap[i].mpt.x * TILESIZE) + TILESIZE / 2, (_vMap[i].mpt.y * TILESIZE) + TILESIZE / 2);
					
					//���� �����
					if (_player->getPlayerType() == PlayerType::MARINE) _boss->setWho(true);
					else if(_player->getPlayerType() == PlayerType::PRISONER) _boss->setWho(false);
					_boss->setAppear(true);

					//createEnemy({ (_vMap[i].mpt.x * TILESIZE) + TILESIZE / 2,(_vMap[i].mpt.y * TILESIZE) + TILESIZE / 2 });
				}
			}
			break;
		}
	}
}

//���� ������ �Ѿ�� �Լ�
void stage1::nextStage()
{
	if (_player->getState() == PlayerState::FALL)
	{
		isNext = true;
	}

	//���� ���������� �Ѿ�� �̺�Ʈ�� Ȱ��ȭ
	if (isNext)
	{
		//���� ���������� �Ѿ�� �� ī��Ʈ
		nextNum++;
		//ī��Ʈ�� �� á�ٸ� ���� ���������� �̵�
		if (nextNum >= 150)
		{
			//�������� ��� �� ����
			_sN++;
			DSS->setStage(_sN);

			//�� �ʱ�ȭ
			_vEnemy.clear();
			vector<enemy*>().swap(_vEnemy);
			//�� �ʱ�ȭ
			_vMap.clear();
			vector<TILE>().swap(_vMap);
			
			//ī�޶� ����
			delete _camera;
			//Z���� ����
			delete _zOrder;

			_player->deleteVMap();

			//�÷��̾� ����
			delete _player;
			//�� ����
			delete _gun;
			//�Ѿ� ����
			delete _bullet;
			//�����۸Ŵ��� ����
			delete _itemManager;

			//this->init();
			
			SCENEMANAGER->changeScene("NEXTSTAGE");
		}
	}
}

//���� ���δ�
void stage1::changeTile()
{
	if (!_stageEnd) return;

	for (int i = 0; i < _vMap.size(); ++i)
	{
		if (_node[2] == 0)
		{
			if (_vMap[i].oState == SET_LEFT_WAY)
			{
				_node[0] = i;
			}
		}
		else if (_node[2] == 1)
		{
			if (_vMap[i].oState == SET_RIGHT_WAY)
			{
				_node[0] = i;
			}
		}
	}

	//�� ������ ����� ����
	if (_nodeEnd) return;
	
	_nCount++;

	if (_nCount % 20 == 0)
	{
		//0:���� ���
		//1:�߰��ؾ��� ���
		switch (_node[1])
		{
		case 0:
			_vMap[_node[0] - _mapSizeX - 1].tFrame = { 2,1 };
			break;
		case 1:
			_vMap[_node[0] - _mapSizeX].tFrame = { 3,1 };
			break;
		case 2:
			_vMap[_node[0] - _mapSizeX + 1].tFrame = { 4,1 };
			break;
		case 3:
			_vMap[_node[0] - 1].tFrame = { 2,2 };
			break;
		case 4:
			_vMap[_node[0] + 1].tFrame = { 4,1 };
			break;
		case 5:
			_vMap[_node[0] + _mapSizeX - 1].tFrame = { 2,3 };
			break;
		case 6:
			_vMap[_node[0] + _mapSizeX].tFrame = { 3,3 };
			break;
		case 7:
			_vMap[_node[0] + _mapSizeX + 1].tFrame = { 4,3 };
			break;
		case 8:
			_vMap[_node[0]].tFrame = { 5,6 };
			_vMap[_node[0]].oState = SET_POTAL;
			_nodeEnd = true;
			break;
		}
		_node[1]++;
	}
}
