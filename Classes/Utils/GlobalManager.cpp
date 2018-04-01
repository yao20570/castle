#include "GlobalManager.h"
#include "UI/WorldScene/WorldScene.h"
#include "UI/HomeScene/HomeScene.h"
#include "UI/BattleScene/BattleScene.h"

GlobalManager* GlobalManager::_g = nullptr;
GlobalManager* GlobalManager::getInstance()
{
    if (nullptr == _g) {
        _g = new GlobalManager();
        _g->init();
    }
    return _g;
}

GlobalManager::GlobalManager()
{
    
}

GlobalManager::~GlobalManager()
{
    _g = nullptr;
}

bool GlobalManager::init()
{
    
    return true;
}


// �������
float GlobalManager::getDistance(Vec2 p1, Vec2 p2)
{
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

// ���Ƿ���Բ����
bool GlobalManager::isPointInCircle(Vec2 centerPoint, float radius, Vec2 p)
{
    float dis = getDistance(centerPoint, p);
    if (dis < radius) return true;
    else return false;
}


// ���
float GlobalManager::xmult(Vec2 p1, Vec2 p2, Vec2 p0)
{
    return (p1.x-p0.x)*(p2.y-p0.y) - (p2.x-p0.x)*(p1.y-p0.y);
}

// ���Ƿ���������
bool GlobalManager::isPointInDiamond(Vec2 centerPoint, Size size, Vec2 p)
{
    Vec2 p1 = centerPoint + Vec2(0, size.height/2);
    Vec2 p2 = centerPoint + Vec2(size.width/2, 0);
    Vec2 p3 = centerPoint + Vec2(0, -size.height/2);
    Vec2 p4 = centerPoint + Vec2(-size.width/2, 0);
    bool a = xmult(p, p2, p1) > 0;
    bool b = xmult(p, p3, p2) > 0;
    bool c = xmult(p, p4, p3) > 0;
    bool d = xmult(p, p1, p4) > 0;
    return (a == b) && (a == c) && (a == d);
}

// ���Ƿ��ھ�����
bool GlobalManager::isPointInRect(Vec2 centerPoint, Size size, Vec2 p)
{
    float leftX = centerPoint.x - size.width/2;
    float rightX = centerPoint.x + size.width/2;
    float upY = centerPoint.y + size.width/2;
    float downY = centerPoint.y - size.width/2;
    if (p.x > leftX && p.x < rightX && p.y > downY && p.y < upY) return true;
    else return false;
}

// ��ȡʱ���
int GlobalManager::getTimeStamp()
{
    timeval tm;
    gettimeofday(&tm, NULL);
    return tm.tv_sec;
}


// intתstring
string GlobalManager::getIntToStr(int value)
{
    if (value <= 0) return "0";
    string str = "";
    while (value) {
        str += value%10 + '0';
        value/=10;
    }
    reverse(str.begin(), str.end());
    return str;
}


// stringתint
int GlobalManager::getStrToInt(string value)
{
    int x = 0;
    for (int i = 0; i < value.length(); i++) {
        x = x*10 + (value[i] - '0');
    }
    return x;
}


// ��Ƭ���� ת ��ͼ����
Vec2 GlobalManager::getMapPos(Vec2 pos)
{
    float x = TILED_WIDTH/2.0 * (pos.y - pos.x) + MAP_X;
    float y = -1.0 * (TILED_HEIGHT/2.0) * (pos.y + pos.x) + MAP_Y;
    return Vec2(x, y);
}

// ��ͼ���� ת ��Ƭ����
Vec2 GlobalManager::getTiledPos(Vec2 pos)
{
    int x = (pos.x-MAP_X)/(TILED_WIDTH/2.0) ;
    int y = (-1.0*(pos.y - MAP_Y)/(TILED_HEIGHT/2.0)) ;
    int tx = (y-x)/2;
    int ty = (y+x)/2;
    return Vec2(tx, ty);
}

// ��ƬԽ��
bool GlobalManager::isOutMap(Vec2 pos)
{
    Vec2 tiledPos = getTiledPos(pos);
    
    if (tiledPos.x < -EPS || tiledPos.x > TILED_TOTAL_X
        || tiledPos.y < -EPS || tiledPos.y > TILED_TOTAL_Y) {
        return true;
    }
    
    return false;
}

// ռ��/�ͷŵ��棺delta=1ռ�ݣ�=-1�ͷ�
void GlobalManager::setCoverd(Vec2 pos, int delta)
{
    for (int i = 0; i < 9; i++) {
        int x = pos.x + DX[i];
        int y = pos.y + DY[i];
        if (x < 0 || x > TILED_TOTAL_X || y < 0 || y > TILED_TOTAL_Y) continue;
        _cover[x][y] += delta;
    }
}

// �����Ƿ�ռ��
bool GlobalManager::isCovered(Vec2 pos)
{
    int x = pos.x;
    int y = pos.y;
    if (x < 0 || x > TILED_TOTAL_X || y < 0 || y > TILED_TOTAL_Y) return true;
    return _cover[x][y] != 0;
}

// ���ռ�����
void GlobalManager::clearCovered()
{
    // ��ʼ��ռ�����
    for (int i = 0; i <= TILED_TOTAL_X; i++) {
        for (int j = 0; j <= TILED_TOTAL_Y; j++) {
            _cover[i][j] = 0;
        }
    }
}

// ��ȡ�յ�λ��
Vec2 GlobalManager::getSpaceTiled()
{
    queue<int>q;
    bool vis[TILED_TOTAL_X+1][TILED_TOTAL_Y+1];
    memset(vis, false, sizeof(vis));

    q.push(19); q.push(19);
    vis[19][19] = true;
    while (!q.empty()) {
        int x = q.front(); q.pop();
        int y = q.front(); q.pop();
        if (_cover[x][y] == 0) return Vec2(x, y);
        for (int i = 0; i < 9; i++) {
            if (DX[i] == 0 && DY[i] == 0) continue;
            int xx = x + DX[i];
            int yy = y + DY[i];
            if (xx < 0 || xx > TILED_TOTAL_X || yy < 0 || yy > TILED_TOTAL_Y) continue;
            if (vis[xx][yy]) continue;
            vis[xx][yy] = true;
            q.push(xx); q.push(yy);
        }
    }
    return Vec2(19, 19);
}

// �����ȡ��ǰλ����Χ�Ŀյ�
ValueVector GlobalManager::getNextSpace(Vec2 pos)
{
    ValueVector vv;
    for (int i = 0; i < 9; i++) {
        if (DX[i] == 0 && DY[i] == 0) continue;
        int xx = pos.x + DX[i];
        int yy = pos.y + DY[i];
        if (xx < 0 || xx > TILED_TOTAL_X || yy < 0 || yy > TILED_TOTAL_Y) continue;
        if (_cover[xx][yy]) continue;
        
        ValueMap map;
        map["x"] = xx; map["y"] = yy; map["dir"] = i;
        vv.push_back((Value)map);
    }
    return vv;
}

// ������Ƭ�����ƫ��������ȡ��ͼ�����ƫ����
Vec2 GlobalManager::getMapDelta(int dir)
{
    switch (dir) {
        case 0: return Vec2(0, -TILED_HEIGHT);
            break;
        case 1: return Vec2(-TILED_WIDTH/2, -TILED_HEIGHT/2);
            break;
        case 2: return Vec2(-TILED_WIDTH, 0);
            break;
        case 3: return Vec2(-TILED_WIDTH/2, TILED_HEIGHT/2);
            break;
        case 4: return Vec2(0, TILED_HEIGHT);
            break;
        case 5: return Vec2(TILED_WIDTH/2, TILED_HEIGHT/2);
            break;
        case 6: return Vec2(TILED_WIDTH, 0);
            break;
        case 7: return Vec2(TILED_WIDTH/2, -TILED_HEIGHT/2);
            break;
        default:
            break;
    }
    return Vec2(0, 0);
}


// ͨ���������򣬻�ȡ����
int GlobalManager::getDir(Vec2 delta)
{
    int x = (int)delta.x;
    int y = (int)delta.y;
    if (x == 0 && y == 1) return 0;
    if (x == 1 && y == 1) return 1;
    if (x == 1 && y == 0) return 2;
    if (x == 1 && y == -1) return 3;
    if (x == 0 && y == -1) return 4;
    if (x == -1 && y == -1) return 5;
    if (x == -1 && y == 0) return 6;
    if (x == -1 && y == 1) return 7;
    return 9;
}

// ͨ�����㣬��ȡ����
int GlobalManager::getDir(Vec2 src, Vec2 des)
{
    Vec2 p = src - des;
    float ang = atan2(p.y, p.x) * 180.0 / acos(-1.0);
	CCLOG("%f", ang);
       
//    printf("%f : ", ang);
    
    if (-67.5 <= ang && ang <= -22.5)   return 3;
    if (-22.5 <= ang && ang <= 22.5)    return 2;
    if (22.5 <= ang && ang <= 67.5)     return 1;
    if (67.5 <= ang && ang <= 112.5)    return 0;
    if (112.5 <= ang && ang <= 157.5)   return 7;
    if (-112.5 <= ang && ang <= -67.5)  return 4;
    if (-157.5 <= ang && ang <= -112.5) return 5;
    return 6;
}


// ��ȡ������ͼƬ��
string GlobalManager::getBuildingIMG(int type)
{
    // BUILDING_TYPE_BaseTower    = 1;    // ˾�
    // BUILDING_TYPE_Raider       = 2;    // �״�
    // BUILDING_TYPE_HeroHotel    = 3;    // Ӣ���ù�
    // BUILDING_TYPE_Camp         = 4;    // ��Ӫ
    // BUILDING_TYPE_ResearchLab  = 5;    // �о���
    // BUILDING_TYPE_MineFactory  = 6;    // �ɿ�
    // BUILDING_TYPE_WoodFactory  = 7;    // ľ�ĳ�
    // BUILDING_TYPE_ArrowTower   = 8;    // ����
    // BUILDING_TYPE_Cannon       = 9;    // ����
    // BUILDING_TYPE_Laser        = 10;   // ������
    string img = "";
    switch (type) {
        case BUILDING_TYPE_BaseTower:
            img = IMG_BUILDING_BaseTower;
            break;
        case BUILDING_TYPE_Raider:
            img = IMG_BUILDING_Raider;
            break;
        case BUILDING_TYPE_HeroHotel:
            img = IMG_BUILDING_HeroHotel;
            break;
        case BUILDING_TYPE_Camp:
            img = IMG_BUILDING_Camp;
            break;
        case BUILDING_TYPE_ResearchLab:
            img = IMG_BUILDING_ResearchLab;
            break;
        case BUILDING_TYPE_MineFactory:
            img = IMG_BUILDING_MineFactory;
            break;
        case BUILDING_TYPE_WoodFactory:
            img = IMG_BUILDING_WoodFactory;
            break;
        case BUILDING_TYPE_ArrowTower:
            img = IMG_BUILDING_ArrowTower;
            break;
        case BUILDING_TYPE_Cannon:
            img = IMG_BUILDING_Cannon;
            break;
        case BUILDING_TYPE_Laser:
            img = IMG_BUILDING_Laser;
            break;
        default:
            break;
    }
    return img;
}

// ��ȡ�������ݻٺ��ͼƬ��
string GlobalManager::getBuildingBrokenIMG(int type)
{
    // BUILDING_TYPE_BaseTower    = 1;    // ˾�
    // BUILDING_TYPE_Raider       = 2;    // �״�
    // BUILDING_TYPE_HeroHotel    = 3;    // Ӣ���ù�
    // BUILDING_TYPE_Camp         = 4;    // ��Ӫ
    // BUILDING_TYPE_ResearchLab  = 5;    // �о���
    // BUILDING_TYPE_MineFactory  = 6;    // �ɿ�
    // BUILDING_TYPE_WoodFactory  = 7;    // ľ�ĳ�
    // BUILDING_TYPE_ArrowTower   = 8;    // ����
    // BUILDING_TYPE_Cannon       = 9;    // ����
    // BUILDING_TYPE_Laser        = 10;   // ������
    string img = "";
    switch (type) {
        case BUILDING_TYPE_BaseTower:
            img = IMG_BUILDING_BaseTower_Broken;
            break;
        case BUILDING_TYPE_Raider:
            img = IMG_BUILDING_Raider_Broken;
            break;
        case BUILDING_TYPE_HeroHotel:
            img = IMG_BUILDING_HeroHotel_Broken;
            break;
        case BUILDING_TYPE_Camp:
            img = IMG_BUILDING_Camp_Broken;
            break;
        case BUILDING_TYPE_ResearchLab:
            img = IMG_BUILDING_ResearchLab_Broken;
            break;
        case BUILDING_TYPE_MineFactory:
            img = IMG_BUILDING_MineFactory_Broken;
            break;
        case BUILDING_TYPE_WoodFactory:
            img = IMG_BUILDING_WoodFactory_Broken;
            break;
        case BUILDING_TYPE_ArrowTower:
            img = IMG_BUILDING_ArrowTower_Broken;
            break;
        case BUILDING_TYPE_Cannon:
            img = IMG_BUILDING_Cannon_Broken;
            break;
        case BUILDING_TYPE_Laser:
            img = IMG_BUILDING_Laser_Broken;
            break;
        default:
            break;
    }
    return img;
}


//--------------------------------------------------------------------------------
// ս�����
//--------------------------------------------------------------------------------

// ��ȡҪ�����ĳǳ�ID
int GlobalManager::getBattleTownID()
{
//    return _townID;
    return 1;
}

// �����پ���
int GlobalManager::getManhadun(Vec2 p1, Vec2 p2)
{
    int x = p1.x - p2.x;
    int y = p1.y - p2.y;
    if (x < 0) x = -x;
    if (y < 0) y = -y;
    return x + y;
}


//--------------------------------------------------------------------------------
// �����л�
//--------------------------------------------------------------------------------

// ����ս�Ե�ͼ
void GlobalManager::enterWorldScene()
{
    auto scene = WorldScene::createScene();
    auto tt = TransitionFade::create(0.5f, scene);
    Director::getInstance()->replaceScene(tt);
}


// ������ҳǳ�
void GlobalManager::enterHomeScene()
{
    auto scene = HomeScene::createScene();
    auto tt = TransitionFade::create(0.5f, scene);
    Director::getInstance()->replaceScene(tt);
}


// ����ؿ��ǳ�
void GlobalManager::enterChapterScene(int townID, int type)
{
    _townID = townID;
    if (type == CHAPTER_TYPE_SEEK) {
        auto scene = BattleScene::createScene();
        auto tt = TransitionFade::create(0.5f, scene);
        Director::getInstance()->replaceScene(tt);
    }
    else if(type == CHAPTER_TYPE_FIGHT) {
        auto scene = BattleScene::createScene();
        auto tt = TransitionFade::create(0.5f, scene);
        Director::getInstance()->replaceScene(tt);
    }
}

