#include "pch.h"
#include "player.h"

HRESULT player::init(POINTF xy, bool active, PlayerType playertype, PlayerGunType armtype)
{
    //공포탄
    _windImg = IMAGEMANAGER->findImage("WIND");
    //그림자
    _sImg = IMAGEMANAGER->findImage("SHADOW");

    //플레이어 타입 설정
    _pType = playertype;
    _armType = armtype;

    //플레이어 타입에 따른 이미지 생성
    if (playertype == PlayerType::MARINE)
    {
        if (_armType == PlayerGunType::NORMAL)
        {
            _bImg = IMAGEMANAGER->findImage("HELMET_IDLE");
        }
        else if (_armType == PlayerGunType::ONEHAND)
        {
            _bImg = IMAGEMANAGER->findImage("HELMET_1H_IDLE");
        }
        else if (_armType == PlayerGunType::TWOHAND)
        {
            _bImg = IMAGEMANAGER->findImage("HELMET_2H_IDLE");
        }
    }
    else if (playertype == PlayerType::PRISONER)
    {
        if (_armType == PlayerGunType::NORMAL)
        {
            _bImg = IMAGEMANAGER->findImage("PRISONER_IDLE");
        }
        else if (_armType == PlayerGunType::ONEHAND)
        {
            _bImg = IMAGEMANAGER->findImage("PRISONER_1H_IDLE");
        }
        else if (_armType == PlayerGunType::TWOHAND)
        {
            _bImg = IMAGEMANAGER->findImage("PRISONER_2H_IDLE");
        }
    }

    //그림자(중점좌표,렉트)
    _sXY.x = xy.x;
    _sXY.y = xy.y;
    _sRc = RectMakeCenter(_sXY.x, _sXY.y, _sImg->getWidth(), _sImg->getHeight());

    //플레이어(허공값,xy좌표,렉트)
    _fly = 30;
    _bXY.x = _sXY.x;
    _bXY.y = _sXY.y - _fly;
    _bRc = RectMakeCenter(_bXY.x, _bXY.y, _bImg->getFrameWidth(), _bImg->getFrameHeight());

    //플레이어 상태(기본 IDLE, 기본 중앙)
    _pState = PlayerState::IDLE;
    _pDir = PlayerDir::CENTER;

    _frame = { 0,0 };

    //플레이어의 조작여부
    _isControl = active;

    //카메라 보정값
    _cMove.x = _sXY.x - MIDDLEX;
    _cMove.y = _sXY.y - MIDDLEY;

    //알파값
    _alpha = 255;
    
    // UI 소모품
    _heart = 6;
    _maxHeart = 6;
    _blankBullet = 2;
    _barrior = 0;
    _key = 0;
    _money = 0;

    _windWidth = 50;
    _windHeight = 50;

    //방 활성화
    _isStart = false;

	return S_OK;
}

HRESULT player::init(POINTF xy, bool active, PlayerGunType armtype)
{
    //공포탄
    _windImg = IMAGEMANAGER->findImage("WIND");
    //그림자
    _sImg = IMAGEMANAGER->findImage("SHADOW");

    //플레이어 타입 설정
    if (DSS->getPlayerType() == 0)
    {
        _pType = PlayerType::MARINE;
    }
    else if (DSS->getPlayerType() == 1)
    {
        _pType = PlayerType::PRISONER;
    }

    _armType = armtype;

    //플레이어 타입에 따른 이미지 생성
    if (_pType == PlayerType::MARINE)
    {
        if (_armType == PlayerGunType::NORMAL)
        {
            _bImg = IMAGEMANAGER->findImage("HELMET_IDLE");
        }
        else if (_armType == PlayerGunType::ONEHAND)
        {
            _bImg = IMAGEMANAGER->findImage("HELMET_1H_IDLE");
        }
        else if (_armType == PlayerGunType::TWOHAND)
        {
            _bImg = IMAGEMANAGER->findImage("HELMET_2H_IDLE");
        }
    }
    else if (_pType == PlayerType::PRISONER)
    {
        if (_armType == PlayerGunType::NORMAL)
        {
            _bImg = IMAGEMANAGER->findImage("PRISONER_IDLE");
        }
        else if (_armType == PlayerGunType::ONEHAND)
        {
            _bImg = IMAGEMANAGER->findImage("PRISONER_1H_IDLE");
        }
        else if (_armType == PlayerGunType::TWOHAND)
        {
            _bImg = IMAGEMANAGER->findImage("PRISONER_2H_IDLE");
        }
    }

    //그림자(중점좌표,렉트)
    _sXY.x = xy.x;
    _sXY.y = xy.y;
    _sRc = RectMakeCenter(_sXY.x, _sXY.y, _sImg->getWidth(), _sImg->getHeight());

    //플레이어(허공값,xy좌표,렉트)
    _fly = 30;
    _bXY.x = _sXY.x;
    _bXY.y = _sXY.y - _fly;
    _bRc = RectMakeCenter(_bXY.x, _bXY.y, _bImg->getFrameWidth(), _bImg->getFrameHeight());

    //플레이어 상태(기본 IDLE, 기본 중앙)
    _pState = PlayerState::IDLE;
    _pDir = PlayerDir::CENTER;

    _frame = { 0,0 };

    //플레이어의 조작여부
    _isControl = active;

    //카메라 보정값
    _cMove.x = _sXY.x - MIDDLEX;
    _cMove.y = _sXY.y - MIDDLEY;

    //알파값
    _alpha = 255;

    // UI 소모품
    _heart = DSS->getHp();
    _maxHeart = DSS->getMaxHp();
    _blankBullet = DSS->getBlank();
    _barrior = DSS->getBarrior();
    _key = DSS->getKey();
    _money = DSS->getMoney();

    _windWidth = 50;
    _windHeight = 50;

    //방 활성화
    _isStart = false;
    return S_OK;
}

void player::release()
{
}

