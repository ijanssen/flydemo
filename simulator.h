#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QObject>
#include <QThread>
#include <queue>

#include "fly.h"

class FieldModel;
class QTimer;

class Simulator : public QObject
{
    Q_OBJECT
public:
    Simulator(FieldModel *model);
    virtual ~Simulator();

signals:
    void init(quint32 sizem, quint32 sizen);
    void initRandom(quint32 num);
    void initFromEditor(const std::vector<std::vector<QVariantList>> &editField);
    void start();
    void stop();
    void moved(quint32 xcell, quint32 ycell, quint32 nxcell, quint32 nycell);
    void currTimeChanged(quint64 time);

private slots:
    void onInit(quint32 sizem, quint32 sizen);
    void onInitFromEditor(const std::vector<std::vector<QVariantList>> &editField);
    void onInitRandom(quint32 num);
    void onStart();
    void onStop();

private:
    FieldModel *model;
    QThread dthread;

    using FlyPtr = std::shared_ptr<Fly>;
    using CellType = std::list<FlyPtr>;

    void updateCell(quint32 xcell, quint32 ycell);
    void timeIteration(bool startNext = true);
    void updateNextMoveTime(const FlyPtr &fly);
    void tryDoMovement(const FlyPtr &fly);
    void createNewFly(quint32 t, quint32 xcell, quint32 ycell);

    quint32 m_M;
    quint32 m_N;
    std::vector<std::vector<CellType>> field;

    std::priority_queue<FlyPtr, std::vector<FlyPtr>, FlyCompare> queue;
    std::priority_queue<FlyPtr, std::vector<FlyPtr>, FlyCompare> deathQueue;

    QTimer *timer;
    quint64 currTime;
    quint64 currTime0;
    QElapsedTimer elapsedTimer;
};

#endif // SIMULATOR_H
