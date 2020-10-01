#include "fly.h"

Fly::Fly(quint32 t, quint64 death, quint32 xcell, quint32 ycell)
    : m_nextMove(0)
    , m_T(t)
    , m_deathTime(death)
    , xcell(xcell)
    , ycell(ycell)
    , dead(false)
    , movesCount(0)
{
}