void player::update()
{
    //===============================================================================
    //플레이어 이동===================================================================
    //===============================================================================

    //플레이어 사망
    if (_heart <= 0)
    {
        _pState = PlayerState::DEAD;
        imageChange();
        _isRoll = true;
    }
    dataSave();
    //마우스 이동
    mouseMove();

    //프레임 연산
    frameUpdate();

    //몸통의 중점
    _bXY.x = _sXY.x;
    _bXY.y = _sXY.y - _fly;

    //그림자와 몸통 위치 변경
    _sRc = RectMakeCenter(_sXY.x, _sXY.y, _sImg->getWidth(), _sImg->getHeight());
    _bRc = RectMakeCenter(_bXY.x, _bXY.y, _bImg->getFrameWidth(), _bImg->getFrameHeight());

    /*
    if (_selectPlayer)
    {
        _normalGun->setBullet(_bullet);
        _normalGun->setCamera(_cam);
        _shotGun->setBullet(_bullet);
        _shotGun->setCamera(_cam);

        // 총 관련
        if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON)) _gun->fire(BTYPE::PLAYER);
        
        if (KEYMANAGER->isOnceKeyDown(VK_CONTROL))
        {
            if      (_gun == _shotGun)   _gun = _normalGun;
            else if (_gun == _normalGun) _gun = _shotGun;
        }

        // 열쇠 추가
        if (KEYMANAGER->isOnceKeyDown('N')) _key++;

        //하트 새로운 것 추가
        /*if (KEYMANAGER->isOnceKeyDown('M'))
        {
            _itemManager->addHeart();
            _heart += 2;
            _maxHeart += 2;
        }

        //하트 반칸 깎임
        if (KEYMANAGER->isOnceKeyDown(VK_F7))
        {
            _itemManager->changeHeart(false);
            if (_heart > 0) _heart--;
        }

        //하트 반칸 추가
        if (KEYMANAGER->isOnceKeyDown(VK_F8))
        {
            _itemManager->changeHeart(true);
            if (_heart < _maxHeart) _heart++;
        }

        //방어막 깎음
        if (KEYMANAGER->isOnceKeyDown(VK_F9)) _itemManager->changeBarrior(false);

        //방어막 추가
        if (KEYMANAGER->isOnceKeyDown(VK_F10)) _itemManager->changeBarrior(true);

        //공포탄 씀
        if (KEYMANAGER->isOnceKeyDown(VK_F11))
        {
            _itemManager->changeBlankBullet(false);
            _isWind = true;
        }
        if (_isWind) wind();

        if (KEYMANAGER->isOnceKeyDown(VK_F12)) _itemManager->changeBlankBullet(true);

        keyMove();
        mouseMove();
        hit();

        _pState->update();
        //_bullet->update();

        if (_selectPlayer) _gun->update(_x, _y);

        if (!_ani->isPlay()) _ani->resume();

        // 구르는 조건
        //if (_playerState == PlayerState::ROLL && 0 < _ani->getNowPlayIndex() && _ani->getNowPlayIndex() < _img->getMaxFrameX()) _isRoll = true;
        if (_playerState == PlayerState::ROLL) _isRoll = true;
        else _isRoll = false;

        _windRc = RectMakeCenter(_windX, _windY, _windImg->getWidth(), _windImg->getHeight());
        _shadowRc = RectMakeCenter(_sX, _sY, _shadowImg->getWidth(), _shadowImg->getHeight());
        _rc = RectMakeCenter(_x, _y, _img->getFrameWidth(), _img->getFrameHeight());
    }
    else
    {
        _shadowRc = RectMakeCenter(_sX - _cam->getX(), _sY - _cam->getY(), _shadowImg->getWidth(), _shadowImg->getHeight());
        _rc = RectMakeCenter(_x - _cam->getX(), _y - _cam->getY(), _img->getFrameWidth(), _img->getFrameHeight());
    }
    */
}

void player::render()
{
    //if (_selectPlayer) _bullet->render();
    //for (int i = 0; i < _vMap.size(); ++i)
    //{
        //Rectangle(getMemDC(),_vMap[i].rc);
    //}
    //Rectangle(getMemDC(), _sRc);

    SetBkMode(getMemDC(), OPAQUE);//TRANSPARENT
    SetTextColor(getMemDC(), RGB(0, 0, 0));

    RECT tRc = RectMake(5, 306, 125,130);
    Rectangle(getMemDC(), tRc);

    char str[128];
    sprintf_s(str, "플레이어 정보");
    TextOut(getMemDC(), 10, 300, str, strlen(str));
    if (_pType == PlayerType::MARINE)
    {
        sprintf_s(str, "Type         : 해병");
        TextOut(getMemDC(), 10, 316, str, strlen(str));
    }
    else if (_pType == PlayerType::PRISONER)
    {
        sprintf_s(str, "Type         : 죄수");
        TextOut(getMemDC(), 10, 316, str, strlen(str));
    }
    
    sprintf_s(str, "frame   : [X:%d][Y:%d]",_frame.x,_frame.y);
    TextOut(getMemDC(), 10, 332, str, strlen(str));
    sprintf_s(str, "ShadowX : %0.1f", _sXY.x);
    TextOut(getMemDC(), 10, 348, str, strlen(str));
    sprintf_s(str, "ShadowY : %0.1f", _sXY.y);
    TextOut(getMemDC(), 10, 364, str, strlen(str));
    sprintf_s(str, "BodyX     : %0.1f", _bXY.x);
    TextOut(getMemDC(), 10, 380, str, strlen(str));
    sprintf_s(str, "BodyY     : %0.1f", _bXY.y);
    TextOut(getMemDC(), 10, 396, str, strlen(str));
    sprintf_s(str, "angle       : %0.3f", _angle);
    TextOut(getMemDC(), 10, 412, str, strlen(str));//(_angle + PI) * 180 / PI

	//draw();
}

