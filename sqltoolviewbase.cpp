/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2020  Alex Griessman <alex.griessman@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "sqltoolviewbase.h"
#include <QWidget>

namespace Sql
{

SqlToolViewBase::SqlToolViewBase(QComboBox *connection, QWidget *parent)
    : QWidget(parent), m_connection(connection)
{
    Q_ASSERT(connection);
    m_connectionsModel = static_cast<ConnectionsAllProjectsModel*>(connection->model());

    connect(m_connection, SIGNAL(currentIndexChanged(int)), SLOT(currentConnectionChanged(int)));
    connect(m_connectionsModel, SIGNAL(modelReset()), SLOT(connectionChanged()));
    connect(m_connectionsModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(connectionChanged()));
}

SqlToolViewBase::~SqlToolViewBase()
{
}

void SqlToolViewBase::connectionChanged()
{
    currentConnectionChanged(m_connection->currentIndex());
}

QList<QAction*> SqlToolViewBase::toolbarActions() const
{
    return m_toolbarActions;
}

}

#include "moc_sqltoolviewbase.cpp"
