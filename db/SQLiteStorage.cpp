#include <QDebug>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QUuid>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QSqlError>
#include "SQLiteStorage.h"


SQLiteStorage::SQLiteStorage(QObject *parent) : QObject(parent)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    QString folder = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (folder.isEmpty())
        qDebug() << "No writable location";

    m_db.setDatabaseName(folder + "/data.sqlite");
    m_db.open();

    if (!m_db.isOpen()) {
        qDebug() << "Could not open database";
    } else {
        QSqlQuery query;
        query.exec("PRAGMA foreign_keys = ON;");
        handleErrors(query);
    }

    runMigrations();
}

RequestTreeNode *SQLiteStorage::getRequestTree()
{
    RequestTreeNode *rootNode = new RequestTreeNode(this);
    rootNode->setIsFolder(true);

    // Get nodes from db
    QSqlQuery nodeQuery("SELECT * FROM node;");
    QMap<int, RequestTreeNode *> nodeMap;
    while (nodeQuery.next()) {
        int localId = nodeQuery.value("id").toInt();
        RequestTreeNode *node = new RequestTreeNode(localId);
        node->setProperty("parent_id", nodeQuery.value("parent_id"));
        nodeMap[localId] = node;
        node->setIsFolder(nodeQuery.value("is_folder").toBool());
        node->setFolderName(nodeQuery.value("folder_name").toString());
    }

    // Assign parent nodes
    for (int nodeId : nodeMap.keys()) {
        RequestTreeNode *node = nodeMap[nodeId];

        RequestTreeNode *parentNode = nodeMap.value(node->property("parent_id").toInt(), rootNode);
        node->setParent(parentNode);
    }

    // Get requests from db
    QSqlQuery requestQuery("SELECT * FROM request;");
    while (requestQuery.next()) {
        QString queryParamsJson = requestQuery.value("query_params_json").toString();
        QList<ParamModel *> queryParams = paramsFromJson(queryParamsJson);

        QString dataParamsJson = requestQuery.value("data_params_json").toString();
        QList<ParamModel *> dataParams = paramsFromJson(dataParamsJson);

        QString headersJson = requestQuery.value("headers_json").toString();
        QList<ParamModel *> headers = paramsFromJson(headersJson);

        int localId = requestQuery.value("id").toInt();
        RequestTreeNode *node = nodeMap[requestQuery.value("node_id").toInt()];

        Request *request = new Request(localId, node);
        request->setUrl(requestQuery.value("url").toString());
        request->setQueryParams(queryParams);
        request->setDataParams(dataParams);
        request->setHeaders(headers);
        request->setRawData(requestQuery.value("raw_data").toString());
        request->setMethod(requestQuery.value("method").toString());
        request->setName(requestQuery.value("name").toString());
        request->setContentType(requestQuery.value("content_type").toString());
        request->setDocumentation(requestQuery.value("documentation").toString());
        request->setEdited(false);

        node->setRequest(request);
    }

    return rootNode;
}

void SQLiteStorage::saveNode(RequestTreeNode *node)
{
    QSqlQuery nodeQuery;
    nodeQuery.prepare("UPDATE node SET folder_name = :folder_name, parent_id = :parent_id WHERE id = :id;");
    nodeQuery.bindValue(":folder_name", node->folderName());
    nodeQuery.bindValue(":id", node->localId());

    // Check if the node has a parent
    RequestTreeNode *parentNode = static_cast<RequestTreeNode *>(node->parent());

    if (!parentNode->isFolder()) {
        qDebug() << "Parent node must be a folder or root node";
        return;
    }

    if (parentNode->localId() != -1) {
        nodeQuery.bindValue(":parent_id", parentNode->localId());
    } else {
        nodeQuery.bindValue(":parent_id", QVariant());
    }

    nodeQuery.exec();
    handleErrors(nodeQuery);

    // Save request
    if (!node->isFolder()) {
        QSqlQuery requestQuery;
        requestQuery.prepare("UPDATE request SET name = :name, url = :url, raw_data = :raw_data, "
                          "method = :method, content_type = :content_type, documentation = :documentation, "
                          "query_params_json = :query_params_json, data_params_json = :data_params_json, "
                          "headers_json = :headers_json WHERE id = :id;");
        requestQuery.bindValue(":id", node->request()->localId());
        requestQuery.bindValue(":name", node->request()->name());
        requestQuery.bindValue(":url", node->request()->url());
        requestQuery.bindValue(":raw_data", node->request()->rawData());
        requestQuery.bindValue(":method", node->request()->method());
        requestQuery.bindValue(":content_type", node->request()->contentType());
        requestQuery.bindValue(":documentation", node->request()->documentation());
        requestQuery.bindValue(":query_params_json", paramsToJson(node->request()->queryParams()));
        requestQuery.bindValue(":data_params_json", paramsToJson(node->request()->dataParams()));
        requestQuery.bindValue(":headers_json", paramsToJson(node->request()->headers()));
        requestQuery.exec();
        handleErrors(requestQuery);
    }
}

