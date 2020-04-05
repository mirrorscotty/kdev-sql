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

#include "dbschemaitem.h"
#include <QVariant>
#include <QMetaType>
#include <klocalizedstring.h>
#include <QIcon>

namespace Sql
{

DbSchemaItem::DbSchemaItem() : parent(nullptr)
{
    name = i18n("Name");
    dataType = i18n("Type");
}

DbSchemaItem::~DbSchemaItem()
{
    qDeleteAll(children);
}

void DbSchemaItem::appendChild(DbSchemaItem* child)
{
    children.append(child);
}

int DbSchemaItem::childCount() const
{
    return children.count();
}

DbSchemaItem* DbSchemaItem::child ( int row ) const
{
    if (row < 0 || row >= children.count()) {
        return nullptr;
    }
    return children.at(row);
}

int DbSchemaItem::columnCount() const
{
    return 2;
}

QVariant DbSchemaItem::data(int column) const
{
    switch (column) {
        case 0:
            return name;
        case 1:
            return dataType;
        default:
            return QVariant();
    }
}

int DbSchemaItem::row() const
{
    if (parent) {
        return parent->children.indexOf(const_cast<DbSchemaItem*>(this));
    } else {
        return 0;
    }
}

QIcon DbSchemaItem::icon(int column) const
{
    column = column == 0; // Warnings are annoying.
    return QIcon();
}


DbSchemaItem *DbSchemaItem::parentItem() const
{
    return parent;
}

}
