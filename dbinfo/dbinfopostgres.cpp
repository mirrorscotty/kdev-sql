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

QVector<QSqlIndex> DbInfoPostgres::getIndexes(QString tableName, QVector<QSqlField> allColumns)
{
    QSqlQuery query(*db);
    //Source: https://stackoverflow.com/questions/2204058/list-columns-with-indexes-in-postgresql
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
    QVector<QSqlIndex> allIndexes;
    QSqlField column;

    if(allColumns.isEmpty())
        allColumns = getColumns(tableName);

    while(query.next()) {
        indexName = query.value(1).toString();
        columnList = query.value(2).toString().split('|');

        // Not sure what the cursor name is for. It's probably not important.
        index = QSqlIndex("asdf", indexName);

        for(int i = 0; i< columnList.count(); i++) {
            column = findColumnInList(columnList[i], allColumns );
            index.append(column);
        }
        allIndexes.append(index);
    }
    return allIndexes;
}

QVector<QSqlField> DbInfoPostgres::getPrimaryKeyColumns(QString tableName, QVector<QSqlField> allColumns)
{
    QSqlQuery query(*db);
    query.prepare(SQL(
        SELECT a.attname
        FROM   pg_index i
        JOIN   pg_attribute a ON a.attrelid = i.indrelid
                            AND a.attnum = ANY(i.indkey)
        WHERE  i.indrelid = cast(:table_name as regclass)
        AND    i.indisprimary;
    ));
    query.bindValue(":table_name", tableName);
    query.exec();
    QVector<QSqlField> pkColumns;
    QSqlField column;
    QString columnName;

    if(allColumns.isEmpty())
        allColumns = getColumns(tableName);

    while(query.next()) {
        columnName = query.record().value(0).toString();
        column = findColumnInList(columnName, allColumns);
        pkColumns.append(column);
    }
    return pkColumns;
}

QVector<QSqlField> DbInfoPostgres::getForeignKeyColumns(QString tableName, QVector<QSqlField> allColumns)
{
    QSqlQuery query(*db);
    // Source: https://dataedo.com/kb/query/postgresql/list-of-foreign-keys-with-columns
    query.prepare(SQL(
        select kcu.table_schema as foreign_table_schema,
            kcu.table_name as foreign_table,
            rel_kcu.table_schema as primary_table_schema,
            rel_kcu.table_name as primary_table,
            kcu.ordinal_position as no,
            kcu.column_name as fk_column,
            rel_kcu.column_name as pk_column,
            kcu.constraint_name
        from information_schema.table_constraints tco
        join information_schema.key_column_usage kcu
                on tco.constraint_schema = kcu.constraint_schema
                and tco.constraint_name = kcu.constraint_name
        join information_schema.referential_constraints rco
                on tco.constraint_schema = rco.constraint_schema
                and tco.constraint_name = rco.constraint_name
        join information_schema.key_column_usage rel_kcu
                on rco.unique_constraint_schema = rel_kcu.constraint_schema
                and rco.unique_constraint_name = rel_kcu.constraint_name
                and kcu.ordinal_position = rel_kcu.ordinal_position
        where tco.constraint_type = 'FOREIGN KEY' and kcu.table_name = :table_name
        order by kcu.table_schema,
                kcu.table_name,
                kcu.ordinal_position;
    ));
    query.bindValue(":table_name", tableName);
    query.exec();
    QVector<QSqlField> fkColumns;
    QSqlField column;
    QString columnName;

    if(allColumns.isEmpty())
        allColumns = getColumns(tableName);

    while(query.next()) {
        columnName = query.record().value(5).toString();
        column = findColumnInList(columnName, allColumns);
        fkColumns.append(column);
    }
    return fkColumns;
}

}
