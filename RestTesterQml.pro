QT += quick sql

CONFIG += c++11
# CONFIG += qtquickcompiler

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Error.cpp \
        Models/Response.cpp \
        Models/parammodel.cpp \
        Models/request.cpp \
        Models/requesttreenode.cpp \
        Models/treeitem.cpp \
        Models/treemodel.cpp \
        app.cpp \
        db/JsonStorage.cpp \
        db/SQLiteStorage.cpp \
        item_models/ParamsItemModel.cpp \
        main.cpp \
        server_api/Authenticator.cpp \
        server_api/NodeSerializer.cpp \
        server_api/RequestSerializer.cpp \
        server_api/ServerApi.cpp \
        server_api/ServerSyncService.cpp \
        utils/HttpClient.cpp

RESOURCES += qml.qrc \
    icons.qrc

TRANSLATIONS += \
    RestTesterQml_en_US.ts

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Error.h \
    Models/Response.h \
    Models/parammodel.h \
    Models/request.h \
    Models/requesttreenode.h \
    Models/treeitem.h \
    Models/treemodel.h \
    app.h \
    db/JsonStorage.h \
    db/SQLiteStorage.h \
    item_models/ParamsItemModel.h \
    server_api/Authenticator.h \
    server_api/NodeSerializer.h \
    server_api/RequestSerializer.h \
    server_api/ServerApi.h \
    server_api/ServerSyncService.h \
    utils/HttpClient.h
