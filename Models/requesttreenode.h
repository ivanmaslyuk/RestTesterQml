#ifndef REQUESTTREENODE_H
#define REQUESTTREENODE_H

#include <QObject>
#include <QJsonObject>
#include "request.h"

class RequestTreeNode : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Request* request READ request WRITE setRequest NOTIFY requestChanged)
    Q_PROPERTY(QString folderName READ folderName WRITE setFolderName NOTIFY folderNameChanged)

public:
    explicit RequestTreeNode(QObject *parent = nullptr);
    explicit RequestTreeNode(int localId, QObject *parent = nullptr);
    explicit RequestTreeNode(QString uuid, QObject *parent = nullptr);

    int localId() const;
    void setLocalId(int id);

    QString uuid() const;
    void setUuid(QString uuid);

    Request *request() const;
    void setRequest(Request *request);

    QString folderName() const;
    void setFolderName(QString name);

    bool isFolder() const;
    void setIsFolder(bool isFolder);

    QDateTime editedAt() const;
    void setEditedAt(QDateTime value);

    int pointer() const;
    void setPointer(int value);

    bool deleted() const;
    void setDeleted(bool deleted);

    QString parentUuid() const;
    void setParentUuid(QString parentUuid);

    bool hasParent() const;

signals:
    void requestChanged(Request *request);
    void folderNameChanged(QString folderName);

private:
    QString m_parentUuid;
    QString m_uuid;
    bool m_deleted;
    int m_localId;
    int m_pointer;
    QDateTime m_editedAt;
    bool m_isFolder;
    Request *m_request;
    QString m_folderName;
};

Q_DECLARE_METATYPE(RequestTreeNode *)

#endif // REQUESTTREENODE_H