void SQLiteStorage::createNode(RequestTreeNode *node)
{
    m_db.transaction();

    QSqlQuery nodeQuery;
    nodeQuery.prepare("INSERT INTO node (uuid, is_folder) VALUES (:uuid, :is_folder);");
    nodeQuery.bindValue(":uuid", getUuid());
    nodeQuery.bindValue(":is_folder", node->isFolder());
    nodeQuery.exec();
    handleErrors(nodeQuery);

    int nodeId = nodeQuery.lastInsertId().toInt();
    node->setLocalId(nodeId);

    if (!node->isFolder()) {
        QSqlQuery requestQuery;
        requestQuery.prepare("INSERT INTO request (uuid, node_id, name) VALUES (:uuid, :node_id, :name);");
        requestQuery.bindValue(":uuid", getUuid());
        requestQuery.bindValue(":node_id", nodeId);
        requestQuery.bindValue(":name", node->request()->name());
        requestQuery.exec();
        handleErrors(requestQuery);

        int requestId = requestQuery.lastInsertId().toInt();
        node->request()->setLocalId(requestId);
    }

    saveNode(node);

    m_db.commit();
}

void SQLiteStorage::deleteNode(RequestTreeNode *node)
{
    if (node->localId() == -1) {
        qDebug() << "Cannot delete node that hasn't been created in the db";
        return;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM node WHERE id = :id");
    query.bindValue(":id", node->localId());
    query.exec();
    handleErrors(query);
}

QList<ParamModel *> SQLiteStorage::paramsFromJson(QString json)
{
    QList<ParamModel *> params;
    for (QJsonValue value : QJsonDocument::fromJson(json.toUtf8()).array())
        params.append(new ParamModel(value["key"].toString(),
                           value["value"].toString(), value["enabled"].toBool()));

    return params;
}

QString SQLiteStorage::paramsToJson(QList<ParamModel *> params)
{
    QJsonArray array;
    for (ParamModel *param : params)
        array.append(QJsonObject({{"key", param->key}, {"value", param->value}, {"enabled", param->enabled}}));

    QJsonDocument doc;
    doc.setArray(array);
    return doc.toJson();
}

void SQLiteStorage::handleErrors(QSqlQuery query)
{
    QSqlError error = query.lastError();
    if (error.type() != QSqlError::NoError) {
        qDebug() << "Error while executing query:";
        qDebug() << query.lastQuery();
        qDebug() << error.text();
    }
}

QString SQLiteStorage::getUuid()
{
    return QUuid::createUuid().toString().remove('{').remove('}');
}

void SQLiteStorage::runMigrations()
{
    QString createNodeTable = ""
        "create table node"
        "("
        "    id          integer  not null primary key autoincrement,"
        "    uuid        CHAR(36) not null unique,"
        "    parent_id   integer references node on delete cascade,"
        "    is_folder   bool     not null,"
        "    folder_name text"
        ");";
    QSqlQuery(createNodeTable).exec();

    QString createRequestTable = ""
        "create table request"
        "("
        "    id                integer  not null primary key autoincrement,"
        "    uuid              CHAR(36) not null unique,"
        "    node_id           integer  not null references node on delete cascade,"
        "    name              text     not null,"
        "    url               text,"
        "    raw_data          text,"
        "    method            text,"
        "    content_type      text,"
        "    documentation     text,"
        "    query_params_json text,"
        "    data_params_json  text,"
        "    headers_json      text"
        ");";
    QSqlQuery(createRequestTable).exec();
}
