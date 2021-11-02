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

	//스페이이이이이이이이이이이이스
	_spaceImg = IMAGEMANAGER->findImage("SPACE");
	_spaceXY = { -400 , -400 };
	_nCount = 0;
	//맵생성
	createMap();
	_sponCheck = 0;

	//카메라
	_camera = new camera;
	_camera->init();

	//Z오더
	_zOrder = new zOrder;

	//플레이어
	_player = new player;
	selectPlayer();

	_bullet = new bullet;
	_bullet->init("BULLET", 1000, 1024.0f);

	_gun = new playerGun;
	_gun->setBullet(_bullet);

	//아이템UI
	_itemManager = new itemManager;
	_itemManager->setPlayer(_player);
	_itemManager->init();

	//======================================================

	_node[0] = 0;		//맵 변환용 노드
	_node[1] = 0;		//맵 변환용 노드
	_node[2] = RND->getInt(2);		//맵 변환용 노드
	_nodeEnd = false;	//맵 변환이 끝났는가?

	//============ 이벤트용 변수 및 입력 변수 ================

	//이벤트용 변수
	isNext = false;
	nextNum = 0;
	
	//이벤트 가능 여부
	_event = true;
	//이벤트 중복 횟수
	_eCont = 0;		
	//이벤트 종료 체크
	_stageEnd = false;

	//보스 미소환
	_isboss = false;

	//키 입력 변수
	_isKey[0] = _isKey[1] = _isKey[2] = _isKey[3] = false;

	return S_OK;
}

void stage1::release()
{
}

void stage1::update()
{
	_player->setTile0(_vMap, _mapSizeX, _mapSizeY);
	//======= 입력 및 계산 ============================================================

	changeTile();

	if (KEYMANAGER->isOnceKeyDown('U'))
	{
		isNext = true;
	}

	//키입력
	keySelect();

	//플레이어의 핸드 타입이 노말이 아닐 경우
	if (_player->getgunType() != PlayerGunType::NORMAL && !_player->getIsRoll())
	{
		_gun->update();
		_gun->setAngle(_player->getAngle());
		//_gun->setSAngle()
		_gun->setPXY(_player->getShadowXY());
	}
	
	//플레이어 업데이트
	_player->update();

	//보스 업데이트
	if (_isboss)
	{
		_boss->update();
		_boss->setPlayerXY(_player->getShadowXY().x, _player->getShadowXY().y);
	}

	//에너미 계산
	for (int i = 0; i < _vEnemy.size(); ++i)
	{
		_vEnemy[i]->update();
		_vEnemy[i]->setPlayerRc(_player->getShadowRc());
		_vEnemy[i]->setPlayerXY(_player->getShadowXY());
	}

	//총알 업데이트
	_bullet->update();
	_itemManager->update();

	//카메라 업데이트 및 플레이어의 이동치 계산
	_camera->update();
	_camera->setCmove(_player->getCMove());

	//========== 타일 업뎃 =============================================================
	//collision()
	//======= z오더 관련 함수 ===========================================================

	//카메라 보정값 전달
	_zOrder->setCmove(_camera->getCurrent());
	_zOrder->setCmouse(_camera->getCMouse());

	//해병
	_zOrder->pushObject(_player);
	
	if (_player->getgunType() != PlayerGunType::NORMAL && !_player->getIsRoll())
	{
		_zOrder->pushObject(_gun);
	}

	//보스
	if (_isboss)
	{
		_zOrder->pushObject(_boss);
	}

	//적
	for (int i = 0; i < _vEnemy.size(); ++i)
	{
		_zOrder->pushObject(_vEnemy[i]);
	}
	//총알
	for (int i = 0; i < _bullet->getVBullet().size(); ++i)
	{
		_zOrder->pushObject(_bullet->getVBullet()[i]);
	}
	//탄피
	for (int i = 0; i < _itemManager->getItem().size(); ++i)
	{
		if (_itemManager->getItem()[i]->getItype() != ITYPE::DROPTAN) continue;
		_zOrder->pushObject((_itemManager->getItem()[i]));
	}

	//업뎃
	_zOrder->update();
	
	//=========== 테스트 ==========================

	//======= 이벤트 업데이트 ==================================================================
	collision();
	playGame();		//게임 시작 조건
	nextStage();	//다음 스테이지 조건
	stageEvent(_sN);//스테이지 이벤트 활성화
	//======= 매니저 업데이트 ==================================================================
	KEYANIMANAGER->update();
	TIMEMANAGER->getElapsedTime();
}

