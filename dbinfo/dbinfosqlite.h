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

#ifndef DBINFOSQLITE_H
#define DBINFOSQLITE_H

#include "dbinfo/dbinfobase.h"

namespace Sql
{
/**
 * @todo write docs
 */
class DbInfoSqlite : public DbInfoBase
{
public:
    DbInfoSqlite(QSqlDatabase *database);
    QVector<QSqlIndex> getIndexes(QString tableName, QVector<QSqlField> allColumns = QVector<QSqlField>()) override;
    QVector<QSqlField> getPrimaryKeyColumns(QString tableName, QVector<QSqlField> allColumns = QVector<QSqlField>()) override;
    QVector<QSqlField> getForeignKeyColumns(QString tableName, QVector<QSqlField> allColumns = QVector<QSqlField>()) override;
};
}

#endif // DBINFOSQLITE_H
