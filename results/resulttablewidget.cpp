/*
   Copyright (C) 2010,2012 Niko Sams <niko.sams@gmail.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/


#include "resulttablewidget.h"

#include <qtextdocument.h>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QTimer>

#include <QDebug>
#include <KComponentData>
#include "kcomponentdata.h"
#include <KSettings/Dispatcher>
#include <klocalizedstring.h>

#include <interfaces/icore.h>
#include <interfaces/iprojectcontroller.h>
#include <interfaces/iproject.h>

#include "connections/connectionsmodel.h"
#include "connections/connectionsallprojectsmodel.h"
#include "queryworker.h"
#include "ui_results.h"


namespace Sql {

ResultTableWidget::ResultTableWidget(QWidget* parent)
    : QWidget(parent), m_queryWorker(0)
{
    m_ui = new Ui::Results;
    m_ui->setupUi(this);
    m_model =  new QSqlQueryModel(this);
    m_ui->table->setModel(m_model);
    m_connectionsModel = new ConnectionsAllProjectsModel(this);
    m_ui->connection->setModel(m_connectionsModel);
    connect(m_ui->connection, SIGNAL(currentIndexChanged(int)), SLOT(currentConnectionChanged(int)));
    connect(m_connectionsModel, SIGNAL(modelReset()), SLOT(connectionChanged()));
    connect(m_connectionsModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(connectionChanged()));

    setWindowIcon(QIcon::fromTheme("server-database"));
}

ResultTableWidget::~ResultTableWidget()
{
    delete m_ui;
    if(m_queryWorker) {
        m_queryWorker->quit();
        m_queryWorker->wait();
        delete m_queryWorker;
    }
    delete m_connectionsModel;
    delete m_model;
}

void ResultTableWidget::currentConnectionChanged(int index)
{
    if (!m_queryWorker) {
        m_queryWorker = new QueryWorker();
        //QThread *queryThread = new QThread(this);
        connect(m_queryWorker, SIGNAL(results(QSqlQuery, int)), SLOT(results(QSqlQuery, int)));
        connect(m_queryWorker, SIGNAL(error(QString)), SLOT(error(QString)));
        //m_queryWorker->moveToThread(queryThread);
        //queryThread->start();
        m_queryWorker->start();
    }

    qDebug() << index;
    if (index != -1) {
        m_ui->messageLabel->setText(QString(""));

        Connection c = m_connectionsModel->connection(index);
        qRegisterMetaType<Connection>("Connection");
        QMetaObject::invokeMethod(m_queryWorker, "changeDatabaseConnection", Qt::QueuedConnection, Q_ARG(Connection, c));

        emit newConnection(c);
    } else {
        if (!m_connectionsModel->rowCount()) {
            m_ui->messageLabel->setText(i18n("No Database Connection available. Open Project Configuration for to create one."));
        } else {
            m_ui->messageLabel->setText(i18n("No Database Connection selected."));
        }
    }

    m_ui->stackedWidget->setCurrentWidget(m_ui->messagePage);
}

void ResultTableWidget::connectionChanged()
{
    currentConnectionChanged(m_ui->connection->currentIndex());
}


void ResultTableWidget::runSql(QString sql)
{
    if (!m_queryWorker) currentConnectionChanged(m_ui->connection->currentIndex());

    m_ui->messageLabel->setText(i18n("Executing Query..."));
    m_ui->stackedWidget->setCurrentWidget(m_ui->messagePage);

    qRegisterMetaType<QSqlDatabase>("QSqlDatabase");
    QMetaObject::invokeMethod(m_queryWorker, "execute", Qt::QueuedConnection, Q_ARG(QString, sql));
}

void ResultTableWidget::error(const QString& errorText)
{
    m_ui->messageLabel->setText(QString(errorText).toHtmlEscaped());
    m_ui->stackedWidget->setCurrentWidget(m_ui->messagePage);
}

void ResultTableWidget::results(QSqlQuery query, int elapsedTime)
{
    m_model->setQuery(query);

    m_ui->durationLabel->setText(i18n("Query finished in %1 ms", QString::number(elapsedTime)));
    m_ui->stackedWidget->setCurrentWidget(m_ui->resultsPage);
}

}

#include "moc_resulttablewidget.cpp"
