#ifndef REQUESTTREENODE_H
#define REQUESTTREENODE_H

#include <QObject>
#include "request.h"

class RequestTreeNode : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Request* request READ request WRITE setRequest NOTIFY requestChanged)
    Q_PROPERTY(QString folderName READ folderName WRITE setFolderName NOTIFY folderNameChanged)

public:
    explicit RequestTreeNode(QObject *parent = nullptr);

    Request *request() const;
    void setRequest(Request *request);
    QString folderName() const;
    void setFolderName(QString name);
    bool isFolder() const;
    void setIsFolder(bool isFolder);

signals:
    void requestChanged(Request *request);
    void folderNameChanged(QString folderName);

private:
    bool m_isFolder;
    Request *m_request;
    QString m_folderName;
};

Q_DECLARE_METATYPE(RequestTreeNode *)

#endif // REQUESTTREENODE_H
