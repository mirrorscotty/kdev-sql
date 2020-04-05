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
#include "dbschemafolder.h"
#include "dbschemaitem.h"

namespace Sql
{

DbSchemaFolder::DbSchemaFolder (QString folderName, DbSchemaItem* parentItem)
{
    name = folderName;
    parent = parentItem;
}

QVariant DbSchemaFolder::data(int column) const
{
    if(column == 0)
        return name;
    return QVariant();
}

QIcon DbSchemaFolder::icon (int column) const
{
    if(column == 0)
        return QIcon::fromTheme("folder");
    return QIcon();
}


}
