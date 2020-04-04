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

#include "dbinfopostgres.h"
#include <QSqlQuery>

namespace Sql
{

#define SQL(...) #__VA_ARGS__

DbInfoPostgres::DbInfoPostgres(QSqlDatabase *database)
    : DbInfoBase(database)
{
}

QVector<QSqlIndex> DbInfoPostgres::getIndexes(QString tableName)
{
    QSqlQuery query(*db);
    query.prepare(SQL(
        select
            t.relname as table_name,
            i.relname as index_name,
            array_to_string(array_agg(a.attname), '|') as column_names
        from
            pg_class t,
            pg_class i,
            pg_index ix,
            pg_attribute a
        where
            t.oid = ix.indrelid
            and i.oid = ix.indexrelid
            and a.attrelid = t.oid
            and a.attnum = ANY(ix.indkey)
            and t.relkind = 'r'
            and t.relname = :table_name
        group by
            t.relname,
            i.relname
        order by
            t.relname,
            i.relname;
        ));
    query.bindValue(":table_name", tableName);
    query.exec();
    QString indexName, columnString;
    QStringList columnList;
    QSqlIndex index;
    QVector<QSqlField> tableColumns;
    QVector<QSqlIndex> allIndexes;
    QSqlField column;
    tableColumns = this->getColumns(tableName);

    while(query.next()) {
        indexName = query.value(1).toString();
        columnList = query.value(2).toString().split('|');

        // Not sure what the cursor name is for. It's probably not important.
        index = QSqlIndex("asdf", indexName);

        for(int i = 0; i< columnList.count(); i++) {
            column = findColumnInList(columnList[i], tableColumns);
            index.append(column);
        }
        allIndexes.append(index);
    }
    return allIndexes;
}

}
