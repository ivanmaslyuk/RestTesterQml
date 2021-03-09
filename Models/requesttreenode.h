#ifndef REQUESTTREENODE_H
#define REQUESTTREENODE_H

#include <QObject>
#include "requestmodel.h"

class RequestTreeNode : public QObject
{
    Q_OBJECT
    Q_PROPERTY(RequestModel* request READ request WRITE setRequest NOTIFY requestChanged)
    Q_PROPERTY(QString folderName READ folderName WRITE setFolderName NOTIFY folderNameChanged)

public:
    explicit RequestTreeNode(QObject *parent = nullptr);

    RequestModel *request() const;
    void setRequest(RequestModel *request);
    QString folderName() const;
    void setFolderName(QString name);
    bool isFolder() const;
    void setIsFolder(bool isFolder);

signals:
    void requestChanged(RequestModel *request);
    void folderNameChanged(QString folderName);

private:
    bool m_isFolder;
    RequestModel *m_request;
    QString m_folderName;
};

Q_DECLARE_METATYPE(RequestTreeNode *)

#endif // REQUESTTREENODE_H
