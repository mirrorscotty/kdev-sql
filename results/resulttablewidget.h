/*
   Copyright (C) 2010 Niko Sams <niko.sams@gmail.com>

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

#ifndef SQL_RESULTTABLEWIDGET_H
#define SQL_RESULTTABLEWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QSqlDatabase>
#include "connections/connectionsmodel.h"
#include "sqltoolviewbase.h"

namespace Ui {
    class Results;
}

class QTableView;
class QSqlQueryModel;

namespace Sql {

class ConnectionsAllProjectsModel;
class QueryWorker;

class ResultTableWidget : public SqlToolViewBase
{
    Q_OBJECT
public:
    ResultTableWidget(QComboBox *connection, QWidget* parent = 0);
    ~ResultTableWidget();

    void runSql(QString sql);

signals:
    void newConnection(Connection c);

private slots:
    virtual void currentConnectionChanged(int index) override;
    void results(QSqlQuery query, int elapsedTime);
    void error(const QString &errorText);

private:
    Ui::Results *m_ui;
    QSqlQueryModel *m_model;
    QueryWorker* m_queryWorker;
};

}

#endif
