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

#ifndef SQL_DBSCHEMACOLUMN_H
#define SQL_DBSCHEMACOLUMN_H

#include <QString>
#include <QSqlField>
#include "dbschemaitem.h"

namespace Sql
{

/**
 * @todo write docs
 */
class DbSchemaColumn:  public DbSchemaItem
{
public:
    /**
     * Create an item from a column
     * @param column The column to use
     * @param parent The item for the parent table
     */
    DbSchemaColumn(QSqlField column, DbSchemaItem *parent = nullptr);
    virtual QVariant data(int column) const override;
    virtual QIcon icon(int column) const override;

protected:
    QSql::TableType tableType;
    int typeId;
};

}

#endif // SQL_DBSCHEMACOLUMN_H
