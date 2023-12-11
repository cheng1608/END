#include "enemyZombleFire.h"
#include"enemyFire.h"

EnemyZombleFire::EnemyZombleFire(Player *pl, QGraphicsScene *scene, QObject *parent)
    : EnemyBase(pl, scene, parent),attackCeil(50)
{
    // 加载动画
    m_movie = new QMovie(":/images/pic/ZombleFire.gif");
    m_movie->setParent(this);
    m_movie->setSpeed(10);
    m_movie->start();

    setTransformOriginPoint(8, 10);

    timeBuffer = 0;
    // 基本数据
    maxHP=21;
    nowHP = 21;
    speed = 3;

    connect(enemyTimer, &QTimer::timeout, this, &EnemyZombleFire::FireSkill);
    attackCount=0;
}

void EnemyZombleFire::Pathfinding (){
    if(isFrozen)return;

    if (!enemyPlayer)return;

    // 计算出 Enemy 到 Player 的方向
    QLineF line(pos(), enemyPlayer->pos());
    // 计算出移动的距离和方向
    qreal dx = cos(line.angle() * M_PI / 180) * speed;
    qreal dy = -sin(line.angle() * M_PI / 180) * speed;
    QPointF newPos = pos() + QPointF(dx, dy);



    if (mapbounds.contains(newPos)) {
        if (dx > 0) {
                newPos = pos() + QPointF(speed, 0);
            }
            else {
                newPos = pos() + QPointF(-speed, 0);
            }
        }
        if (mapbounds.contains(newPos)) {
            if (dy > 0) {
                newPos = pos() + QPointF(0, speed);
            }else {
                newPos = pos() + QPointF(0, -speed);
            }
        }
        if (mapbounds.contains(newPos)){
            newPos = pos() + QPointF(0, 0);
        }
        // 移动 Enemy
        setPos(newPos);
        timeBuffer++;

}

void EnemyZombleFire::load()
{
    QImage img(m_movie->currentImage());

    if (isHurt)
    {
        uchar *pixels = img.bits();
        int width = img.width();
        int height = img.height();
        int bytesPerLine = img.bytesPerLine();
        for (int y = 0; y < height; ++y)
        {
            uchar *line = pixels + y * bytesPerLine;
            for (int x = 0; x < width; ++x)
            {
                uchar alpha = line[x * 4 + 3];
                if (alpha != 0)
                {
                    line[x * 4] = 255;
                    line[x * 4 + 1] = 255;
                    line[x * 4 + 2] = 255;
                }
            }
        }
    }
    if (isFrozen) {
        uchar *pixels = img.bits();
        int width = img.width();
        int height = img.height();
        int bytesPerLine = img.bytesPerLine();
        for (int y = 0; y < height; ++y) {
            uchar *line = pixels + y * bytesPerLine;
            for (int x = 0; x < width; ++x) {
                uchar alpha = line[x * 4 + 3];
                if (alpha != 0) {
                    //  变蓝
                    line[x * 4] = qMin<uchar>(line[x * 4], 255);
                    line[x * 4 + 1] = qMin<uchar>(line[x * 4 + 1], 255);
                    line[x * 4 + 2] = qMin<uchar>(line[x * 4 + 2] + 100, 255);
                    line[x * 4 + 3] = qMin<uchar>(alpha + 64, 255);
                }
            }
        }
    }
    // 确定角度
    qreal angle = QLineF(pos(), enemyPlayer->pos()).angle();

    QPixmap pixmap = QPixmap::fromImage(img);
    // 镜像处理
    if ((angle > 90 && angle < 270) || (angle < -90 && angle > -270))
    {
        // 水平镜像
        pixmap = pixmap.transformed(QTransform().scale(-1, 1));
    }
    // 设置显示图像
    setPixmap(pixmap);
}

void EnemyZombleFire::FireSkill()
{
    if(isFrozen)return;

    attackCount++;
    qreal dist = QLineF(pos(), enemyPlayer->pos()).length();
    if (dist > 650){
        return;
    }
    if (attackCount < attackCeil) {
        return;
    }
    attackCount = 0;
    qreal angle = -QLineF(pos(), enemyPlayer->pos()).angle();

    QPointF tmpBias(100, 100);
    QPointF realPos=tmpBias+pos();//让子弹从中心点射出
    
    enemyFire *bullet = new enemyFire(realPos, angle, 6, 3, 0, enemyScene, enemyPlayer, enemyScene);
    bullet->setScale(2);
    bullet->setParentItem(this->parentItem());
    enemyScene->addItem(bullet);

}