//플레이어 이동
void player::playerMove(PlayerDir pd)
{
    if (!_isControl) return;
    if(_pState == PlayerState::FALL || _pState == PlayerState::DEAD)return;
    //_cMove = { 0,0 };

    //리동
    switch (pd)
    {
        //움직이지 않음
        case PlayerDir::CENTER:
        {
            if (_pState != PlayerState::ROLL)
            {
                _pState = PlayerState::IDLE;
            }
            imageChange();
            break;
        }
        //왼쪽
        case PlayerDir::L:
        {
            if (_pState == PlayerState::ROLL)
            {
                _sXY.x -= ROLLSPEED+1;
                _cMove.x -= ROLLSPEED+1;
            }
            else
            {
                _pState = PlayerState::WALK;
                _sXY.x -= MOVESPPED;
                _cMove.x -= MOVESPPED;
            }
            imageChange();
            break;
        }
        //왼쪽위
        case PlayerDir::LT:
        {
            if (_pState == PlayerState::ROLL)
            {
                _sXY.x -= ROLLSPEED - 1;
                _sXY.y -= ROLLSPEED - 1;
                _cMove.x -= ROLLSPEED - 1;
                _cMove.y -= ROLLSPEED - 1;
            }
            else
            {
                _sXY.x -= MOVESPPED;
                _sXY.y -= MOVESPPED;
                _cMove.x -= MOVESPPED;
                _cMove.y -= MOVESPPED;
                _pState = PlayerState::WALK;
            }
            imageChange();
            break;
        }
        //위
        case PlayerDir::T:
        {
            if (_pState == PlayerState::ROLL)
            {
                _sXY.y -= ROLLSPEED;
                _cMove.y -= ROLLSPEED;
            }
            else
            {
                _sXY.y -= MOVESPPED;
                _cMove.y -= MOVESPPED;
                _pState = PlayerState::WALK;
            }
            imageChange();
            break;
        }
        //오른쪽위
        case PlayerDir::RT:
        {
            if (_pState == PlayerState::ROLL)
            {
                _sXY.x += ROLLSPEED - 1;
                _sXY.y -= ROLLSPEED - 1;
                _cMove.x += ROLLSPEED - 1;
                _cMove.y -= ROLLSPEED - 1;
            }
            else
            {
                _sXY.x += MOVESPPED;
                _sXY.y -= MOVESPPED;
                _cMove.x += MOVESPPED;
                _cMove.y -= MOVESPPED;
                _pState = PlayerState::WALK;
            }
            imageChange();
            break;
        }
        //오른쪽
        case PlayerDir::R:
        {
            if (_pState == PlayerState::ROLL)
            {
                _sXY.x += ROLLSPEED+1;
                _cMove.x += ROLLSPEED+1;
            }
            else
            {
                _sXY.x += MOVESPPED;
                _cMove.x += MOVESPPED;
                _pState = PlayerState::WALK;
            }
            imageChange();
            break;
        }
        //오른쪽 아래
        case PlayerDir::RB:
        {
            if (_pState == PlayerState::ROLL)
            {
                _sXY.x += ROLLSPEED - 1;
                _sXY.y += ROLLSPEED - 1;
                _cMove.x += ROLLSPEED - 1;
                _cMove.y += ROLLSPEED - 1;
            }
            else
            {
                _sXY.x += MOVESPPED;
                _sXY.y += MOVESPPED;
                _cMove.x += MOVESPPED;
                _cMove.y += MOVESPPED;
                _pState = PlayerState::WALK;
            }
            imageChange();
            break;
        }
        //아래
        case PlayerDir::B:
        {
            if (_pState == PlayerState::ROLL)
            {
                _sXY.y += ROLLSPEED;
                _cMove.y += ROLLSPEED;
            }
            else
            {
                _sXY.y += MOVESPPED;
                _cMove.y += MOVESPPED;
                _pState = PlayerState::WALK;
            }
            imageChange();
            break;
        }
        //왼쪽 아래
        case PlayerDir::LB:
        {
            if (_pState == PlayerState::ROLL)
            {
                _sXY.x -= ROLLSPEED - 1;
                _sXY.y += ROLLSPEED - 1;
                _cMove.x -= ROLLSPEED - 1;
                _cMove.y += ROLLSPEED - 1;
            }
            else
            {
                _sXY.x -= MOVESPPED;
                _sXY.y += MOVESPPED;
                _cMove.x -= MOVESPPED;
                _cMove.y += MOVESPPED;
                _pState = PlayerState::WALK;
            }
            imageChange();
            break;
        }
    }

    _bXY.x = _sXY.x;
    _bXY.y = _sXY.y - _fly;
    //그림자와 몸통 위치 변경
    _sRc = RectMakeCenter(_sXY.x, _sXY.y, _sImg->getWidth(), _sImg->getHeight());
    _bRc = RectMakeCenter(_bXY.x, _bXY.y, _bImg->getFrameWidth(), _bImg->getFrameHeight());

    //충돌체크,
    playerTile(pd);
}

//프레임 업데이트
void player::frameUpdate()
{
    //프레임 카운트
    int maxFrame = 10;

    if (_pState == PlayerState::ROLL)
    {
        maxFrame = 5;
    }

    _frameCount++;

    //프레임 업
    if (_frameCount % maxFrame == 0)
    {
        if(_pState == PlayerState::FALL)
        {
            //끝이 없는 프레임
            _frame.x++;//if(_frame.x <= _bImg->getMaxFrameX())
            //if (_frame.x > _bImg->getMaxFrameX()) _frame.x = 0;
            _isRoll = true;
        }
        else if (_pState == PlayerState::ROLL)
        {
            if (_frame.y == 5 || _frame.y == 2)
            {
                _frame.x--;
                if (_frame.x < 0)
                {
                    //프레임 종료 후 대기로 변경
                    _pState = PlayerState::IDLE;
                    _isRoll = false;
                    imageChange();
                    _frame.x = 0;
                }
            }
            else
            {
                _frame.x++;
                if (_frame.x > _bImg->getMaxFrameX())
                {
                    //프레임 종료 후 대기로 변경
                    _pState = PlayerState::IDLE;
                    _isRoll = false;
                    imageChange();
                    _frame.x = 0;
                }
            }
        }
        else if (_pState == PlayerState::DEAD)
        {
            _frame.y = 0;
            if(_frame.x < _bImg->getMaxFrameX())_frame.x++;
        }
        else
        {
            _frame.x++;
            if (_frame.x > _bImg->getMaxFrameX()) _frame.x = 0;
        }
    }
}

