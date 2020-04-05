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

#ifndef SQL_DBSCHEMAITEM_H
#define SQL_DBSCHEMAITEM_H

#include <QString>
#include <QSqlField>
#include <QVector>
#include <QIcon>

namespace Sql
{

enum TableType { VIEW, TABLE, SYSTEM_TABLE };

/**
 * @todo write docs
 */
class DbSchemaItem
{
public:
    /**
     * Default constructor -- root item only!
     */
    DbSchemaItem();

    /**
     * Add a child item to this node in the tree
     * @param child The item to add
     */
    void appendChild(DbSchemaItem *child);
    /**
     * Get the child at the specified row number
     * @param row Row number
     * @returns Pointer to the child item
     */
    DbSchemaItem *child(int row) const;
    /**
     * Get the number of children for this item
     * @returns Count of children
     */
    int childCount() const;
    /**
     * Get the number of columns for this item
     * @returns Count of columns
     */
    int columnCount() const;
    /**
     * Get the value of the specified column for this item
     * @param column Column number
     * @returns Value
     */
    virtual QVariant data(int column) const;

    /* TODO: Write docs! */
    virtual QIcon icon(int column) const;
    /**
     * Get the row number of this item
     * @returns Row number
     */
    int row() const;
    /**
     * Get this item's parent
     * @returns Pointer to parent item
     */
    DbSchemaItem *parentItem() const;

    /**
     * Destructor
     */
    virtual ~DbSchemaItem();

protected:
    QString name;
    QString dataType;

    DbSchemaItem *parent;
    QVector<DbSchemaItem*> children;

};

}

#endif // SQL_DBSCHEMAITEM_H
