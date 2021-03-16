#include "treemodel.h"
#include <QVariant>
#include <QDebug>
#include <QPair>

TreeModel::TreeModel(RequestTreeNode *rootNode, QObject *parent)
    : QAbstractItemModel(parent)
{
    rootItem = new TreeItem(QVariant::fromValue(rootNode));
    setupModelData(rootNode, rootItem);
}

TreeModel::~TreeModel()
{
    delete rootItem;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    RequestTreeNode *node = item->data().value<RequestTreeNode*>();

    if (role == 0) {
        return node->isFolder() ? node->folderName() : node->request()->name();
    }
    else if (role == 1) {
        return node->isFolder() ? QVariant() : node->request()->method();
    }

    return QVariant();
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    TreeItem *item = getItem(index);
    bool result = item->setData(value);

    if (result)
        emit dataChanged(index, index, {Qt::DisplayRole});

    return result;
}

bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    if (!parentItem)
        return false;

    beginInsertRows(parent, position, position + rows - 1);
    const bool success = parentItem->insertChildren(position, rows);
    endInsertRows();

    return success;
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    if (!parentItem)
        return false;

    beginRemoveRows(parent, position, position + rows - 1);
    const bool success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

QHash<int, QByteArray> TreeModel::roleNames() const
{
    return {
        {Roles::name, "name"},
        {Roles::method, "method"}
    };
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    const TreeItem *parentItem = getItem(parent);

    return parentItem ? parentItem->childCount() : 0;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

void TreeModel::setupModelData(RequestTreeNode *rootNode, TreeItem *parent)
{
    Q_UNUSED(parent)

    QList<QPair<QModelIndex, RequestTreeNode *>> nodes;

    // Add root's children to nodes
    QList<RequestTreeNode *> rootChildren = rootNode->findChildren<RequestTreeNode *>("", Qt::FindDirectChildrenOnly);
    insertRows(0, rootChildren.length());
    for (int i = 0; i < rootChildren.length(); ++i)
        nodes.append({index(i, 0), rootChildren[i]});

    while (nodes.count() > 0) {
        QPair<QModelIndex, RequestTreeNode *> item = nodes.takeFirst();
        QModelIndex modelIndex = item.first;
        RequestTreeNode *node = item.second;

        // Add current item to model
        setData(modelIndex, QVariant::fromValue(node));

        // Add children of current item next
        QList<RequestTreeNode *> nodeChildren = node->findChildren<RequestTreeNode *>("", Qt::FindDirectChildrenOnly);
        insertRows(0, nodeChildren.length(), modelIndex);
        for (int i = 0; i < nodeChildren.length(); ++i)
            nodes.append({index(i, 0, modelIndex), nodeChildren[i]});
    }
}

TreeItem *TreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}
