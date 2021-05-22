#include "ParamsItemModel.h"
#include <QDebug>

ParamsItemModel::ParamsItemModel(QList<ParamModel *> *params, bool readOnly, QObject *parent)
    : QAbstractItemModel(parent)
{
    m_readOnly = readOnly;
    m_params = params;
    if ((m_params->isEmpty() || !m_params->last()->isEmpty()) && !readOnly)
        m_params->append(new ParamModel("", ""));
}

QHash<int, QByteArray> ParamsItemModel::roleNames() const
{
    return {
        { DataRole::IsEnabled, "isEnabled" },
        { DataRole::Key, "key" },
        { DataRole::Value, "value" },
    };
}

QModelIndex ParamsItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    ParamModel *param = m_params->at(row);
    return createIndex(row, column, param);
}

QModelIndex ParamsItemModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

int ParamsItemModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_params->length();
}

int ParamsItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

bool ParamsItemModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent)
    Q_UNUSED(count)

    if (row < 0 || row >= m_params->count())
        return false;

    beginRemoveRows(parent, row, row);
    m_params->removeAt(row);
    endRemoveRows();

    return true;
}

QVariant ParamsItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    ParamModel *param = static_cast<ParamModel *>(index.internalPointer());

    if (role == DataRole::IsEnabled)
        return QVariant(param->enabled);
    else if (role == DataRole::Key)
        return QVariant(param->key);
    else if (role == DataRole::Value)
        return QVariant(param->value);

    return QVariant();
}

bool ParamsItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    if (data(index, role) == value)
        return true; // Prevent binding loop

    if (index.row() == rowCount() - 1 && role != DataRole::IsEnabled)
        appendEmptyRow();

    ParamModel *param = static_cast<ParamModel *>(index.internalPointer());

    if (role == DataRole::IsEnabled) {
        param->enabled = value.toBool();
        emit dataChanged(index, index, {DataRole::IsEnabled});
        return true;
    } else if (role == DataRole::Key) {
        param->key = value.toString();
        emit dataChanged(index, index, {DataRole::Key});
        return true;
    } else if (role == DataRole::Value) {
        param->value = value.toString();
        emit dataChanged(index, index, {DataRole::Value});
        return true;
    }

    return false;
}

void ParamsItemModel::replaceData(QList<ParamModel *> newParams)
{
    beginResetModel();
    m_params->clear();
    m_params->append(newParams);
    if ((m_params->isEmpty() || !m_params->last()->isEmpty()) && !m_readOnly)
        m_params->append(new ParamModel("", ""));
    endResetModel();
}

void ParamsItemModel::appendEmptyRow()
{
    int position = m_params->length();
    beginInsertRows(QModelIndex(), position, position);
    m_params->append(new ParamModel("", "", true));
    endInsertRows();

    emit emptyRowAppended();
}
