#include "fieldmodel.h"

#include <QRandomGenerator>
#include <QTimer>
#include <QDebug>

#include "simulator.h"

FieldModel::FieldModel(QObject *parent)
    : QAbstractTableModel(parent)
    , simulator(new Simulator(this))
    , m_running(false)
    , m_currTime(0)
{
    m_M = 0;
    m_N = 0;

    connect(this, &FieldModel::updateCell, this, &FieldModel::onUpdateCell, Qt::QueuedConnection);
    connect(simulator, &Simulator::moved, this, &FieldModel::moved, Qt::QueuedConnection);
    connect(simulator, &Simulator::currTimeChanged, this, [this](quint64 time){
        m_currTime = time;
        emit currTimeChanged();
    }, Qt::QueuedConnection);
}

bool FieldModel::running() const
{
    return m_running;
}

quint64 FieldModel::currTime() const
{
    return m_currTime;
}

int FieldModel::rowCount(const QModelIndex &) const
{
    return m_M;
}

int FieldModel::columnCount(const QModelIndex &) const
{
    return m_M;
}

QVariant FieldModel::data(const QModelIndex &index, int role) const
{
    if (index.column() < 0 || index.column() >= static_cast<int>(m_M))
        return QVariant();
    if (index.row() < 0 || index.row() >= static_cast<int>(m_M))
        return QVariant();
    switch (role) {
    case Qt::DisplayRole:
        return "!!!";
    case CountRole: {
        return field[index.row()][index.column()];
        /*
        const CellType &cell = field[index.row()][index.column()];
        QVariantList list;
        for (const FlyPtr &fly : cell) {
            list.append(QVariant(QVariantList {fly->dead, 2, fly->m_T}));
        }
        ///qDebug() << list;
        return list;*/
        }
        break;
    case EditCellRole:
        return editField[index.row()][index.column()];
        break;
    default:
        break;
    }

    return QVariant();
}

bool FieldModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != EditCellRole)
        return false;
    if (index.column() < 0 || index.column() >= static_cast<int>(m_M))
        return false;
    if (index.row() < 0 || index.row() >= static_cast<int>(m_M))
        return false;
    qDebug() << index << value << role;
    QVariantList list = value.value<QVariantList>();
    if (static_cast<quint32>(list.size()) > m_N)
        return false;
    editField[index.row()][index.column()] = list;
    emit dataChanged(index, index);
    return true;
}

QHash<int, QByteArray> FieldModel::roleNames() const
{
    return {
        {Qt::DisplayRole, QByteArrayLiteral("display")},
        {CountRole, QByteArrayLiteral("fcount")},
        {EditCellRole, QByteArrayLiteral("editCell")},
    };
}

void FieldModel::init(quint32 sizem, quint32 sizen)
{
    m_M = sizem;
    m_N = sizen;
    beginResetModel();
    field.resize(m_M);
    editField.resize(m_M);
    for (quint32 ycell = 0; ycell < m_M; ycell++) {
        field[ycell].resize(m_M);
        editField[ycell].resize(m_M);
        for (quint32 xcell = 0; xcell < m_M; xcell++) {
               field[ycell][xcell] = QVariantList();
               editField[ycell][xcell] = QVariantList();
        }
    }

    endResetModel();
    emit simulator->init(sizem, sizen);
}

void FieldModel::start()
{
    emit simulator->start();
    m_running = true;
    emit runningChanged();
}

void FieldModel::stop()
{
    emit simulator->stop();
    m_running = false;
    emit runningChanged();
}

void FieldModel::initRandom(quint32 num)
{
    emit simulator->initRandom(num);
}

void FieldModel::startEdit()
{

}

void FieldModel::endEdit()
{
    simulator->initFromEditor(editField);
}

void FieldModel::onUpdateCell(quint32 xcell, quint32 ycell, const QVariantList data)
{
    field[ycell][xcell] = data;
    emit dataChanged(index(ycell, xcell), index(ycell, xcell));
}

