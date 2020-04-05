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

#include <QVariant>
#include <QString>
#include <klocalizedstring.h>

#include "dbschematable.h"
#include "dbschemaitem.h"

namespace Sql
{

DbSchemaTable::DbSchemaTable(QString tableName, QSql::TableType type, DbSchemaItem* parentItem)
{
    name = tableName;
    tableType = type;
    parent = parentItem;
}

QVariant DbSchemaTable::data(int column) const
{
    if(column == 0) {
        return name;
    } else if(column == 1) {
        switch (tableType) {
            case QSql::TableType::Views:
                return i18n("View");
            case QSql::TableType::Tables:
                return i18n("Table");
            case QSql::TableType::SystemTables:
                return i18n("System Table");
            default:
                return i18n("Unknown");
        }
    }
    return QVariant();
}

QIcon DbSchemaTable::icon (int column) const
{
    if(column == 0)
        return QIcon::fromTheme("table");
    return QIcon();
}

}
