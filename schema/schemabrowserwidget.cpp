#include "connections/connectionsmodel.h"
#include "connections/connectionsallprojectsmodel.h"
#include "sqltoolviewbase.h"
#include "ui_schemabrowser.h"
#include "schemabrowserwidget.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlDriver>

#define KDEV_SCHEMA_BROWSER_DB "kdev_schema"

namespace Sql
{

SchemaBrowserWidget::SchemaBrowserWidget(QComboBox *connection, QWidget *parent)
    : SqlToolViewBase(connection, parent), ui(new Ui::SchemaBrowser)
{
    ui->setupUi(this);
    schemaModel = nullptr;

    refreshButton = new QAction(QIcon::fromTheme("view-refresh"), i18n("Refresh"), this);
    currentConnectionChanged(m_connection->currentIndex());
    m_toolbarActions.append(refreshButton);
}

SchemaBrowserWidget::~SchemaBrowserWidget()
{
    delete ui;
    if(schemaModel) {
        delete schemaModel;
    }
    if(db.isOpen()) {
        db.close();
    }
    delete refreshButton;
}

void SchemaBrowserWidget::setConnection(Connection c)
{
    if(QSqlDatabase::contains(KDEV_SCHEMA_BROWSER_DB)) {
        if(db.isOpen()) {
            db.close();
        }
        QSqlDatabase::removeDatabase(KDEV_SCHEMA_BROWSER_DB);
    }

    db = QSqlDatabase::addDatabase(c.driver, KDEV_SCHEMA_BROWSER_DB);
    db.setHostName(c.hostName);
    db.setUserName(c.userName);
    db.setPassword(c.password);
    db.setDatabaseName(c.databaseName);
    if(!db.open()) {
        qDebug() << db.lastError();
        // TODO: Handle error here
    }
    qDebug() << db << "opened successfully!";

    if(!schemaModel) {
        schemaModel = new DbSchemaModel(&db);
        connect(refreshButton, SIGNAL(triggered(bool)), schemaModel, SLOT(refreshModelData()));
        ui->schemaView->setModel(schemaModel);
    }
    schemaModel->refreshModelData();
}

void SchemaBrowserWidget::currentConnectionChanged(int index)
{
    if(index < 0) {
        return;
    }
    setConnection(m_connectionsModel->connection(index));
    qDebug() << "Refreshed";
}

QList<QAction*> SchemaBrowserWidget::toolbarActions() const
{
    return m_toolbarActions;
}

}

#include "moc_schemabrowserwidget.cpp"
