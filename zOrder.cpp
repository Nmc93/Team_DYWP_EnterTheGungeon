#include "pch.h"
#include "zOrder.h"


HRESULT zOrder::init()
{
	return S_OK;
    _cMove = { 0,0 };
    _cMouse = { 0,0 };
}

void zOrder::release()
{
}

void zOrder::update()
{
}

void zOrder::render()
{
    //합병정렬
    mergeSort(0, _vObj.size() - 1);

    for (int i = 0; i < _vObj.size(); ++i)
    {
        //플레이어
        if (_vObj[i]->unitType == UnitType::PLAYER)
        {
            //그림자
            _vObj[i]->sImage->render(getMemDC(), _vObj[i]->shadow.x - (_cMove.x + _cMouse.x), _vObj[i]->shadow.y - (_cMove.y + _cMouse.y));
            //몸통
            _vObj[i]->bImage->frameRender(getMemDC(), _vObj[i]->body.x - (_cMove.x + _cMouse.x), _vObj[i]->body.y - (_cMove.y + _cMouse.y), _vObj[i]->frame.x, _vObj[i]->frame.y);
        }
        //에너미
        else if (_vObj[i]->unitType == UnitType::ENEMY)
        {
            if (!_vObj[i]->plusbool)
            {
                _vObj[i]->plusImg->frameRender(getMemDC(), (_vObj[i]->body.x - 15) - (_cMove.x + _cMouse.x), _vObj[i]->body.y - (_cMove.y + _cMouse.y), _vObj[i]->frame.x, _vObj[i]->frame.y);
            }
            else
            {
                //그림자
                _vObj[i]->sImage->render(getMemDC(), _vObj[i]->shadow.x - (_cMove.x + _cMouse.x), _vObj[i]->shadow.y - (_cMove.y + _cMouse.y));
                //몸통
                _vObj[i]->bImage->frameRender(getMemDC(), _vObj[i]->body.x - (_cMove.x + _cMouse.x), _vObj[i]->body.y - (_cMove.y + _cMouse.y), _vObj[i]->frame.x, _vObj[i]->frame.y);
            }
        }
        //총
        else if (_vObj[i]->unitType == UnitType::GUN)
        {
            //몸통
            _vObj[i]->bImage->frameRender(getMemDC(), _vObj[i]->body.x - (_cMove.x + _cMouse.x), _vObj[i]->body.y - (_cMove.y + _cMouse.y), _vObj[i]->frame.x, _vObj[i]->frame.y);
        }
        //보스
        else if (_vObj[i]->unitType == UnitType::BOSS)
        {
            //그림자
            _vObj[i]->sImage->render(getMemDC(), _vObj[i]->shadow.x - (_cMove.x + _cMouse.x), _vObj[i]->shadow.y - (_cMove.y + _cMouse.y));
            //의자
            _vObj[i]->plusImg->frameRender(getMemDC(), _vObj[i]->plusPointf.x - (_cMove.x + _cMouse.x), _vObj[i]->plusPointf.y - (_cMove.y + _cMouse.y), _vObj[i]->plusPoint.x, _vObj[i]->plusPoint.y);
            //몸통//_vObj[i]->plusPointf.x - (_cMove.x + _cMouse.x), _vObj[i]->plusPointf.y - (_cMove.y + _cMouse.y)
            _vObj[i]->bImage->frameRender(getMemDC(), _vObj[i]->body.x - (_cMove.x + _cMouse.x), _vObj[i]->body.y - (_cMove.y + _cMouse.y), _vObj[i]->frame.x, _vObj[i]->frame.y);
        }
        //총알
        else if (_vObj[i]->unitType == UnitType::BULLET)
        {
            if (!_vObj[i]->plusbool) continue;
            //그림자
            _vObj[i]->bImage->render(getMemDC(), _vObj[i]->body.x - (_cMove.x + _cMouse.x), _vObj[i]->body.y - (_cMove.y + _cMouse.y));
        }
        else if (_vObj[i]->unitType == UnitType::ITEM)
        {
            _vObj[i]->bImage->render(getMemDC(), _vObj[i]->body.x - (_cMove.x + _cMouse.x), _vObj[i]->body.y - (_cMove.y + _cMouse.y));
        }
    }

    //벡터 삭제 함수
    vClear();
}

void zOrder::drawImage(vector<Unit*>& vOrder)
{
	//mergeSort(0, vObj.size() - 1);
}

//정렬===============================================================================
void zOrder::merge(int left, int right)
{
    //정리한 배열을 담을 벡터를 선언 및 초기화
    vector<Unit*> test;
    test.clear();

    //중간지점 선언 및 초기화
    int middle = (left + right) / 2;

    //첫번째 검사 배열(left(시작지점)에서 시작)
    int first = left;
    //두번째 검사 배열(중간 + 1 에서 시작)
    int second = middle + 1;
    //배열이 정리된 후 삽입할 변수
    int startNum = left;

    //첫 배열과 두번째 배열이 끝에 닿지 않았을때
    while (first <= middle && second <= right)
    {
        //첫 배열이 두번째 배열보다 작다면
        if (_vObj[first]->key < _vObj[second]->key)
        {
            test.push_back(_vObj[first++]);
        }
        else if (_vObj[first]->key >= _vObj[second]->key)
        {
            test.push_back(_vObj[second++]);
        }
    }

    //첫 배열이 끝에 닿은채 끝났을 때
    if (first > middle)
    {
        //두번째 배열이 끝이 될때까지
        for (int i = second; i <= right; i++)
        {
            test.push_back(_vObj[second++]);
        }
    }
    //두번째 배열이 끝에 닿은채 끝났을 때
    else if (second > right)
    {
        //첫번째 배열이 끝이 될때까지
        for (int i = first; i <= middle; i++)
        {
            test.push_back(_vObj[first++]);
        }
    }

    //정리가 끝난 후 정리된 배열을 원본에 삽입
    for (int i = 0; i < test.size(); i++)
    {
        _vObj[startNum++] = test[i];
    }
}

