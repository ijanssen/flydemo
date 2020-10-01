#include "simulator.h"


#include <QRandomGenerator>
#include <QTimer>
#include <QDebug>

#include "fieldmodel.h"

const quint32 kAnimateTime = 500;

namespace {

inline quint32 getRandom(quint32 lowest, quint32 highest)
{
    return QRandomGenerator::global()->bounded(lowest, highest + 1);
}

inline quint32 getRandom(quint32 highest)
{
    return QRandomGenerator::global()->bounded(highest);
}

}


Simulator::Simulator(FieldModel *model)
    : model(model)
    , timer(new QTimer(this))
{
    m_M = 0;
    m_N = 0;

    connect(this, &Simulator::init, this,  &Simulator::onInit, Qt::QueuedConnection);
    connect(this, &Simulator::initRandom, this,  &Simulator::onInitRandom, Qt::QueuedConnection);
    connect(this, &Simulator::initFromEditor, this,  &Simulator::onInitFromEditor, Qt::QueuedConnection);
    connect(this, &Simulator::start, this, &Simulator::onStart, Qt::QueuedConnection);
    connect(this, &Simulator::stop, this, &Simulator::onStop, Qt::QueuedConnection);

    timer->setSingleShot(true);
    timer->setInterval(100);
    connect(timer, &QTimer::timeout, [this](){
        timeIteration();
    });

    dthread.start();
    moveToThread(&dthread);
}

Simulator::~Simulator()
{
    dthread.quit();
    if (!dthread.wait(3000)) {
        dthread.terminate();
        dthread.wait();
    }
}

void Simulator::onInit(quint32 sizem, quint32 sizen)
{
    m_M = sizem;
    m_N = sizen;
    currTime = 0;
    // init field
    field.resize(m_M);
    for (quint32 ycell = 0; ycell < m_M; ycell++) {
        field[ycell].resize(m_M);
    }
}

void Simulator::onInitFromEditor(const std::vector<std::vector<QVariantList> > &editField)
{
    while (!queue.empty())
        queue.pop();
    while (!deathQueue.empty())
        deathQueue.pop();
    currTime0 = 0;


    for (quint32 ycell = 0; ycell < m_M; ycell++)
        for (quint32 xcell = 0; xcell < m_M; xcell++) {
            field[ycell][xcell].clear();
            const QVariantList list = editField[ycell][xcell];
            for (const QVariant &d : list) {
                createNewFly(d.toInt(), xcell, ycell);
            }
            updateCell(xcell, ycell);
        }
}

void Simulator::onInitRandom(quint32 num)
{
    while (!queue.empty())
        queue.pop();
    while (!deathQueue.empty())
        deathQueue.pop();
    currTime0 = 0;

    for (quint32 ycell = 0; ycell < m_M; ycell++)
        for (quint32 xcell = 0; xcell < m_M; xcell++)
            field[ycell][xcell].clear();

     for (quint32 i = 0; i < num; i++) {
         const quint32 xcell = getRandom(m_M);
         const quint32 ycell = getRandom(m_M);
         const quint32 time = getRandom(1, 11);
         createNewFly(time, xcell, ycell);
         updateCell(xcell, ycell);
     }

}

void Simulator::onStart()
{
    elapsedTimer.start();
    timer->start();
}

void Simulator::onStop()
{
    timer->stop();
    timeIteration(false);
    currTime0 = currTime;
    // Update stat
    for (quint32 ycell = 0; ycell < m_M; ycell++)
        for (quint32 xcell = 0; xcell < m_M; xcell++)
        updateCell(xcell, ycell);
}

void Simulator::updateCell(quint32 xcell, quint32 ycell)
{
    const CellType &cell = field[ycell][xcell];
    QVariantList list;
    for (const FlyPtr &fly : cell) {
        list.append(QVariant(QVariantList {fly->dead, fly->m_T, fly->movesCount, fly->dead ? fly->m_deathTime : currTime}));
    }
    emit model->updateCell(xcell, ycell, list);
}

void Simulator::timeIteration(bool startNext)
{
    currTime = currTime0 + elapsedTimer.elapsed();
    emit currTimeChanged(currTime);
    qDebug() << currTime;
    while (true) {
        if (queue.size() == 0 && deathQueue.size() == 0) {
            // end
            return;
        }
        quint64 minMoveTime = std::numeric_limits<quint64>::max();
        if (queue.size() > 0)
            minMoveTime = queue.top()->m_nextMove;
        quint64 minDeathTime = std::numeric_limits<quint64>::max();
        if (deathQueue.size() > 0)
            minDeathTime = deathQueue.top()->m_deathTime;
        //qDebug() << "D: " << minDeathTime;
        const quint64 minTime = qMin(minMoveTime, minDeathTime);
        if (minTime > currTime) {
            // wait next iteration
            break;
        }
        if (minTime == minDeathTime) {
            deathQueue.top()->dead = true;
            updateCell(deathQueue.top()->xcell, deathQueue.top()->ycell);
            deathQueue.pop();
        } else {
            qDebug() << "try";
            FlyPtr fly = queue.top();
            queue.pop();
            if (!fly->dead) {
                tryDoMovement(fly);
                queue.emplace(fly);
            }
        }
    }
    if (startNext)
        timer->start();
}

void Simulator::updateNextMoveTime(const Simulator::FlyPtr &fly)
{
    fly->m_nextMove = currTime + getRandom(kAnimateTime, fly->m_T * 1000);
    //qDebug() << "NEXT " << fly->m_nextMove;
}

void Simulator::tryDoMovement(const Simulator::FlyPtr &fly)
{
    const quint32 nxcell = fly->xcell + getRandom((fly->xcell > 0) ? 0 : 1, (fly->xcell < m_M - 1) ? 2 : 1) - 1;
    const quint32 nycell = fly->ycell + getRandom((fly->ycell > 0) ? 0 : 1, (fly->ycell < m_M - 1) ? 2 : 1) - 1;
    // TODO
    updateNextMoveTime(fly);
    CellType &ncell = field[nycell][nxcell];
    if (ncell.size() >= m_N) {
        // new cell is full
        return;
    }
    // move
    fly->movesCount++;
    field[fly->ycell][fly->xcell].remove(fly);
    emit moved(fly->xcell, fly->ycell, nxcell, nycell);
    updateCell(fly->xcell, fly->ycell);
    //emit dataChanged(index(fly->ycell, fly->xcell), index(fly->ycell, fly->xcell));
    fly->xcell = nxcell;
    fly->ycell = nycell;
    ncell.emplace_back(fly);
    updateCell(nxcell, nycell);
    //emit dataChanged(index(nycell, nxcell), index(nycell, nxcell));
}

void Simulator::createNewFly(quint32 t, quint32 xcell, quint32 ycell)
{
    CellType &cell = field[ycell][xcell];
    if (cell.size() >= m_N)
        return;
    qDebug() << "Create with " << t << m_M * t;
    const quint64 death = m_M * t * 500;
//    const quint64 death = m_M * t * 1000; // death time (ms)
    FlyPtr fly = std::make_shared<Fly>(t, death, xcell, ycell);
    updateNextMoveTime(fly);

    cell.emplace_back(fly);
    deathQueue.emplace(fly);
    queue.emplace(fly);
}
