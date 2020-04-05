#ifndef SQL_SCHEMABROWSER_H
#define SQL_SCHEMABROWSER_H

#include <QWidget>
#include <QComboBox>
#include <QSqlDatabase>
#include <QAction>
#include <QList>
#include "connections/connectionsmodel.h"
#include "dbschemamodel.h"
#include "sqltoolviewbase.h"

namespace Ui {
    class SchemaBrowser;
}

namespace Sql {

class ConnectionsAllProjectsModel;

class SchemaBrowserWidget : public SqlToolViewBase {
    Q_OBJECT
public:
    SchemaBrowserWidget(QComboBox *connection, QWidget *parent = 0);
    ~SchemaBrowserWidget();

    QList<QAction*> toolbarActions() const override;

public slots:
    virtual void currentConnectionChanged(int index) override;

private:
    void setConnection(Connection c);
    QSqlDatabase db;
    Ui::SchemaBrowser *ui;
    DbSchemaModel *schemaModel;
    QAction *refreshButton;
};

}

#endif //SQL_SCHEMABROWSER_H
