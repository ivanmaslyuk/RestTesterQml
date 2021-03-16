#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QObject>
#include "treeitem.h"
#include "Models/requesttreenode.h"

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeModel(RequestTreeNode *rootNode, QObject *parent = nullptr);
    ~TreeModel();

    enum Roles {
        name,
        method
    };
    Q_ENUM(Roles)

    QVariant data(const QModelIndex &index, int role) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                     int role = Qt::EditRole) override;
    bool insertRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;
    QHash<int, QByteArray> roleNames() const override;

    TreeItem *rootItem() const;

private:
    void setupModelData(RequestTreeNode *rootNode, TreeItem *parent);
    TreeItem *getItem(const QModelIndex &index) const;

    TreeItem *m_rootItem;
};

#endif // TREEMODEL_H
