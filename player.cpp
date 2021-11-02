#include "pch.h"
#include "player.h"

HRESULT player::init(POINTF xy, bool active, PlayerType playertype, PlayerGunType armtype)
{
    //����ź
    _windImg = IMAGEMANAGER->findImage("WIND");
    //�׸���
    _sImg = IMAGEMANAGER->findImage("SHADOW");

    //�÷��̾� Ÿ�� ����
    _pType = playertype;
    _armType = armtype;

    //�÷��̾� Ÿ�Կ� ���� �̹��� ����
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

    //�׸���(������ǥ,��Ʈ)
    _sXY.x = xy.x;
    _sXY.y = xy.y;
    _sRc = RectMakeCenter(_sXY.x, _sXY.y, _sImg->getWidth(), _sImg->getHeight());

    //�÷��̾�(�����,xy��ǥ,��Ʈ)
    _fly = 30;
    _bXY.x = _sXY.x;
    _bXY.y = _sXY.y - _fly;
    _bRc = RectMakeCenter(_bXY.x, _bXY.y, _bImg->getFrameWidth(), _bImg->getFrameHeight());

    //�÷��̾� ����(�⺻ IDLE, �⺻ �߾�)
    _pState = PlayerState::IDLE;
    _pDir = PlayerDir::CENTER;

    _frame = { 0,0 };

    //�÷��̾��� ���ۿ���
    _isControl = active;

    //ī�޶� ������
    _cMove.x = _sXY.x - MIDDLEX;
    _cMove.y = _sXY.y - MIDDLEY;

    //���İ�
    _alpha = 255;
    
    // UI �Ҹ�ǰ
    _heart = 6;
    _maxHeart = 6;
    _blankBullet = 2;
    _barrior = 0;
    _key = 0;
    _money = 0;

    _windWidth = 50;
    _windHeight = 50;

    //�� Ȱ��ȭ
    _isStart = false;

	return S_OK;
}

HRESULT player::init(POINTF xy, bool active, PlayerGunType armtype)
{
    //����ź
    _windImg = IMAGEMANAGER->findImage("WIND");
    //�׸���
    _sImg = IMAGEMANAGER->findImage("SHADOW");

    //�÷��̾� Ÿ�� ����
    if (DSS->getPlayerType() == 0)
    {
        _pType = PlayerType::MARINE;
    }
    else if (DSS->getPlayerType() == 1)
    {
        _pType = PlayerType::PRISONER;
    }

    _armType = armtype;

    //�÷��̾� Ÿ�Կ� ���� �̹��� ����
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

    //�׸���(������ǥ,��Ʈ)
    _sXY.x = xy.x;
    _sXY.y = xy.y;
    _sRc = RectMakeCenter(_sXY.x, _sXY.y, _sImg->getWidth(), _sImg->getHeight());

    //�÷��̾�(�����,xy��ǥ,��Ʈ)
    _fly = 30;
    _bXY.x = _sXY.x;
    _bXY.y = _sXY.y - _fly;
    _bRc = RectMakeCenter(_bXY.x, _bXY.y, _bImg->getFrameWidth(), _bImg->getFrameHeight());

    //�÷��̾� ����(�⺻ IDLE, �⺻ �߾�)
    _pState = PlayerState::IDLE;
    _pDir = PlayerDir::CENTER;

    _frame = { 0,0 };

    //�÷��̾��� ���ۿ���
    _isControl = active;

    //ī�޶� ������
    _cMove.x = _sXY.x - MIDDLEX;
    _cMove.y = _sXY.y - MIDDLEY;

    //���İ�
    _alpha = 255;

    // UI �Ҹ�ǰ
    _heart = DSS->getHp();
    _maxHeart = DSS->getMaxHp();
    _blankBullet = DSS->getBlank();
    _barrior = DSS->getBarrior();
    _key = DSS->getKey();
    _money = DSS->getMoney();

    _windWidth = 50;
    _windHeight = 50;

    //�� Ȱ��ȭ
    _isStart = false;
    return S_OK;
}

void player::release()
{
}

