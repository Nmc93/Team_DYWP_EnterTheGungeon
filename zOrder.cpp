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
    //�պ�����
    mergeSort(0, _vObj.size() - 1);

    for (int i = 0; i < _vObj.size(); ++i)
    {
        //�÷��̾�
        if (_vObj[i]->unitType == UnitType::PLAYER)
        {
            //�׸���
            _vObj[i]->sImage->render(getMemDC(), _vObj[i]->shadow.x - (_cMove.x + _cMouse.x), _vObj[i]->shadow.y - (_cMove.y + _cMouse.y));
            //����
            _vObj[i]->bImage->frameRender(getMemDC(), _vObj[i]->body.x - (_cMove.x + _cMouse.x), _vObj[i]->body.y - (_cMove.y + _cMouse.y), _vObj[i]->frame.x, _vObj[i]->frame.y);
        }
        //���ʹ�
        else if (_vObj[i]->unitType == UnitType::ENEMY)
        {
            if (!_vObj[i]->plusbool)
            {
                _vObj[i]->plusImg->frameRender(getMemDC(), (_vObj[i]->body.x - 15) - (_cMove.x + _cMouse.x), _vObj[i]->body.y - (_cMove.y + _cMouse.y), _vObj[i]->frame.x, _vObj[i]->frame.y);
            }
            else
            {
                //�׸���
                _vObj[i]->sImage->render(getMemDC(), _vObj[i]->shadow.x - (_cMove.x + _cMouse.x), _vObj[i]->shadow.y - (_cMove.y + _cMouse.y));
                //����
                _vObj[i]->bImage->frameRender(getMemDC(), _vObj[i]->body.x - (_cMove.x + _cMouse.x), _vObj[i]->body.y - (_cMove.y + _cMouse.y), _vObj[i]->frame.x, _vObj[i]->frame.y);
            }
        }
        //��
        else if (_vObj[i]->unitType == UnitType::GUN)
        {
            //����
            _vObj[i]->bImage->frameRender(getMemDC(), _vObj[i]->body.x - (_cMove.x + _cMouse.x), _vObj[i]->body.y - (_cMove.y + _cMouse.y), _vObj[i]->frame.x, _vObj[i]->frame.y);
        }
        //����
        else if (_vObj[i]->unitType == UnitType::BOSS)
        {
            //�׸���
            _vObj[i]->sImage->render(getMemDC(), _vObj[i]->shadow.x - (_cMove.x + _cMouse.x), _vObj[i]->shadow.y - (_cMove.y + _cMouse.y));
            //����
            _vObj[i]->plusImg->frameRender(getMemDC(), _vObj[i]->plusPointf.x - (_cMove.x + _cMouse.x), _vObj[i]->plusPointf.y - (_cMove.y + _cMouse.y), _vObj[i]->plusPoint.x, _vObj[i]->plusPoint.y);
            //����//_vObj[i]->plusPointf.x - (_cMove.x + _cMouse.x), _vObj[i]->plusPointf.y - (_cMove.y + _cMouse.y)
            _vObj[i]->bImage->frameRender(getMemDC(), _vObj[i]->body.x - (_cMove.x + _cMouse.x), _vObj[i]->body.y - (_cMove.y + _cMouse.y), _vObj[i]->frame.x, _vObj[i]->frame.y);
        }
        //�Ѿ�
        else if (_vObj[i]->unitType == UnitType::BULLET)
        {
            if (!_vObj[i]->plusbool) continue;
            //�׸���
            _vObj[i]->bImage->render(getMemDC(), _vObj[i]->body.x - (_cMove.x + _cMouse.x), _vObj[i]->body.y - (_cMove.y + _cMouse.y));
        }
        else if (_vObj[i]->unitType == UnitType::ITEM)
        {
            _vObj[i]->bImage->render(getMemDC(), _vObj[i]->body.x - (_cMove.x + _cMouse.x), _vObj[i]->body.y - (_cMove.y + _cMouse.y));
        }
    }

    //���� ���� �Լ�
    vClear();
}

void zOrder::drawImage(vector<Unit*>& vOrder)
{
	//mergeSort(0, vObj.size() - 1);
}

//����===============================================================================
void zOrder::merge(int left, int right)
{
    //������ �迭�� ���� ���͸� ���� �� �ʱ�ȭ
    vector<Unit*> test;
    test.clear();

    //�߰����� ���� �� �ʱ�ȭ
    int middle = (left + right) / 2;

    //ù��° �˻� �迭(left(��������)���� ����)
    int first = left;
    //�ι�° �˻� �迭(�߰� + 1 ���� ����)
    int second = middle + 1;
    //�迭�� ������ �� ������ ����
    int startNum = left;

    //ù �迭�� �ι�° �迭�� ���� ���� �ʾ�����
    while (first <= middle && second <= right)
    {
        //ù �迭�� �ι�° �迭���� �۴ٸ�
        if (_vObj[first]->key < _vObj[second]->key)
        {
            test.push_back(_vObj[first++]);
        }
        else if (_vObj[first]->key >= _vObj[second]->key)
        {
            test.push_back(_vObj[second++]);
        }
    }

    //ù �迭�� ���� ����ä ������ ��
    if (first > middle)
    {
        //�ι�° �迭�� ���� �ɶ�����
        for (int i = second; i <= right; i++)
        {
            test.push_back(_vObj[second++]);
        }
    }
    //�ι�° �迭�� ���� ����ä ������ ��
    else if (second > right)
    {
        //ù��° �迭�� ���� �ɶ�����
        for (int i = first; i <= middle; i++)
        {
            test.push_back(_vObj[first++]);
        }
    }

    //������ ���� �� ������ �迭�� ������ ����
    for (int i = 0; i < test.size(); i++)
    {
        _vObj[startNum++] = test[i];
    }
}

