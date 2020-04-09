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

#ifndef DBSCHEMAMODEL_H
#define DBSCHEMAMODEL_H

#include <QModelIndex>
#include <QSqlDatabase>
#include "dbschemaitem.h"

namespace Sql
{

class DbInfoBase;

/**
 * @todo write docs
 */
class DbSchemaModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    /**
     * Default constructor
     */
    DbSchemaModel(QSqlDatabase *database, QObject *parent = nullptr);

    /**
     * Destructor
     */
    ~DbSchemaModel();

    /**
     * @todo write docs
     *
     * @param index TODO
     * @param role TODO
     * @return TODO
     */
    QVariant data ( const QModelIndex& index, int role ) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;


    /**
     * @todo write docs
     *
     * @param parent TODO
     * @return TODO
     */
    int columnCount ( const QModelIndex& parent ) const override;

    /**
     * @todo write docs
     *
     * @param parent TODO
     * @return TODO
     */
    int rowCount ( const QModelIndex& parent ) const override;

    /**
     * @todo write docs
     *
     * @param child TODO
     * @return TODO
     */
    QModelIndex parent ( const QModelIndex& child ) const override;

    /**
     * @todo write docs
     *
     * @param row TODO
     * @param column TODO
     * @param parent TODO
     * @return TODO
     */
    QModelIndex index ( int row, int column, const QModelIndex& parent ) const override;

    /**
     * @todo write docs
     *
     * @param section TODO
     * @param orientation TODO
     * @param role TODO
     * @return TODO
     */
    QVariant headerData ( int section, Qt::Orientation orientation, int role ) const override;

public slots:
    void refreshModelData();

private:
    //QSqlDatabase *db;
    DbInfoBase *dbInfo;
    DbSchemaItem *rootItem;

    void loadTablesFromDb(QSql::TableType type, DbSchemaItem *parent = nullptr);
    QVector<QSqlField> loadColumnsForTable(DbSchemaItem *tableItem);
    void loadIndexesForTable(DbSchemaItem *tableItem, QVector<QSqlField> allColumns);
};

}

#endif // DBSCHEMAMODEL_H
