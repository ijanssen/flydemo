#ifndef BOARDMODEL_H
#define BOARDMODEL_H

#include <QAbstractTableModel>

class Simulator;
class QTimer;

class FieldModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(bool running READ running NOTIFY runningChanged)
    Q_PROPERTY(quint64 currTime READ currTime NOTIFY currTimeChanged)

public:
    FieldModel(QObject *parent = nullptr);

    bool running() const;
    quint64 currTime() const;

    enum Roles {
        FlyListRole = Qt::UserRole + 1,
        EditCellRole = Qt::UserRole + 2
    };

    int rowCount(const QModelIndex & = QModelIndex()) const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    QHash<int, QByteArray> roleNames() const override;

public:
    Q_INVOKABLE void init(quint32 sizem, quint32 sizen);
    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void initRandom(quint32 num);
    Q_INVOKABLE void startEdit();
    Q_INVOKABLE void endEdit();

signals:
    void runningChanged();
    void currTimeChanged();
    void moved(quint32 xcell, quint32 ycell, quint32 nxcell, quint32 nycell);

signals:
    void updateCell(quint32 xcell, quint32 ycell, const QVariantList data);

private slots:
    void onUpdateCell(quint32 xcell, quint32 ycell, const QVariantList data);

private:
    Simulator *simulator;
    quint32 m_M;
    quint32 m_N;
    std::vector<std::vector<QVariantList>> field;
    std::vector<std::vector<QVariantList>> editField;

    bool m_running;
    quint64 m_currTime;
};

#endif // BOARDMODEL_H