//�պ� ����
void zOrder::mergeSort(int left, int right)
{
    if (left < right)
    {
        //�߾�
        int mid = (left + right) / 2;

        //�׽�Ʈ�ؽ�Ʈ
        //cout << left << "," << right << endl;

        //�񱳴�� ����
        //���� �߰�
        mergeSort(left, mid);
        //�߰� + 1 ��
        mergeSort(mid + 1, right);
        //�պ� ����
        merge(left, right);
    }
}

//�÷��̾� Ǫ��
void zOrder::pushObject(player* player)
{
    Unit* zo = new Unit;
    //����
    zo->unitType = UnitType::PLAYER;
    //Ű��
    zo->key = player->getShadowXY().y;
    //�׸��� LT
    zo->shadow = { static_cast<float>(player->getShadowRc().left),static_cast<float>(player->getShadowRc().top) };
    //���� LT
    zo->body = { static_cast<float>(player->getBodyRc().left),static_cast<float>(player->getBodyRc().top) };
    //�̹��� ������
    zo->frame = player->getFrame();
    //�׸��� �̹���
    zo->sImage = player->getShadowImg();
    //���� �̹���
    zo->bImage = player->getBodyImg();
    //Z������ ����
    _vObj.push_back(zo);
}

//�� Ǫ��
void zOrder::pushObject(enemy* em)
{
    Unit* zo = new Unit;
    //����
    zo->unitType = UnitType::ENEMY;
    //Ű��
    zo->key = em->getShadow().y;
    //���� LT
    zo->body = { static_cast<float>(em->getBRect().left),static_cast<float>(em->getBRect().top) };
    //�׸��� LT
    zo->shadow = { static_cast<float>(em->getSRect().left) ,static_cast<float>(em->getSRect().top) };
    //�̹��� ������
    zo->frame = { em->getIdX(),em->getIdY() };
    //���� �̹���
    zo->bImage = em->getBImg();
    //�׸��� �̹���
    zo->sImage = em->getSImg();
    //��ȯ �̹���
    zo->plusImg = em->getCImg();
    //��ȯ ����
    zo->plusbool = em->getAppear();
    //Z������ ����
    _vObj.push_back(zo);
}

//���� Ǫ��
void zOrder::pushObject(boss* boss)
{
    Unit* zo = new Unit;
    //����
    zo->unitType = UnitType::BOSS;
    //Ű��
    zo->key = boss->getShadowY();
    //�׸��� LT
    zo->shadow = { static_cast<float>(boss->getShadowRc().left),static_cast<float>(boss->getShadowRc().top) };
    //���� LT
    zo->body = { static_cast<float>(boss->getBodyRc().left),static_cast<float>(boss->getBodyRc().top) };
    //�̹��� ������
    zo->frame = { boss->getBFrame(), 0 };
    //�׸��� �̹���
    zo->sImage = boss->getShadowImg();
    //���� �̹���
    zo->bImage = boss->getBodyImg();
    //����
    zo->plusImg = boss->getChairImg();
    zo->plusPoint = { boss->getCFrame() , 0 };
    zo->plusPointf = { static_cast<float>(boss->getChairRc().left),static_cast<float>(boss->getChairRc().top) };

    //Z������ ����
    _vObj.push_back(zo);
}

//�� Ǫ��
void zOrder::pushObject(playerGun* gun)
{
    Unit* zo = new Unit;
    //����
    zo->unitType = UnitType::GUN;
    //Ű��
    zo->key = gun->getKey();
    //���� LT
    zo->body = { static_cast<float>(gun->getGRc().left),static_cast<float>(gun->getGRc().top) };
    //�̹��� ������
    zo->frame = gun->getFrame();
    //���� �̹���
    zo->bImage = gun->getImage();
    //Z������ ����
    _vObj.push_back(zo);
}

//�Ѿ� Ǫ��
void zOrder::pushObject(tagBullet bullet)
{
    Unit* zo = new Unit;
    //����
    zo->unitType = UnitType::BULLET;
    //Ű��
    zo->key = bullet.y + 20;
    //���� LT
    zo->body = { static_cast<float>(bullet.rc.left),static_cast<float>(bullet.rc.top) };
    //�̹��� ������
    //zo->frame = bullet.;
    //���� �̹���
    zo->bImage = IMAGEMANAGER->findImage("BULLET");
    zo->plusbool = bullet.isFire;
    //Z������ ����
    _vObj.push_back(zo);
}

void zOrder::pushObject(item* item)
{
    Unit* zo = new Unit;
    //����
    zo->unitType = UnitType::BULLET;
    //Ű��
    zo->key = item->getY();
    //���� LT
    zo->body = { static_cast<float>(item->getRect().left),static_cast<float>(item->getRect().top) };
    //�̹��� ������
    //zo->frame = bullet.;
    //���� �̹���
    zo->bImage = IMAGEMANAGER->findImage("DROPTAN");
    //Z������ ����
    _vObj.push_back(zo);
}