void player::update()
{
    //===============================================================================
    //�÷��̾� �̵�===================================================================
    //===============================================================================

    //�÷��̾� ���
    if (_heart <= 0)
    {
        _pState = PlayerState::DEAD;
        imageChange();
        _isRoll = true;
    }
    dataSave();
    //���콺 �̵�
    mouseMove();

    //������ ����
    frameUpdate();

    //������ ����
    _bXY.x = _sXY.x;
    _bXY.y = _sXY.y - _fly;

    //�׸��ڿ� ���� ��ġ ����
    _sRc = RectMakeCenter(_sXY.x, _sXY.y, _sImg->getWidth(), _sImg->getHeight());
    _bRc = RectMakeCenter(_bXY.x, _bXY.y, _bImg->getFrameWidth(), _bImg->getFrameHeight());

    /*
    if (_selectPlayer)
    {
        _normalGun->setBullet(_bullet);
        _normalGun->setCamera(_cam);
        _shotGun->setBullet(_bullet);
        _shotGun->setCamera(_cam);

        // �� ����
        if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON)) _gun->fire(BTYPE::PLAYER);
        
        if (KEYMANAGER->isOnceKeyDown(VK_CONTROL))
        {
            if      (_gun == _shotGun)   _gun = _normalGun;
            else if (_gun == _normalGun) _gun = _shotGun;
        }

        // ���� �߰�
        if (KEYMANAGER->isOnceKeyDown('N')) _key++;

        //��Ʈ ���ο� �� �߰�
        /*if (KEYMANAGER->isOnceKeyDown('M'))
        {
            _itemManager->addHeart();
            _heart += 2;
            _maxHeart += 2;
        }

        //��Ʈ ��ĭ ����
        if (KEYMANAGER->isOnceKeyDown(VK_F7))
        {
            _itemManager->changeHeart(false);
            if (_heart > 0) _heart--;
        }

        //��Ʈ ��ĭ �߰�
        if (KEYMANAGER->isOnceKeyDown(VK_F8))
        {
            _itemManager->changeHeart(true);
            if (_heart < _maxHeart) _heart++;
        }

        //�� ����
        if (KEYMANAGER->isOnceKeyDown(VK_F9)) _itemManager->changeBarrior(false);

        //�� �߰�
        if (KEYMANAGER->isOnceKeyDown(VK_F10)) _itemManager->changeBarrior(true);

        //����ź ��
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

        // ������ ����
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
    sprintf_s(str, "�÷��̾� ����");
    TextOut(getMemDC(), 10, 300, str, strlen(str));
    if (_pType == PlayerType::MARINE)
    {
        sprintf_s(str, "Type         : �غ�");
        TextOut(getMemDC(), 10, 316, str, strlen(str));
    }
    else if (_pType == PlayerType::PRISONER)
    {
        sprintf_s(str, "Type         : �˼�");
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

//�÷��̾� �̵�
void player::playerMove(PlayerDir pd)
{
    if (!_isControl) return;
    if(_pState == PlayerState::FALL || _pState == PlayerState::DEAD)return;
    //_cMove = { 0,0 };

    //����
    switch (pd)
    {
        //�������� ����
        case PlayerDir::CENTER:
        {
            if (_pState != PlayerState::ROLL)
            {
                _pState = PlayerState::IDLE;
            }
            imageChange();
            break;
        }
        //����
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
        //������
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
        //��
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
        //��������
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
        //������
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
        //������ �Ʒ�
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
        //�Ʒ�
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
        //���� �Ʒ�
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
    //�׸��ڿ� ���� ��ġ ����
    _sRc = RectMakeCenter(_sXY.x, _sXY.y, _sImg->getWidth(), _sImg->getHeight());
    _bRc = RectMakeCenter(_bXY.x, _bXY.y, _bImg->getFrameWidth(), _bImg->getFrameHeight());

    //�浹üũ,
    playerTile(pd);
}

//������ ������Ʈ
void player::frameUpdate()
{
    //������ ī��Ʈ
    int maxFrame = 10;

    if (_pState == PlayerState::ROLL)
    {
        maxFrame = 5;
    }

    _frameCount++;

    //������ ��
    if (_frameCount % maxFrame == 0)
    {
        if(_pState == PlayerState::FALL)
        {
            //���� ���� ������
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
                    //������ ���� �� ���� ����
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
                    //������ ���� �� ���� ����
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

//�̹��� ����
void player::imageChange()
{
    //�غ�����
    if (_pType == PlayerType::MARINE)
    {
        //���
        if (_armType == PlayerGunType::NORMAL)
        {
            //�⺻ ����
            if (_pState == PlayerState::IDLE)
            {
                _bImg = IMAGEMANAGER->findImage("HELMET_IDLE");
            }
            //�ȴ� ����
            else if (_pState == PlayerState::WALK)
            {
                _bImg = IMAGEMANAGER->findImage("HELMET_WALK");
            }
            //������
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
        //�Ѽ�
        else if (_armType == PlayerGunType::ONEHAND)
        {
            //�⺻ ����
            if (_pState == PlayerState::IDLE)
            {
                _bImg = IMAGEMANAGER->findImage("HELMET_1H_IDLE");
            }
            //�ȴ� ����
            else if (_pState == PlayerState::WALK)
            {
                _bImg = IMAGEMANAGER->findImage("HELMET_1H_WALK");
            }
            //������
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
        //�ո���
        else if (_armType == PlayerGunType::TWOHAND)
        {
            //�⺻ ����
            if (_pState == PlayerState::IDLE)
            {
                _bImg = IMAGEMANAGER->findImage("HELMET_2H_IDLE");
            }
            //�ȴ� ����
            else if (_pState == PlayerState::WALK)
            {
                _bImg = IMAGEMANAGER->findImage("HELMET_2H_WALK");
            }
            //������
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
    //�˼�
    else if (_pType == PlayerType::PRISONER)
    {
        //���
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
        //�Ѽ�
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
        //�ո���
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

void player::mouseMove() //�ﰢ�Լ� �̿��ؼ� �÷��̾� x,y�� ���콺 ptmouse ���� ����ؼ� ���� ����
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

    // Ȯ�ο�
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
        _alphaCount++;  // �󸶳� �����Ÿ���

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

//�浹üũ
void player::playerTile(PlayerDir pd)
{
    //�� ���
    /*
    int left = _sRc.left;
    int right = _sRc.right;
    int top = _sRc.top;
    int bottom = _sRc.bottom;

    int tileSize = tileWidth * tileHeight;

    for (int i = 0; i < tileSize; i++)
    {
        // �浹
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

    //�����ν�
    /*
    //�÷����̾��� �߹� Ÿ��
    int center = (_sXY.y * _tileMax.x + _sXY.x);
 
    int tile[8];
    tile[0] = center - _tileMax.x;      //12�� ����
    tile[1] = center - _tileMax.x + 1;  //2�� ����
    tile[2] = center + 1;               //3�� ����
    tile[3] = center + _tileMax.x + 1;  //5�� ����
    tile[4] = center + _tileMax.x;      //6�� ����
    tile[5] = center + _tileMax.x - 1;  //8�� ����
    tile[6] = center - 1;               //9�� ����
    tile[7] = center - _tileMax.x - 1;  //10�� ����

    //2Ű ��ȣ�� ���
    if (pd == PlayerDir::LB || pd == PlayerDir::LT ||
        pd == PlayerDir::RB || pd == PlayerDir::RT)
    {
        switch (pd)
        {
            case PlayerDir::LT:
            {
                //����
                RECT tRc;
                if (IntersectRect(&tRc, &_sRc, &_vTile[tile[0]].rc) && _vTile[tile[0]].tState == MAP_UNMOVE)
                {
                    _sXY.y += tRc.bottom - _sRc.top; // MOVESPPED
                }
                //��¦
                if (IntersectRect(&tRc, &_sRc, &_vTile[tile[6]].rc) && _vTile[tile[6]].tState == MAP_UNMOVE)
                {
                    _sXY.x += tRc.right - _sRc.left;
                }

                //�߾ӿ��� �������̶��
                if ((_vTile[center].mpt.x + TILESIZE / 2) <= _sXY.x)
                {
                    if (IntersectRect(&tRc, &_sRc, &_vTile[tile[1]].rc) && _vTile[tile[1]].tState == MAP_UNMOVE)
                    {
                        _sXY.y += tRc.bottom - _sRc.top; // MOVESPPED;
                    }
                }
                //�߾ӿ��� �����̶��
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
                //����
                RECT tRc;
                if (IntersectRect(&tRc, &_sRc, &_vTile[tile[0]].rc) && _vTile[tile[0]].tState == MAP_UNMOVE)
                {
                    _sXY.y += tRc.bottom - _sRc.top;
                }
                //����¦
                if (IntersectRect(&tRc, &_sRc, &_vTile[tile[2]].rc) && _vTile[tile[2]].tState == MAP_UNMOVE)
                {
                    _sXY.x -= _sRc.left - tRc.right;
                }

                //�߾ӿ��� �������̶��
                if ((_vTile[center].mpt.x + TILESIZE / 2) <= _sXY.x)
                {
                    //������ ��
                    if (IntersectRect(&tRc, &_sRc, &_vTile[tile[1]].rc) && _vTile[tile[1]].tState == MAP_UNMOVE)
                    {
                        _sXY.y += tRc.bottom - _sRc.top;
                    }
                }
                //�߾ӿ��� �����̶��
                else if ((_vTile[center].mpt.x + TILESIZE / 2) > _sXY.x)
                {
                    //���� ��
                    if (IntersectRect(&tRc, &_sRc, &_vTile[tile[7]].rc) && _vTile[tile[7]].tState == MAP_UNMOVE)
                    {
                        _sXY.y += tRc.bottom - _sRc.top;
                    }
                }
                break;
            }
            case PlayerDir::LB:
            {
                //�Ʒ���
                RECT tRc;
                if (IntersectRect(&tRc, &_sRc, &_vTile[tile[4]].rc) && _vTile[tile[4]].tState == MAP_UNMOVE)
                {
                    _sXY.y -= _sRc.bottom - tRc.top;
                }
                //����
                if (IntersectRect(&tRc, &_sRc, &_vTile[tile[6]].rc) && _vTile[tile[6]].tState == MAP_UNMOVE)
                {
                    _sXY.x += tRc.right - _sRc.left;
                }
                //�߾ӿ��� �������̶��
                if ((_vTile[center].mpt.x + TILESIZE / 2) <= _sXY.x)
                {
                    if (IntersectRect(&tRc, &_sRc, &_vTile[tile[3]].rc) && _vTile[tile[3]].tState == MAP_UNMOVE)
                    {
                        _sXY.y -= _sRc.bottom - tRc.top;
                    }
                }
                //�߾ӿ��� �����̶��
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
                //�Ʒ���
                RECT tRc;
                if (IntersectRect(&tRc, &_sRc, &_vTile[tile[4]].rc) && _vTile[tile[4]].tState == MAP_UNMOVE)
                {
                    _sXY.y -= _sRc.bottom - tRc.top;
                }
                //������
                if (IntersectRect(&tRc, &_sRc, &_vTile[tile[2]].rc) && _vTile[tile[2]].tState == MAP_UNMOVE)
                {
                    _sXY.x -= tRc.right - _sRc.left;
                }
                //�߾ӿ��� �������̶��
                if ((_vTile[center].mpt.x + TILESIZE / 2) <= _sXY.x)
                {
                    if (IntersectRect(&tRc, &_sRc, &_vTile[tile[3]].rc) && _vTile[tile[3]].tState == MAP_UNMOVE)
                    {
                        _sXY.y -= _sRc.bottom - tRc.top;
                    }
                }
                //�߾ӿ��� �����̶��
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
    //2Ű ��ȣ�� �ƴ� ���(1Ű ��ȣ)
    else
    {
        switch (pd)
        {
            case PlayerDir::L:
            {
                //��¦
                RECT tRc;
                if (IntersectRect(&tRc, &_sRc, &_vTile[tile[6]].rc) && _vTile[tile[6]].tState == MAP_UNMOVE)
                {
                    _sXY.x += tRc.left - _sRc.right;
                }
                //�߾ӿ��� �������̶��
                if ((_vTile[center].mpt.y + TILESIZE / 2) <= _sXY.y)
                {
                    if (IntersectRect(&tRc, &_sRc, &_vTile[tile[5]].rc) && _vTile[tile[5]].tState == MAP_UNMOVE)
                    {
                        _sXY.x += tRc.left - _sRc.right;
                    }
                }
                //�߾ӿ��� �����̶��
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
                //��¦
                RECT tRc;
                if (IntersectRect(&tRc, &_sRc, &_vTile[tile[2]].rc) && _vTile[tile[2]].tState == MAP_UNMOVE)
                {
                    _sXY.x -= _sRc.left - tRc.right;
                }
                //�߾ӿ��� �������̶��
                if ((_vTile[center].mpt.y + TILESIZE / 2) <= _sXY.y)
                {
                    if (IntersectRect(&tRc, &_sRc, &_vTile[tile[3]].rc) && _vTile[tile[3]].tState == MAP_UNMOVE)
                    {
                        _sXY.x -= _sRc.left - tRc.right;
                    }
                }
                //�߾ӿ��� �����̶��
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
                //��¦
                RECT tRc;
                if (IntersectRect(&tRc, &_sRc, &_vTile[tile[0]].rc) && _vTile[tile[0]].tState == MAP_UNMOVE)
                {
                    _sXY.x += tRc.bottom - _sRc.top;
                }
                //�߾ӿ��� �������̶��
                if ((_vTile[center].mpt.x + TILESIZE / 2) <= _sXY.x)
                {
                    if (IntersectRect(&tRc, &_sRc, &_vTile[tile[1]].rc) && _vTile[tile[1]].tState == MAP_UNMOVE)
                    {
                        _sXY.x += tRc.bottom - _sRc.top ;
                    }
                }
                //�߾ӿ��� �����̶��
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
                //��¦
                RECT tRc;
                if (IntersectRect(&tRc, &_sRc, &_vTile[tile[4]].rc) && _vTile[tile[4]].tState == MAP_UNMOVE)
                {
                    _sXY.x -= _sRc.bottom - tRc.top;
                }
                //�߾ӿ��� �������̶��
                if ((_vTile[center].mpt.x + TILESIZE / 2) <= _sXY.x)
                {
                    if (IntersectRect(&tRc, &_sRc, &_vTile[tile[3]].rc) && _vTile[tile[3]].tState == MAP_UNMOVE)
                    {
                        _sXY.x -= _sRc.bottom - tRc.top;
                    }
                }
                //�߾ӿ��� �����̶��
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

    //������ ��¥�ν�
    for (int i = 0; i < _vMap.size(); ++i)
    {
        if (_pState == PlayerState::FALL) break;

        RECT tRc;
        //���� �浹
        if (IntersectRect(&tRc, &_sRc, &_vMap[i].rc))
        {
            int width = tRc.right - tRc.left;
            int height = tRc.bottom - tRc.top;
            
            if (_vMap[i].oState == SET_POTAL && getDistance(_sXY.x,_sXY.y,(_vMap[i].rc.left + TILESIZE/2), (_vMap[i].rc.top + TILESIZE / 2)) < 15)
            {
                //if (width > 30 && height > 17)
                _pState = PlayerState::FALL;
                
                //������ ����
                dataSave();
               
                //������ �ʱ�ȭ
                if (_frame.y == 0 || _frame.y == 1 || _frame.y == 2) _frame.y = 0;
                else if (_frame.y == 3 || _frame.y == 4 || _frame.y == 5)  _frame.y = 1;
                
                _frame.x = 0;

                //�̹��� ����
                imageChange();
            }
            else if (_vMap[i].oState == SET_EVENT)
            {
                _isStart = true;
            }
        }

        //����Ÿ���̶�� �ʱ�ȭ
        if (_vMap[i].tState == MAP_MOVE) continue;

        //�浹üũ
        if (IntersectRect(&tRc, &_sRc, &_vMap[i].rc))
        {
            int width = tRc.right - tRc.left;
            int height = tRc.bottom - tRc.top;
            
            //���� �Ʒ����� �ھ��� ��
            if (width > height)
            {
                //��
                if (_sRc.top > _vMap[i].rc.top)
                {
                    _sXY.y += height;
                    _cMove.y += height;
                }
                //�Ʒ�
                else if (_sRc.top < _vMap[i].rc.top)
                {
                    _sXY.y -= height;
                    _cMove.y -= height;
                }
            }
            //�³� �쿡�� �ھ��� ��
            else if (width < height)
            {
                //��
                if (_sRc.left > _vMap[i].rc.left)
                {
                    _sXY.x += width;
                    _cMove.x += width;
                }
                //��
                else if (_sRc.left < _vMap[i].rc.left)
                {
                    _sXY.x -= width;
                    _cMove.x -= width;
                }
            }
        }
        //ü��
        //fly = 50;
        //������ ����
        _bXY.x = _sXY.x;
        _bXY.y = _sXY.y - _fly;
        //�׸��ڿ� ���� ��ġ ����
        _sRc = RectMakeCenter(_sXY.x, _sXY.y, _sImg->getWidth(), _sImg->getHeight());
        _bRc = RectMakeCenter(_bXY.x, _bXY.y, _bImg->getFrameWidth(), _bImg->getFrameHeight());
        
    }
}

//Ÿ�ϼ� �޾ƿ���
void player::setTile0(vector<TILE> tile, int tilex, int tiley)
{
    //Ÿ�ϸ� �޾ƿ���
    _vMap = tile;

    //Ÿ�ϸ� XY �ִ� ũ�� �޾ƿ���
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

    //���� �� �� �ѱ��
    DSS->setPlayerData(_maxHeart, _heart, _barrior, _blankBullet, _key, _money);
    DSS->setPlayerType(p);
}