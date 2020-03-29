/*
    Copyright (C) 2010 Niko Sams <niko.sams@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#include "connectionsmodel.h"
#include "ui_connections.h"
#include "connections.h"

#include <interfaces/iproject.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QtCore>

namespace Sql
{

ProjectConfigPage::ProjectConfigPage(KDevelop::IPlugin *plugin,
        const KDevelop::ProjectConfigOptions &options,
        QWidget *parent)
    : KDevelop::ConfigPage(plugin, nullptr, parent)
{
    project = options.project;
    Q_ASSERT(project);

    ui = new Ui_Connections;
    ui->setupUi(this);

    conn = new ConnectionsModel(project, this);

    ui->list->setModel(conn);
    connect(conn, SIGNAL(dataChanged(QModelIndex, QModelIndex)),
            this, SLOT(changed()));
    connect(conn, SIGNAL(rowsRemoved(QModelIndex, int, int)),
            this, SLOT(changed()));
    connect(ui->list->selectionModel(),
            SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
            SLOT(currentRowChanged(QModelIndex)));

    ui->testResult->setText("");
    ui->driver->insertItems(0, QSqlDatabase::drivers());

    connect(ui->driver, SIGNAL(currentIndexChanged(int)),
            SLOT(connectionEdited()));
    connect(ui->hostName, SIGNAL(textEdited(QString)),
            SLOT(connectionEdited()));
    connect(ui->database, SIGNAL(textEdited(QString)),
            SLOT(connectionEdited()));
    connect(ui->userName, SIGNAL(textEdited(QString)),
            SLOT(connectionEdited()));
    connect(ui->password, SIGNAL(textEdited(QString)),
            SLOT(connectionEdited()));

    reset();
}

ProjectConfigPage::~ProjectConfigPage()
{
    delete ui;
}

void ProjectConfigPage::apply()
{
    conn->submit();
}

void ProjectConfigPage::reset()
{
    conn->revert();
    ui->list->setCurrentIndex(conn->index(0, 0));
}



QIcon ProjectConfigPage::icon() const
{
    return QIcon::fromTheme(QStringLiteral("server-database"));
}

QString ProjectConfigPage::name() const
{
    return i18n("Database Connections");
}

void ProjectConfigPage::currentRowChanged(const QModelIndex& index)
{
    Connection c = conn->connection(index.row());
    ui->driver->blockSignals(true);
    ui->driver->setCurrentItem(c.driver);
    ui->driver->blockSignals(false);
    ui->hostName->setText(c.hostName);
    ui->database->setText(c.databaseName);
    ui->userName->setText(c.userName);
    ui->password->setText(c.password);
    testConnection();
}

void ProjectConfigPage::connectionEdited()
{
    Connection c;
    c.driver = ui->driver->currentText();
    c.hostName = ui->hostName->text();
    c.databaseName = ui->database->text();
    c.userName = ui->userName->text();
    c.password = ui->password->text();
    conn->setConnection(ui->list->currentIndex().row(), c);

    testConnection(); //TODO buffer
}

void ProjectConfigPage::testConnection()
{
    if (ui->list->currentIndex().row() == conn->rowCount()-1) {
        //new connection
        ui->testResult->setText("");
        return;
    }
    {
        Connection c = conn->connection(ui->list->currentIndex().row());
        if (c.driver.isEmpty()) {
            ui->testResult->setText("");
            return;
        }
        QSqlDatabase testDb = QSqlDatabase::addDatabase(c.driver, "kdevsqltest");
        testDb.setHostName(c.hostName);
        testDb.setUserName(c.userName);
        testDb.setPassword(c.password);
        testDb.setDatabaseName(c.databaseName);
        if (!testDb.open()) {
            ui->testResult->setText(i18n("<b>Connect failed:</b><br />%1", QString(testDb.lastError().text()).toHtmlEscaped()));
        } else {
            ui->testResult->setText(i18n("Successfully connected."));
        }
        testDb.close();
    }
    QSqlDatabase::removeDatabase("kdevsqltest");
}

}
