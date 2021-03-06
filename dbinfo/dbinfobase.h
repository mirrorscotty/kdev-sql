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

#ifndef SQL_DBINFOBASE_H
#define SQL_DBINFOBASE_H

#include <QVector>
#include <QSqlField>
#include <QSqlIndex>
#include <QSqlDatabase>

namespace Sql
{

QSqlField findColumnInList(QString, QVector<QSqlField>);
/**
 * @todo write docs
 */
class DbInfoBase
{
public:
    /**
     * Default constructor
     */
    DbInfoBase(QSqlDatabase *database);

    /**
     * Destructor
     */
    virtual ~DbInfoBase();

    virtual QStringList getTables(QSql::TableType tableType);
    virtual QVector<QSqlField> getColumns(QString tableName);
    virtual QVector<QSqlIndex> getIndexes(QString tableName, QVector<QSqlField> allColumns = QVector<QSqlField>());
    virtual QVector<QSqlField> getPrimaryKeyColumns(QString tableName, QVector<QSqlField> allColumns = QVector<QSqlField>());
    virtual QVector<QSqlField> getForeignKeyColumns(QString tableName, QVector<QSqlField> allColumns = QVector<QSqlField>());

protected:
    QSqlDatabase *db;

};

}

#endif // DBINFOBASE_H