//합병 정렬
void zOrder::mergeSort(int left, int right)
{
    if (left < right)
    {
        //중앙
        int mid = (left + right) / 2;

        //테스트텍스트
        //cout << left << "," << right << endl;

        //비교대상 분할
        //시작 중간
        mergeSort(left, mid);
        //중간 + 1 끝
        mergeSort(mid + 1, right);
        //합병 정렬
        merge(left, right);
    }
}

//플레이어 푸시
void zOrder::pushObject(player* player)
{
    Unit* zo = new Unit;
    //종류
    zo->unitType = UnitType::PLAYER;
    //키값
    zo->key = player->getShadowXY().y;
    //그림자 LT
    zo->shadow = { static_cast<float>(player->getShadowRc().left),static_cast<float>(player->getShadowRc().top) };
    //몸통 LT
    zo->body = { static_cast<float>(player->getBodyRc().left),static_cast<float>(player->getBodyRc().top) };
    //이미지 프레임
    zo->frame = player->getFrame();
    //그림자 이미지
    zo->sImage = player->getShadowImg();
    //몸통 이미지
    zo->bImage = player->getBodyImg();
    //Z오더용 벡터
    _vObj.push_back(zo);
}

//적 푸시
void zOrder::pushObject(enemy* em)
{
    Unit* zo = new Unit;
    //종류
    zo->unitType = UnitType::ENEMY;
    //키값
    zo->key = em->getShadow().y;
    //몸통 LT
    zo->body = { static_cast<float>(em->getBRect().left),static_cast<float>(em->getBRect().top) };
    //그림자 LT
    zo->shadow = { static_cast<float>(em->getSRect().left) ,static_cast<float>(em->getSRect().top) };
    //이미지 프레임
    zo->frame = { em->getIdX(),em->getIdY() };
    //몸통 이미지
    zo->bImage = em->getBImg();
    //그림자 이미지
    zo->sImage = em->getSImg();
    //소환 이미지
    zo->plusImg = em->getCImg();
    //소환 여부
    zo->plusbool = em->getAppear();
    //Z오더용 벡터
    _vObj.push_back(zo);
}

//보스 푸시
void zOrder::pushObject(boss* boss)
{
    Unit* zo = new Unit;
    //종류
    zo->unitType = UnitType::BOSS;
    //키값
    zo->key = boss->getShadowY();
    //그림자 LT
    zo->shadow = { static_cast<float>(boss->getShadowRc().left),static_cast<float>(boss->getShadowRc().top) };
    //몸통 LT
    zo->body = { static_cast<float>(boss->getBodyRc().left),static_cast<float>(boss->getBodyRc().top) };
    //이미지 프레임
    zo->frame = { boss->getBFrame(), 0 };
    //그림자 이미지
    zo->sImage = boss->getShadowImg();
    //몸통 이미지
    zo->bImage = boss->getBodyImg();
    //의자
    zo->plusImg = boss->getChairImg();
    zo->plusPoint = { boss->getCFrame() , 0 };
    zo->plusPointf = { static_cast<float>(boss->getChairRc().left),static_cast<float>(boss->getChairRc().top) };

    //Z오더용 벡터
    _vObj.push_back(zo);
}

//총 푸시
void zOrder::pushObject(playerGun* gun)
{
    Unit* zo = new Unit;
    //종류
    zo->unitType = UnitType::GUN;
    //키값
    zo->key = gun->getKey();
    //몸통 LT
    zo->body = { static_cast<float>(gun->getGRc().left),static_cast<float>(gun->getGRc().top) };
    //이미지 프레임
    zo->frame = gun->getFrame();
    //몸통 이미지
    zo->bImage = gun->getImage();
    //Z오더용 벡터
    _vObj.push_back(zo);
}

//총알 푸시
void zOrder::pushObject(tagBullet bullet)
{
    Unit* zo = new Unit;
    //종류
    zo->unitType = UnitType::BULLET;
    //키값
    zo->key = bullet.y + 20;
    //몸통 LT
    zo->body = { static_cast<float>(bullet.rc.left),static_cast<float>(bullet.rc.top) };
    //이미지 프레임
    //zo->frame = bullet.;
    //몸통 이미지
    zo->bImage = IMAGEMANAGER->findImage("BULLET");
    zo->plusbool = bullet.isFire;
    //Z오더용 벡터
    _vObj.push_back(zo);
}

void zOrder::pushObject(item* item)
{
    Unit* zo = new Unit;
    //종류
    zo->unitType = UnitType::BULLET;
    //키값
    zo->key = item->getY();
    //몸통 LT
    zo->body = { static_cast<float>(item->getRect().left),static_cast<float>(item->getRect().top) };
    //이미지 프레임
    //zo->frame = bullet.;
    //몸통 이미지
    zo->bImage = IMAGEMANAGER->findImage("DROPTAN");
    //Z오더용 벡터
    _vObj.push_back(zo);
}
