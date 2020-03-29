#ifndef SQL_SCHEMABROWSER_H
#define SQL_SCHEMABROWSER_H

#include <QWidget>
#include <QSqlDatabase>
#include "connections/connectionsmodel.h"
#include "dbschemamodel.h"

namespace Ui {
    class SchemaBrowser;
}

namespace Sql {

class ConnectionsAllProjectsModel;

class SchemaBrowserWidget : public QWidget {
    Q_OBJECT
public:
    SchemaBrowserWidget(QWidget *parent = 0);
    ~SchemaBrowserWidget();

public slots:
    void connectionChanged(int index);

private:
    void setConnection(Connection c);
    QSqlDatabase db;
    Ui::SchemaBrowser *ui;
    DbSchemaModel *schemaModel;
    ConnectionsAllProjectsModel *allConnections;
};

}

#endif //SQL_SCHEMABROWSER_H
