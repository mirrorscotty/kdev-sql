/*
   Copyright (C) 2012 Niko Sams <niko.sams@gmail.com>

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

#include "queryworker.h"
#include "connections/connectionsmodel.h"
#include <QMetaType>
#include <QTime>
#include <QSqlError>
#include <QDebug>

#define KDEV_SQL_DB_NAME "kdevsql"

namespace Sql {

QueryWorker::QueryWorker(QObject *parent)
    : QThread(parent)
{
}

QueryWorker::~QueryWorker()
{
    //QSqlDatabase::removeDatabase(KDEV_SQL_DB_NAME);
}

void QueryWorker::run()
{
    exec();
}

void QueryWorker::execute(const QString& query)
{
    QTime t;
    t.start();
    QSqlQuery sql(query, m_db);
    if(sql.isActive()) {
        int elapsed = t.elapsed();
        qRegisterMetaType<QSqlQuery>("QSqlQuery");
        emit results( sql, elapsed );
    } else {
        emit error( sql.lastError().databaseText() );
    }
}

void QueryWorker::changeDatabaseConnection(Connection c)
{
    if (QSqlDatabase::contains(KDEV_SQL_DB_NAME)) {
        if (m_db.isOpen()) m_db.close();
        m_db = QSqlDatabase();
        QSqlDatabase::removeDatabase(KDEV_SQL_DB_NAME);
    }

    m_db = QSqlDatabase::addDatabase(c.driver, KDEV_SQL_DB_NAME);
    m_db.setHostName(c.hostName);
    m_db.setUserName(c.userName);
    m_db.setPassword(c.password);
    m_db.setDatabaseName(c.databaseName);
    if (!m_db.open()) {
        emit error( m_db.lastError().text() );
    }
}

}

#include "moc_queryworker.cpp"
