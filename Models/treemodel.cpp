#include "treemodel.h"
#include <QVariant>
#include <QDebug>
#include <QPair>
#include "Error.h"

TreeModel::TreeModel(RequestTreeNode *rootNode, QObject *parent)
    : QAbstractItemModel(parent)
{
    setRootNode(rootNode);
}

TreeModel::~TreeModel()
{
    delete m_rootItem;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    RequestTreeNode *node = item->data().value<RequestTreeNode*>();

    if (role == Roles::name) {
        return node->isFolder() ? node->folderName() : node->request()->name();
    }
    else if (role == Roles::method) {
        return node->isFolder() ? QVariant() : node->request()->method();
    } else if (role == Roles::isFolder) {
        return node->isFolder();
    }

    return QVariant();
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

bool TreeModel::hasChildren(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return false;

    TreeItem *item = static_cast<TreeItem *>(parent.internalPointer());
    RequestTreeNode *node = item->data().value<RequestTreeNode *>();

    return node->isFolder();
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
        {Roles::method, "method"},
        {Roles::isFolder, "isFolder"},
    };
}

TreeItem *TreeModel::rootItem() const
{
    return m_rootItem;
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = m_rootItem;
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

    if (parentItem == m_rootItem)
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

void TreeModel::setRootNode(RequestTreeNode *rootNode)
{
    beginResetModel();

    m_rootItem = new TreeItem(QVariant::fromValue(rootNode));

    QList<QPair<QModelIndex, RequestTreeNode *>> nodes;

    auto sortFunc = [](RequestTreeNode *node1, RequestTreeNode *node2) {
        if (node1->isFolder() && !node2->isFolder())
            return true;
        if (!node1->isFolder() && node2->isFolder())
            return false;
        if (node1->isFolder() && node2->isFolder())
            return node1->folderName().toLower() < node2->folderName().toLower();
        if (!node1->isFolder() && !node2->isFolder())
            return node1->request()->name().toLower() < node2->request()->name().toLower();
        throw Error("Sort function not full!");
    };

    // Add root's children to nodes
    QList<RequestTreeNode *> rootChildren = rootNode->findChildren<RequestTreeNode *>("", Qt::FindDirectChildrenOnly);

    std::sort(rootChildren.begin(), rootChildren.end(), sortFunc);
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
        std::sort(nodeChildren.begin(), nodeChildren.end(), sortFunc);
        insertRows(0, nodeChildren.length(), modelIndex);
        for (int i = 0; i < nodeChildren.length(); ++i)
            nodes.append({index(i, 0, modelIndex), nodeChildren[i]});
    }

    endResetModel();
}

TreeItem *TreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return m_rootItem;
}
