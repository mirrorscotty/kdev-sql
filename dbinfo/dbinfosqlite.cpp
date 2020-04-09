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

#include "dbinfosqlite.h"
#include <QSqlQuery>
#include <QSqlField>
#include <QDebug>

namespace Sql
{

#define SQL(...) #__VA_ARGS__

DbInfoSqlite::DbInfoSqlite(QSqlDatabase *database)
    : DbInfoBase(database)
{
}

QVector<QSqlIndex> DbInfoSqlite::getIndexes(QString tableName, QVector<QSqlField> allColumns)
{
    QSqlQuery listQuery(*db), indexQuery(*db);
    listQuery.prepare("pragma index_list(" + tableName + ")");
    listQuery.exec();

    QString indexName, columnString;
    QSqlIndex index;
    QVector<QSqlIndex> allIndexes;
    QSqlField column;

    if(allColumns.isEmpty())
        allColumns = this->getColumns(tableName);

    while(listQuery.next()) {
        indexName = listQuery.value(1).toString();
        index = QSqlIndex("asdf", indexName);
        qDebug() << "Index Name" << indexName;

        indexQuery.prepare("pragma index_info(" + indexName + ")");
        indexQuery.exec();
        while(indexQuery.next()) {
            columnString = indexQuery.value(2).toString();
            qDebug() << "Column" << columnString;
            column = findColumnInList(columnString, allColumns );
            index.append(column);
        }
        indexQuery.finish();
        allIndexes.append(index);
    }
    return allIndexes;
}

QVector<QSqlField> DbInfoSqlite::getPrimaryKeyColumns(QString tableName, QVector<QSqlField> allColumns)
{
    QSqlQuery query(*db);
    query.prepare("pragma table_info(" + tableName + ")");
    query.exec();
    QVector<QSqlField> pkColumns;
    QSqlField column;
    QString columnName;

    if(allColumns.isEmpty())
        allColumns = getColumns(tableName);

    while(query.next()) {
        if(query.record().value(5).toInt() != 1)
            continue;
        columnName = query.record().value(1).toString();
        column = findColumnInList(columnName, allColumns);
        pkColumns.append(column);
    }
    return pkColumns;
}

QVector<QSqlField> DbInfoSqlite::getForeignKeyColumns(QString tableName, QVector<QSqlField> allColumns)
{
    QSqlQuery query(*db);
    query.prepare("pragma foreign_key_list(" + tableName + ")");
    query.exec();
    QVector<QSqlField> fkColumns;
    QSqlField column;
    QString columnName;

    if(allColumns.isEmpty())
        allColumns = getColumns(tableName);

    while(query.next()) {
        columnName = query.record().value(3).toString();
        column = findColumnInList(columnName, allColumns);
        fkColumns.append(column);
    }
    return fkColumns;
}

}