//이미지 삽입
void player::imageChange()
{
    //해병병이
    if (_pType == PlayerType::MARINE)
    {
        //양손
        if (_armType == PlayerGunType::NORMAL)
        {
            //기본 상태
            if (_pState == PlayerState::IDLE)
            {
                _bImg = IMAGEMANAGER->findImage("HELMET_IDLE");
            }
            //걷는 상태
            else if (_pState == PlayerState::WALK)
            {
                _bImg = IMAGEMANAGER->findImage("HELMET_WALK");
            }
            //떨어짐
            else if (_pState == PlayerState::FALL)
            {
                _bImg = IMAGEMANAGER->findImage("HELMET_FALL");
                _sImg = IMAGEMANAGER->findImage("NULLSHADOW");
            }
            else if (_pState == PlayerState::ROLL)
            {
                _bImg = IMAGEMANAGER->findImage("HELMET_ROLL");//HELMET_ROLL
            }
            else if (_pState == PlayerState::DEAD)
            {
                _bImg = IMAGEMANAGER->findImage("HELMET_DEAD");
            }
        }
        //한손
        else if (_armType == PlayerGunType::ONEHAND)
        {
            //기본 상태
            if (_pState == PlayerState::IDLE)
            {
                _bImg = IMAGEMANAGER->findImage("HELMET_1H_IDLE");
            }
            //걷는 상태
            else if (_pState == PlayerState::WALK)
            {
                _bImg = IMAGEMANAGER->findImage("HELMET_1H_WALK");
            }
            //떨어짐
            else if (_pState == PlayerState::FALL)
            {
                _bImg = IMAGEMANAGER->findImage("HELMET_FALL");
                _sImg = IMAGEMANAGER->findImage("NULLSHADOW");
            }
            else if (_pState == PlayerState::ROLL)
            {
                _bImg = IMAGEMANAGER->findImage("HELMET_ROLL");//HELMET_ROLL
            }
            else if (_pState == PlayerState::DEAD)
            {
                _bImg = IMAGEMANAGER->findImage("HELMET_DEAD");
            }
        }
        //손리스
        else if (_armType == PlayerGunType::TWOHAND)
        {
            //기본 상태
            if (_pState == PlayerState::IDLE)
            {
                _bImg = IMAGEMANAGER->findImage("HELMET_2H_IDLE");
            }
            //걷는 상태
            else if (_pState == PlayerState::WALK)
            {
                _bImg = IMAGEMANAGER->findImage("HELMET_2H_WALK");
            }
            //떨어짐
            else if (_pState == PlayerState::FALL)
            {
                _bImg = IMAGEMANAGER->findImage("HELMET_FALL");
                _sImg = IMAGEMANAGER->findImage("NULLSHADOW");
            }
            else if (_pState == PlayerState::ROLL)
            {
                _bImg = IMAGEMANAGER->findImage("HELMET_ROLL");//HELMET_ROLL
            }
            else if (_pState == PlayerState::DEAD)
            {
                _bImg = IMAGEMANAGER->findImage("HELMET_DEAD");
            }
        }
    }
    //죄수
    else if (_pType == PlayerType::PRISONER)
    {
        //양손
        if (_armType == PlayerGunType::NORMAL)
        {
            if (_pState == PlayerState::IDLE)
            {
                _bImg = IMAGEMANAGER->findImage("PRISONER_IDLE");
            }
            else if (_pState == PlayerState::WALK)
            {
                _bImg = IMAGEMANAGER->findImage("PRISONER_WALK");
            }
            else if (_pState == PlayerState::FALL)
            {
                _bImg = IMAGEMANAGER->findImage("PRISONER_FALL");
                _sImg = IMAGEMANAGER->findImage("NULLSHADOW");
            }
            else if (_pState == PlayerState::ROLL)
            {
                _bImg = IMAGEMANAGER->findImage("PRISONER_ROLL");//PRISONER_ROLL
            }
            else if (_pState == PlayerState::DEAD)
            {
                _bImg = IMAGEMANAGER->findImage("PRISONER_DEAD");
            }
        }
        //한손
        else if (_armType == PlayerGunType::ONEHAND)
        {
            if (_pState == PlayerState::IDLE)
            {
                _bImg = IMAGEMANAGER->findImage("PRISONER_1H_IDLE");
            }
            else if (_pState == PlayerState::WALK)
            {
                _bImg = IMAGEMANAGER->findImage("PRISONER_1H_WALK");
            }
            else if (_pState == PlayerState::FALL)
            {
                _bImg = IMAGEMANAGER->findImage("PRISONER_FALL");
                _sImg = IMAGEMANAGER->findImage("NULLSHADOW");
            }
            else if (_pState == PlayerState::ROLL)
            {
                _bImg = IMAGEMANAGER->findImage("PRISONER_ROLL");//PRISONER_ROLL
            }
            else if (_pState == PlayerState::DEAD)
            {
                _bImg = IMAGEMANAGER->findImage("PRISONER_DEAD");
            }
        }
        //손리스
        else if (_armType == PlayerGunType::TWOHAND)
        {
            if (_pState == PlayerState::IDLE)
            {
                _bImg = IMAGEMANAGER->findImage("PRISONER_2H_IDLE");
            }
            else if (_pState == PlayerState::WALK)
            {
                _bImg = IMAGEMANAGER->findImage("PRISONER_2H_WALK");
            }
            else if (_pState == PlayerState::FALL)
            {
                _bImg = IMAGEMANAGER->findImage("PRISONER_FALL");
                _sImg = IMAGEMANAGER->findImage("NULLSHADOW");
            }
            else if (_pState == PlayerState::ROLL)
            {
                _bImg = IMAGEMANAGER->findImage("PRISONER_ROLL");//PRISONER_ROLL
            }
            else if (_pState == PlayerState::DEAD)
            {
                _bImg = IMAGEMANAGER->findImage("PRISONER_DEAD");
            }
        }
    }
    //_pState
}

