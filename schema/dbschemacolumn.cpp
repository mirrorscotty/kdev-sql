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
#include <QMetaType>
#include <QSqlField>
#include <klocalizedstring.h>

#include "dbschemacolumn.h"
#include "dbschemaitem.h"

namespace Sql
{

DbSchemaColumn::DbSchemaColumn(QSqlField column, DbSchemaItem* parentItem)
{
    name = column.name();
    typeId = column.type();
    parent = parentItem;
}

QVariant DbSchemaColumn::data(int column) const
{
    if(column == 0) {
        return name;
    } else if(column == 1) {
        QString(QMetaType::typeName(typeId));
    }
    return QVariant();
}

QIcon DbSchemaColumn::icon (int column) const
{
    if(column == 0)
        return QIcon::fromTheme("object-columns");
    return QIcon();
}

}
