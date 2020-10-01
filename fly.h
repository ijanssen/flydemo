#ifndef FLY_H
#define FLY_H

#include <QtGlobal>
#include <memory>

class Fly
{
public:
    Fly(quint32 t, quint64 death, quint32 xcell, quint32 ycell);

    quint64 m_nextMove;
    quint32 m_T;
    quint64 m_deathTime;
    quint32 xcell;
    quint32 ycell;
    bool dead;
    quint32 movesCount;
};

struct FlyCompare
{
    inline bool operator()(const std::shared_ptr<Fly> &a, const std::shared_ptr<Fly> &b)
    {
        return a->m_nextMove > b->m_nextMove;
    }
};

struct FlyDeathCompare
{
    inline bool operator()(const std::shared_ptr<Fly> &a, const std::shared_ptr<Fly> &b)
    {
        return a->m_deathTime > b->m_deathTime;
    }
};

#endif // FLY_H