void player::mouseMove() //삼각함수 이용해서 플레이어 x,y랑 마우스 ptmouse 각도 계산해서 방향 정의
{
    if (_pState == PlayerState::ROLL ||
        _pState == PlayerState::FALL ||
        _pState == PlayerState::DEAD) return;

    if (!_isRoll && _isControl)
    {
        //_angle = atan2f(_sXY.y - _ptMouse.y, _sXY.x - _ptMouse.x);
        _angle = atan2f(MIDDLEY - _ptMouse.y, MIDDLEX - _ptMouse.x);
        float angle = (_angle +PI) * 180 / PI;
        //if (_angle > PI / 6 && angle < PI - PI / 6)
        //if(_angle > )
        
        //1 : ++
        if (MIDDLEX < _ptMouse.x && MIDDLEY < _ptMouse.y)
        {
            if(angle >= 45.0f) _frame.y = 0;
            else if(angle < 45.0f) _frame.y = 1;
        }
        //2:-+
        else if (MIDDLEX > _ptMouse.x && MIDDLEY < _ptMouse.y)
        {
            if (angle >= 135.0f) _frame.y = 2;
            else if (angle < 135.0f) _frame.y = 0;
        }
        //3:-- S
        else if (MIDDLEX > _ptMouse.x && MIDDLEY > _ptMouse.y)
        {
            if (angle < 225.0f) _frame.y = 2;
            else if (angle >= 225.0f && angle < 247.5) _frame.y = 5;
            else if(angle >= 247.5f) _frame.y = 3;
        }
        //4:+-
        else if (MIDDLEX < _ptMouse.x && MIDDLEY > _ptMouse.y)
        {
            if (angle > 315.0f) _frame.y = 1;
            else if (angle <= 315.0f && angle > 292.5f) _frame.y = 4;
            else if (angle <= 292.5f)_frame.y = 3;
        }
    }
    
}

void player::draw()
{
    if (_isWind) _windImg->render(getMemDC(), _windRc.left, _windRc.top);

    gunDraw();
    
    char str[128];
    sprintf_s(str, "%d: %d", _vMap[0].rc.left, _vMap[0].rc.top);
    TextOut(getMemDC(), 0, 340, str, strlen(str));

    // 확인용
    if (KEYMANAGER->isToggleKey(VK_TAB))
    {
        //Rectangle(getMemDC(), _rc);

        //for (int i = 0; i < 4; i++)
        //{
        //    Rectangle(getMemDC(), _rcCollision[i]);
        //}

        //sprintf_s(str, "_widthDir : %d", _widthDir);
        //TextOut(getMemDC(), 0, 200, str, strlen(str));

        //sprintf_s(str, "_heightDir : %d", _heightDir);
        //TextOut(getMemDC(), 0, 220, str, strlen(str));

        //sprintf_s(str, "_isPlay : %d", _ani->isPlay());
        //TextOut(getMemDC(), 0, 240, str, strlen(str));

        /*sprintf_s(str, "_nowPlayIndex : %d", _ani->getNowPlayIndex());
        TextOut(getMemDC(), 0, 260, str, strlen(str));

        sprintf_s(str, "getFrameSec : %f", _ani->getFrameSec());
        TextOut(getMemDC(), 0, 280, str, strlen(str));

        sprintf_s(str, "getElapseSec : %f", _ani->getElapseSec());
        TextOut(getMemDC(), 0, 300, str, strlen(str));

        sprintf_s(str, "time : %f", KEYANIMANAGER->getTime());
        TextOut(getMemDC(), 0, 320, str, strlen(str));*/

        sprintf_s(str, "%d: %d", _vMap[0].rc.left, _vMap[0].rc.top);
        TextOut(getMemDC(), 0, 340, str, strlen(str));

        //sprintf_s(str, "_angle : %f", angle);
        //TextOut(getMemDC(), 0, 360, str, strlen(str));

        //sprintf_s(str, "_shadowX, _shadowY : %d,%d", _sX, _sY);
        //TextOut(getMemDC(), 0, 520, str, strlen(str));

        //sprintf_s(str, "_X, _Y : %d,%d", _x, _y);
        //TextOut(getMemDC(), 0, 500, str, strlen(str));

        //sprintf_s(str, "_tileIndex : %d", _tileIndex[0]);
        //TextOut(getMemDC(), 0, 460, str, strlen(str));

        //sprintf_s(str, "_angle : %.2f", (_angle + PI));
        //TextOut(getMemDC(), 0, 460, str, strlen(str));
    }

}

void player::gunDraw()
{
    /*
    if (_selectPlayer)
    {
        if (5 < _gun->getFrameX() && _gun->getFrameX() < 18)
        {
            _sImg->render(getMemDC(), _sRc.left, _sRc.top);
            _bImg->alphaAniRender(getMemDC(), _rc.left, _rc.top, _ani, _alpha);
            _gun->render();
        }
        else
        {
            _gun->render();
            _sImg->render(getMemDC(), _sRc.left, _sRc.top);
            _bImg->alphaAniRender(getMemDC(), _rc.left, _rc.top, _ani, _alpha);
        }
    }
    else
    {
        _sImg->render(getMemDC(), _sRc.left, _sRc.top);
        _bImg->alphaAniRender(getMemDC(), _rc.left, _rc.top, _ani, _alpha);
    }
    */
}

void player::hit()
{
    if (_barrior != 0) _barrior--;
    else _heart--;
    /*
    if (KEYMANAGER->isOnceKeyDown(VK_F4)) _isHit = true;

    if (!_isHit) _alphaCount = 0;
    else
    {
        _alphaCount++;  // 얼마나 깜빡거릴지

        if (_alphaCount % 20 == 0) _alpha = 0;
        else if (_alphaCount % 10 == 0) _alpha = 255;

        if (_alphaCount > 50) _isHit = false;
    }
    */
}