void stage1::render()
{

	_spaceImg->render(getMemDC(), _spaceXY.x - ((_camera->getCurrent().x + _camera->getCMouse().x) / 8 ), _spaceXY.y - (_camera->getCurrent().y + _camera->getCMouse().y) / 8);

	//윗돌맹이
	for (int i = 0; i < _vMap.size(); ++i)
	{
		//화면 밖으로 벗어나면 미출력
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
	//============================================ 이 사이에 그리시오 =====================================================
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
	//아이템 UI등등
	_itemManager->render();

	//보스 등장시 이미지
	if (_isboss)
	{
		_boss->render();
	}

	//for (int i = 0; i < _vEnemy.size(); ++i)
	//{
	//	_vEnemy[i]->render();
	//}
	
	//=====================================================================================================================
	//============================================ 이 사이에 그리시오 =====================================================
	//=====================================================================================================================
	//아래 돌맹이
	for (int i = 0; i < _vMap.size(); ++i)
	{
		//화면 밖으로 벗어나면 미출력
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

//맵생성
void stage1::createMap()
{
	//스테이지에 따른 랜덤맵 설정
	if (_mapStage[_sN].min == _mapStage[_sN].max)
	{
		//고정 생성
		DSS->addTileMap(_mapStage[_sN].min, _vMap, _mapSizeX, _mapSizeY, false);
	}
	else
	{
		//너무 길어서 변수해줌
		int setStage = RND->getFromIntTo(_mapStage[_sN].min, _mapStage[_sN].max);
		//랜덤 생성
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
		//vector<TILE>	_upMap;	//저장용 윗맵
		//vector<TILE>	_downMap;//아랫맵
	
		//RND->getFromIntTo();
	//맵 불러오기
}

//플레이어 설정
void stage1::selectPlayer()
{
	//플레이어 맵세팅

	//플레이어 캐릭터 생성
	for (int i = 0; i < _vMap.size(); ++i)
	{
		if (_vMap[i].oState == SET_PLAYER)
		{
			//해병을 가지고 왔느냐?
			//if (DSS->getPlayerType() == 0)
			//{
				//캐릭터 생성 비콘에 캐릭터를 소환
			_player->init({ (_vMap[i].mpt.x * TILESIZE) + TILESIZE / 2 ,(_vMap[i].mpt.y * TILESIZE) + TILESIZE / 2 }, false, PlayerGunType::NORMAL);
			_player->setFly(500);
			//}
			//아니면 죄수를 가지고 왔느냐
			//else if (DSS->getPlayerType() == 1)
			//{
				//이하동문
			//	_player->init({ (_vMap[i].mpt.x * TILESIZE) + TILESIZE / 2 ,(_vMap[i].mpt.y * TILESIZE) + TILESIZE / 2 }, false, PlayerType::PRISONER,PlayerGunType::NORMAL);
			//	_player->setFly(500);
			//}
		}
	}
}

//캐릭터 플레이 시작
void stage1::playGame()
{
	//플레이어가 활성화 상태라면 종료
	if (_player->getActive()) return;

	//밑으로 내려옴, 전부 내려오면 활성화
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

//충돌체크
void stage1::collision()
{
	RECT tRc;
	RECT bRc;
	RECT cRc;

	for (int j = 0; j < _bullet->getVBullet().size(); ++j)
	{
		bool isgone = false;;
		//총알과 플레이어
		bRc = _bullet->getVBullet()[j].rc;
		cRc = _player->getBodyRc();
		if (IntersectRect(&tRc, &bRc, &cRc)
			&& _bullet->getVBullet()[j].btype != BTYPE::PLAYER
			&& !_player->getIsRoll())
		{
			//뚜껑
			if (_player->getBarrior() != 0) _itemManager->changeBarrior(false);
			else _itemManager->changeHeart(false);

			_player->hit();
			_bullet->removeBullet(j);
			continue;
		}
		//총알과 적
		for (int i = 0; i < _vEnemy.size(); ++i)
		{
			//플레이어의 총알이 아니라면 검사 종료
			if (_bullet->getVBullet()[j].btype != BTYPE::PLAYER) continue;
			cRc = _vEnemy[i]->getBRect();
			if (IntersectRect(&tRc, &bRc, &cRc) && !_vEnemy[i]->getEnemyDie())
			{
				// 적이 맞았을 때
				if (!_vEnemy[i]->getEnemyDie()) _vEnemy[i]->changeState(ENEMYSTATE::HIT);

				// 죽었을 때 바로 들어가기 위해서
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
		//이전에 총알이 사라졌다면
		if (isgone) continue;

	}
}

//버튼 관리
void stage1::keySelect()
{
	if (_player->getActive() && !_player->getIsRoll())
	{
		_isKey[0] = _isKey[1] = _isKey[2] = _isKey[3] = false;
	}
	//키입력

	if (KEYMANAGER->isOnceKeyDown(VK_TAB))
	{
		//정보창 스왑
		_infoNum++;
		if (_infoNum >= 3) _infoNum = 0;
	}

	if (KEYMANAGER->isOnceKeyDown('Q'))
	{
		_itemManager->changeBlankBullet(false);
	}
	//마우스 왼짝버튼
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

	//키사용

	//스페이스바
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

//적 생성
void stage1::createEnemy(POINTF exy)
{
	//랜덤 생성용 지역 변수[0 : 일반][1 : 샷건][2 : 책]
	int ranNum = RND->getInt(3);
	//ranNum = 2;
	//적 생성용
	enemy* em = NULL;

	switch (ranNum)
	{
	case 0:    //일반 건데드
		em = new normalEnemy;
		em->init(ENEMYTYPE::NORMAL, exy);
		em->setBullet(_bullet);
		break;

	case 1:    //샷건 건데드
		em = new shotgunEnemy;
		em->init(ENEMYTYPE::SHOTGUN, exy);
		em->setBullet(_bullet);
		break;

	case 2:    //날아댕기는 책
		em = new bookEnemy;
		em->init(ENEMYTYPE::BOOK, exy);
		//em->setBullet(_bookBullet);
		em->setBullet(_bullet);
		break;
	}

	//em->setCamera(_camera);
	_vEnemy.push_back(em);
}

//스테이지 이벤트
void stage1::stageEvent(int stage)
{
	//플레이어가 이벤트 비컨을 밟은 후 진행
	if (!_player->getEvent() || _stageEnd) return;


	switch (stage)
	{
		case 0:
		{
			if (_event)
			{
				//이벤트 진행 후 중복 소환 방지
				_event = false;
				//맵의 비컨만큼 몬스터를 소환
				for (int i = 0; i < _vMap.size(); ++i)
				{
					if (_vMap[i].oState != SET_ENEMY) continue;
					//적
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
				//이벤트 진행 후 중복 소환 방지
				_event = false;
				//맵의 비컨만큼 몬스터를 소환
				for (int i = 0; i < _vMap.size(); ++i)
				{
					if (_vMap[i].oState != SET_ENEMY) continue;
					//적
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
				//이벤트 진행 후 중복 소환 방지
				_event = false;
				//이벤트 진행 후 중복 소환 방지
				//맵의 비컨만큼 몬스터를 소환
				for (int i = 0; i < _vMap.size(); ++i)
				{
					if (_vMap[i].oState != SET_BOSS) continue;
					//적
					_sponCheck++;
					_isboss = true;
					_boss = new boss;
					_boss->setBullet(_bullet);
					//_boss->init((_vMap[i].mpt.x * TILESIZE) + TILESIZE / 2,(_vMap[i].mpt.y * TILESIZE) + TILESIZE / 2);
					_boss->init((_vMap[i].mpt.x * TILESIZE) + TILESIZE / 2, (_vMap[i].mpt.y * TILESIZE) + TILESIZE / 2);
					
					//보스 등장씬
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

//다음 맵으로 넘어가는 함수
void stage1::nextStage()
{
	if (_player->getState() == PlayerState::FALL)
	{
		isNext = true;
	}

	//다음 스테이지로 넘어가는 이벤트가 활성화
	if (isNext)
	{
		//다음 스테이지로 넘어가기 전 카운트
		nextNum++;
		//카운트가 다 찼다면 다음 스테이지로 이동
		if (nextNum >= 150)
		{
			//스테이지 상승 및 저장
			_sN++;
			DSS->setStage(_sN);

			//적 초기화
			_vEnemy.clear();
			vector<enemy*>().swap(_vEnemy);
			//맵 초기화
			_vMap.clear();
			vector<TILE>().swap(_vMap);
			
			//카메라 제거
			delete _camera;
			//Z오더 제거
			delete _zOrder;

			_player->deleteVMap();

			//플레이어 제거
			delete _player;
			//총 제거
			delete _gun;
			//총알 제거
			delete _bullet;
			//아이템매니저 제거
			delete _itemManager;

			//this->init();
			
			SCENEMANAGER->changeScene("NEXTSTAGE");
		}
	}
}

//끝이 보인다
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

	//맵 생산이 종료시 종료
	if (_nodeEnd) return;
	
	_nCount++;

	if (_nCount % 20 == 0)
	{
		//0:현재 노드
		//1:추가해야할 노드
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
