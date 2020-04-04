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

#include "dbinfobase.h"

namespace Sql
{


QSqlField findColumnInList(QString columnName, QVector<QSqlField> allColumns)
{
    int i;
    for(i=0; i<allColumns.count(); i++) {
        if(columnName == allColumns[i].name()) {
            return allColumns[i];
        }
    }
    return QSqlField();
}

DbInfoBase::DbInfoBase(QSqlDatabase *database)
{
    db = database;
    if(!db->isOpen()) {
        db->open();
    }
}

DbInfoBase::~DbInfoBase()
{
}

QStringList DbInfoBase::getTables(QSql::TableType tableType)
{
    return db->tables(tableType);
}

QVector<QSqlField> DbInfoBase::getColumns(QString tableName)
{
    QVector<QSqlField> allColumns;
    QSqlRecord record;
    record = db->record(tableName);

    QSqlField column;
    for(int i = 0; i < record.count(); i++) {
        column = record.field(i);
        allColumns.append(column);
    }
    return allColumns;
}

QVector<QSqlIndex> DbInfoBase::getIndexes(QString tableName)
{
    QVector<QSqlIndex> allIndexes;
    QSqlIndex primaryKey;
    primaryKey = db->primaryIndex(tableName);
    if(!primaryKey.name().isNull()) {
        allIndexes.append(db->primaryIndex(tableName));
    }
    return allIndexes;
}

}
