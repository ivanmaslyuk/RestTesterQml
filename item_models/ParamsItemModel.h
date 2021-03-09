#ifndef PARAMSITEMMODEL_H
#define PARAMSITEMMODEL_H

#include <QAbstractItemModel>
#include <QObject>
#include <Models/parammodel.h>

class ParamsItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit ParamsItemModel(QList<ParamModel *> *params, QObject *parent = nullptr);

    enum DataRole {
        IsEnabled,
        Key,
        Value,
    };
    Q_ENUM(DataRole)

    QHash<int, QByteArray> roleNames() const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Q_INVOKABLE void appendRow();

private:
    QList<ParamModel *> *m_params;
};

#endif // PARAMSITEMMODEL_H
