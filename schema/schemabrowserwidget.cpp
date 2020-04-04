#include "connections/connectionsmodel.h"
#include "connections/connectionsallprojectsmodel.h"
#include "ui_schemabrowser.h"
#include "schemabrowserwidget.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlDriver>

#define KDEV_SCHEMA_BROWSER_DB "kdev_schema"

namespace Sql
{

SchemaBrowserWidget::SchemaBrowserWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::SchemaBrowser)
{
    ui->setupUi(this);
    schemaModel = nullptr;
    allConnections = new ConnectionsAllProjectsModel(this);
    ui->connection->setModel(allConnections);
    connect(ui->connection, SIGNAL(currentIndexChanged(int)), this, SLOT(connectionChanged(int)));
    connectionChanged(ui->connection->currentIndex());
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
    delete allConnections;
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
        connect(ui->refreshButton, SIGNAL(clicked()), schemaModel, SLOT(refreshModelData()));
        ui->schemaView->setModel(schemaModel);
    }
    schemaModel->refreshModelData();
}

void SchemaBrowserWidget::connectionChanged(int index)
{
    if(index < 0) {
        return;
    }
    setConnection(allConnections->connection(index));
}

}

#include "moc_schemabrowserwidget.cpp"