void player::wind()
{
    _windWidth += 20;
    _windHeight += 20;

    if (_windWidth >= WINSIZEY)
    {
        _windWidth = 50;
        _windHeight = 50;
        _isWind = false;
    }

    IMAGEMANAGER->deleteImage("WIND");
    _windImg = IMAGEMANAGER->addImage("WIND", "image/ui/wind.bmp", _windWidth, _windHeight, true, RGB(255, 0, 255));
}

//충돌체크
void player::playerTile(PlayerDir pd)
{
    //구 방법
    /*
    int left = _sRc.left;
    int right = _sRc.right;
    int top = _sRc.top;
    int bottom = _sRc.bottom;

    int tileSize = tileWidth * tileHeight;

    for (int i = 0; i < tileSize; i++)
    {
        // 충돌
        switch (_rollWidthDir)
        {
        case PlayerDir::L:
            if (PtInRect(&tile[i].rc, PointMake(left, top))) _tileIndex[0] = i;
            if (PtInRect(&tile[i].rc, PointMake(left, bottom))) _tileIndex[1] = i;

            break;

        case PlayerDir::RIGHT:
            if (PtInRect(&tile[i].rc, PointMake(right, top))) _tileIndex[0] = i;
            if (PtInRect(&tile[i].rc, PointMake(right, bottom))) _tileIndex[1] = i;

            break;  
        }

        switch (_rollHeightDir)
        {
        case PlayerDir::UP:
            if (_rollWidthDir == PlayerDir::CENTER)
            {
                if (PtInRect(&tile[i].rc, PointMake(left, top))) _tileIndex[2] = i;
                if (PtInRect(&tile[i].rc, PointMake(right, top))) _tileIndex[3] = i;
            }
           
            break;

        case PlayerDir::DOWN:
            if (_rollWidthDir == PlayerDir::CENTER) 
            {
                if (PtInRect(&tile[i].rc, PointMake(left, bottom))) _tileIndex[2] = i;
                if (PtInRect(&tile[i].rc, PointMake(right, bottom))) _tileIndex[3] = i;
            }
            
            break;
        }
    }

    bool obj = false;

    for (int i = 0; i < 4; i++)
    {
        if (tile[_tileIndex[i]].tState == MAP_UNMOVE)
        {
            if (i < 2)
            {
                switch (_rollWidthDir)
                {
                case PlayerDir::LEFT:  _cam->setCX(_cam->getCX() + 5);      
                    break;

                case PlayerDir::RIGHT: _cam->setCX(_cam->getCX() - 5);
                    break;
                }
            }
            else
            {
                switch (_rollHeightDir)
                {
                case PlayerDir::UP:   _cam->setCY(_cam->getCY() + 5);
                    break;

                case PlayerDir::DOWN: _cam->setCY(_cam->getCY() - 5);
                    break;
                }
            }
        }
    }

    _rcCollision[0] = tile[_tileIndex[0]].rc;
    _rcCollision[1] = tile[_tileIndex[1]].rc;
    _rcCollision[2] = tile[_tileIndex[2]].rc;
    _rcCollision[3] = tile[_tileIndex[3]].rc;
    */

    //개별인식
    /*
    //플레어이어의 발밑 타일
    int center = (_sXY.y * _tileMax.x + _sXY.x);
 
    int tile[8];
    tile[0] = center - _tileMax.x;      //12시 방향
    tile[1] = center - _tileMax.x + 1;  //2시 방향
    tile[2] = center + 1;               //3시 방향
    tile[3] = center + _tileMax.x + 1;  //5시 방향
    tile[4] = center + _tileMax.x;      //6시 방향
    tile[5] = center + _tileMax.x - 1;  //8시 방향
    tile[6] = center - 1;               //9시 방향
    tile[7] = center - _tileMax.x - 1;  //10시 방향

    //2키 신호의 경우
    if (pd == PlayerDir::LB || pd == PlayerDir::LT ||
        pd == PlayerDir::RB || pd == PlayerDir::RT)
    {
        switch (pd)
        {
            case PlayerDir::LT:
            {
                //위쪽
                RECT tRc;
                if (IntersectRect(&tRc, &_sRc, &_vTile[tile[0]].rc) && _vTile[tile[0]].tState == MAP_UNMOVE)
                {
                    _sXY.y += tRc.bottom - _sRc.top; // MOVESPPED
                }
                //왼짝
                if (IntersectRect(&tRc, &_sRc, &_vTile[tile[6]].rc) && _vTile[tile[6]].tState == MAP_UNMOVE)
                {
                    _sXY.x += tRc.right - _sRc.left;
                }

                //중앙에서 오른쪽이라면
                if ((_vTile[center].mpt.x + TILESIZE / 2) <= _sXY.x)
                {
                    if (IntersectRect(&tRc, &_sRc, &_vTile[tile[1]].rc) && _vTile[tile[1]].tState == MAP_UNMOVE)
                    {
                        _sXY.y += tRc.bottom - _sRc.top; // MOVESPPED;
                    }
                }
                //중앙에서 왼쪽이라면
                else if ((_vTile[center].mpt.x + TILESIZE / 2) > _sXY.x)
                {
                    if (IntersectRect(&tRc, &_sRc, &_vTile[tile[7]].rc) && _vTile[tile[7]].tState == MAP_UNMOVE)
                    {
                        _sXY.y += tRc.bottom - _sRc.top; // MOVESPPED;
                    }
                }

                break;
            }
            case PlayerDir::RT:
            {
                //위쪽
                RECT tRc;
                if (IntersectRect(&tRc, &_sRc, &_vTile[tile[0]].rc) && _vTile[tile[0]].tState == MAP_UNMOVE)
                {
                    _sXY.y += tRc.bottom - _sRc.top;
                }
                //오른짝
                if (IntersectRect(&tRc, &_sRc, &_vTile[tile[2]].rc) && _vTile[tile[2]].tState == MAP_UNMOVE)
                {
                    _sXY.x -= _sRc.left - tRc.right;
                }

                //중앙에서 오른쪽이라면
                if ((_vTile[center].mpt.x + TILESIZE / 2) <= _sXY.x)
                {
                    //오른쪽 위
                    if (IntersectRect(&tRc, &_sRc, &_vTile[tile[1]].rc) && _vTile[tile[1]].tState == MAP_UNMOVE)
                    {
                        _sXY.y += tRc.bottom - _sRc.top;
                    }
                }
                //중앙에서 왼쪽이라면
                else if ((_vTile[center].mpt.x + TILESIZE / 2) > _sXY.x)
                {
                    //왼쪽 위
                    if (IntersectRect(&tRc, &_sRc, &_vTile[tile[7]].rc) && _vTile[tile[7]].tState == MAP_UNMOVE)
                    {
                        _sXY.y += tRc.bottom - _sRc.top;
                    }
                }
                break;
            }
            case PlayerDir::LB:
            {
                //아래쪽
                RECT tRc;
                if (IntersectRect(&tRc, &_sRc, &_vTile[tile[4]].rc) && _vTile[tile[4]].tState == MAP_UNMOVE)
                {
                    _sXY.y -= _sRc.bottom - tRc.top;
                }
                //왼쪽
                if (IntersectRect(&tRc, &_sRc, &_vTile[tile[6]].rc) && _vTile[tile[6]].tState == MAP_UNMOVE)
                {
                    _sXY.x += tRc.right - _sRc.left;
                }
                //중앙에서 오른쪽이라면
                if ((_vTile[center].mpt.x + TILESIZE / 2) <= _sXY.x)
                {
                    if (IntersectRect(&tRc, &_sRc, &_vTile[tile[3]].rc) && _vTile[tile[3]].tState == MAP_UNMOVE)
                    {
                        _sXY.y -= _sRc.bottom - tRc.top;
                    }
                }
                //중앙에서 왼쪽이라면
                else if ((_vTile[center].mpt.x + TILESIZE / 2) > _sXY.x)
                {
                    if (IntersectRect(&tRc, &_sRc, &_vTile[tile[5]].rc) && _vTile[tile[5]].tState == MAP_UNMOVE)
                    {
                        _sXY.y -= _sRc.bottom - tRc.top;
                    }
                }
                break;
            }
            case PlayerDir::RB:
            {
                //아래쪽
                RECT tRc;
                if (IntersectRect(&tRc, &_sRc, &_vTile[tile[4]].rc) && _vTile[tile[4]].tState == MAP_UNMOVE)
                {
                    _sXY.y -= _sRc.bottom - tRc.top;
                }
                //오른쪽
                if (IntersectRect(&tRc, &_sRc, &_vTile[tile[2]].rc) && _vTile[tile[2]].tState == MAP_UNMOVE)
                {
                    _sXY.x -= tRc.right - _sRc.left;
                }
                //중앙에서 오른쪽이라면
                if ((_vTile[center].mpt.x + TILESIZE / 2) <= _sXY.x)
                {
                    if (IntersectRect(&tRc, &_sRc, &_vTile[tile[3]].rc) && _vTile[tile[3]].tState == MAP_UNMOVE)
                    {
                        _sXY.y -= _sRc.bottom - tRc.top;
                    }
                }
                //중앙에서 왼쪽이라면
                else if ((_vTile[center].mpt.x + TILESIZE / 2) > _sXY.x)
                {
                    if (IntersectRect(&tRc, &_sRc, &_vTile[tile[5]].rc) && _vTile[tile[5]].tState == MAP_UNMOVE)
                    {
                        _sXY.y -= _sRc.bottom - tRc.top;
                    }
                }
                break;
            }
        }
    }
    //2키 신호가 아닐 경우(1키 신호)
    else
    {
        switch (pd)
        {
            case PlayerDir::L:
            {
                //왼짝
                RECT tRc;
                if (IntersectRect(&tRc, &_sRc, &_vTile[tile[6]].rc) && _vTile[tile[6]].tState == MAP_UNMOVE)
                {
                    _sXY.x += tRc.left - _sRc.right;
                }
                //중앙에서 오른쪽이라면
                if ((_vTile[center].mpt.y + TILESIZE / 2) <= _sXY.y)
                {
                    if (IntersectRect(&tRc, &_sRc, &_vTile[tile[5]].rc) && _vTile[tile[5]].tState == MAP_UNMOVE)
                    {
                        _sXY.x += tRc.left - _sRc.right;
                    }
                }
                //중앙에서 왼쪽이라면
                else if ((_vTile[center].mpt.y + TILESIZE / 2) > _sXY.y)
                {
                    if (IntersectRect(&tRc, &_sRc, &_vTile[tile[7]].rc) && _vTile[tile[7]].tState == MAP_UNMOVE)
                    {
                        _sXY.x += tRc.left - _sRc.right;
                    }
                }
                break;
            }
            case PlayerDir::R:
            {
                //왼짝
                RECT tRc;
                if (IntersectRect(&tRc, &_sRc, &_vTile[tile[2]].rc) && _vTile[tile[2]].tState == MAP_UNMOVE)
                {
                    _sXY.x -= _sRc.left - tRc.right;
                }
                //중앙에서 오른쪽이라면
                if ((_vTile[center].mpt.y + TILESIZE / 2) <= _sXY.y)
                {
                    if (IntersectRect(&tRc, &_sRc, &_vTile[tile[3]].rc) && _vTile[tile[3]].tState == MAP_UNMOVE)
                    {
                        _sXY.x -= _sRc.left - tRc.right;
                    }
                }
                //중앙에서 왼쪽이라면
                else if ((_vTile[center].mpt.y + TILESIZE / 2) > _sXY.y)
                {
                    if (IntersectRect(&tRc, &_sRc, &_vTile[tile[1]].rc) && _vTile[tile[1]].tState == MAP_UNMOVE)
                    {
                        _sXY.x -= _sRc.left - tRc.right;
                    }
                }
                break;
            }
            case PlayerDir::T:
            {
                //왼짝
                RECT tRc;
                if (IntersectRect(&tRc, &_sRc, &_vTile[tile[0]].rc) && _vTile[tile[0]].tState == MAP_UNMOVE)
                {
                    _sXY.x += tRc.bottom - _sRc.top;
                }
                //중앙에서 오른쪽이라면
                if ((_vTile[center].mpt.x + TILESIZE / 2) <= _sXY.x)
                {
                    if (IntersectRect(&tRc, &_sRc, &_vTile[tile[1]].rc) && _vTile[tile[1]].tState == MAP_UNMOVE)
                    {
                        _sXY.x += tRc.bottom - _sRc.top ;
                    }
                }
                //중앙에서 왼쪽이라면
                else if ((_vTile[center].mpt.x + TILESIZE / 2) > _sXY.x)
                {
                    if (IntersectRect(&tRc, &_sRc, &_vTile[tile[7]].rc) && _vTile[tile[7]].tState == MAP_UNMOVE)
                    {
                        _sXY.x += tRc.bottom - _sRc.top;
                    }
                }
                break;
            }
            case PlayerDir::B:
            {
                //왼짝
                RECT tRc;
                if (IntersectRect(&tRc, &_sRc, &_vTile[tile[4]].rc) && _vTile[tile[4]].tState == MAP_UNMOVE)
                {
                    _sXY.x -= _sRc.bottom - tRc.top;
                }
                //중앙에서 오른쪽이라면
                if ((_vTile[center].mpt.x + TILESIZE / 2) <= _sXY.x)
                {
                    if (IntersectRect(&tRc, &_sRc, &_vTile[tile[3]].rc) && _vTile[tile[3]].tState == MAP_UNMOVE)
                    {
                        _sXY.x -= _sRc.bottom - tRc.top;
                    }
                }
                //중앙에서 왼쪽이라면
                else if ((_vTile[center].mpt.x + TILESIZE / 2) > _sXY.x)
                {
                    if (IntersectRect(&tRc, &_sRc, &_vTile[tile[5]].rc) && _vTile[tile[5]].tState == MAP_UNMOVE)
                    {
                        _sXY.x -= _sRc.bottom - tRc.top;
                    }
                }
                break;
            }
        }
    }

    */

    //무식한 통짜인식
    for (int i = 0; i < _vMap.size(); ++i)
    {
        if (_pState == PlayerState::FALL) break;

        RECT tRc;
        //상태 충돌
        if (IntersectRect(&tRc, &_sRc, &_vMap[i].rc))
        {
            int width = tRc.right - tRc.left;
            int height = tRc.bottom - tRc.top;
            
            if (_vMap[i].oState == SET_POTAL && getDistance(_sXY.x,_sXY.y,(_vMap[i].rc.left + TILESIZE/2), (_vMap[i].rc.top + TILESIZE / 2)) < 15)
            {
                //if (width > 30 && height > 17)
                _pState = PlayerState::FALL;
                
                //데이터 저장
                dataSave();
               
                //프레임 초기화
                if (_frame.y == 0 || _frame.y == 1 || _frame.y == 2) _frame.y = 0;
                else if (_frame.y == 3 || _frame.y == 4 || _frame.y == 5)  _frame.y = 1;
                
                _frame.x = 0;

                //이미지 변경
                imageChange();
            }
            else if (_vMap[i].oState == SET_EVENT)
            {
                _isStart = true;
            }
        }

        //정상타일이라면 초기화
        if (_vMap[i].tState == MAP_MOVE) continue;

        //충돌체크
        if (IntersectRect(&tRc, &_sRc, &_vMap[i].rc))
        {
            int width = tRc.right - tRc.left;
            int height = tRc.bottom - tRc.top;
            
            //위나 아래에서 박았을 때
            if (width > height)
            {
                //위
                if (_sRc.top > _vMap[i].rc.top)
                {
                    _sXY.y += height;
                    _cMove.y += height;
                }
                //아래
                else if (_sRc.top < _vMap[i].rc.top)
                {
                    _sXY.y -= height;
                    _cMove.y -= height;
                }
            }
            //좌나 우에서 박았을 때
            else if (width < height)
            {
                //좌
                if (_sRc.left > _vMap[i].rc.left)
                {
                    _sXY.x += width;
                    _cMove.x += width;
                }
                //우
                else if (_sRc.left < _vMap[i].rc.left)
                {
                    _sXY.x -= width;
                    _cMove.x -= width;
                }
            }
        }
        //체공
        //fly = 50;
        //몸통의 중점
        _bXY.x = _sXY.x;
        _bXY.y = _sXY.y - _fly;
        //그림자와 몸통 위치 변경
        _sRc = RectMakeCenter(_sXY.x, _sXY.y, _sImg->getWidth(), _sImg->getHeight());
        _bRc = RectMakeCenter(_bXY.x, _bXY.y, _bImg->getFrameWidth(), _bImg->getFrameHeight());
        
    }
}

//타일셋 받아오기
void player::setTile0(vector<TILE> tile, int tilex, int tiley)
{
    //타일맵 받아오기
    _vMap = tile;

    //타일맵 XY 최대 크기 받아오기
    _tileMax = { tilex , tiley };
}

void player::addIsRoll()
{
    if (!_isRoll && _isControl)
    {
        dataSave();
        _isRoll = true;
        _pState = PlayerState::ROLL;
        imageChange();
    }
}

void player::dataSave()
{
    int p;
    if (_pType == PlayerType::MARINE) p = 0;
    else if (_pType == PlayerType::PRISONER) p = 1;

    //저장 및 씬 넘기기
    DSS->setPlayerData(_maxHeart, _heart, _barrior, _blankBullet, _key, _money);
    DSS->setPlayerType(p);
}