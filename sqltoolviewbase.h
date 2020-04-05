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

#ifndef SQLTOOLVIEWBASE_H
#define SQLTOOLVIEWBASE_H

#include <QAction>
#include <QList>
#include <QComboBox>

#include "connections/connectionsallprojectsmodel.h"

namespace Sql
{

/**
 * @todo write docs
 */
class SqlToolViewBase : public QWidget
{
    Q_OBJECT
public:
    /**
     * Default constructor
     */
    SqlToolViewBase(QComboBox *connection, QWidget *parent = 0);

    /**
     * Destructor
     */
    virtual ~SqlToolViewBase();

    virtual QList<QAction*> toolbarActions() const;

public slots:
    virtual void connectionChanged();
    virtual void currentConnectionChanged(int index) = 0;

protected:
    ConnectionsAllProjectsModel *m_connectionsModel;
    QComboBox *m_connection;
    QList<QAction*> m_toolbarActions;
};

}

#endif // SQLTOOLVIEWBASE_H
