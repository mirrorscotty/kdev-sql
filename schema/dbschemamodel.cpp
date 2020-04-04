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

#include "dbschemamodel.h"
#include "dbinfo/dbinfobase.h"
#include "dbinfo/dbinfopostgres.h"
#include "dbinfo/dbinfosqlite.h"

#include <klocalizedstring.h>
#include <QAbstractItemModel>
#include <QSqlRecord>
#include <QSqlIndex>
#include <QDebug>

namespace Sql
{

DbSchemaModel::DbSchemaModel(QSqlDatabase *database, QObject *parent)
    : QAbstractItemModel(parent)
{
    qDebug() << "Driver:" << database->driverName();
    if(database->driverName() == QStringLiteral("QPSQL"))
        dbInfo = new DbInfoPostgres(database);
    else if(database->driverName() == QStringLiteral("QSQLITE"))
        dbInfo = new DbInfoSqlite(database);
    else
        dbInfo = new DbInfoBase(database);

    rootItem = nullptr;
    refreshModelData();
}

DbSchemaModel::~DbSchemaModel()
{
    delete rootItem;
    delete dbInfo;
}

QModelIndex DbSchemaModel::index ( int row, int column, const QModelIndex& parent ) const
{
    if(!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    DbSchemaItem *parentItem;
    if(!parent.isValid()) {
        parentItem = rootItem;
    } else {
        parentItem = static_cast<DbSchemaItem*>(parent.internalPointer());
    }

    DbSchemaItem *childItem;
    childItem = parentItem->child(row);
    if(childItem) {
        return createIndex(row, column, childItem);
    }
    return QModelIndex();
}

QModelIndex DbSchemaModel::parent ( const QModelIndex& child ) const
{
    if(!child.isValid()) {
        return QModelIndex();
    }

    DbSchemaItem *childItem, *parentItem;
    childItem = static_cast<DbSchemaItem*>(child.internalPointer());
    parentItem = childItem->parentItem();

    if(parentItem == rootItem) {
        return QModelIndex();
    }
    if(!parentItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int DbSchemaModel::rowCount ( const QModelIndex& parent ) const
{
    if(parent.column() > 0) {
        return 0;
    }

    DbSchemaItem *parentItem;
    if(parent.isValid()) {
        parentItem = static_cast<DbSchemaItem*>(parent.internalPointer());
    } else {
        parentItem = rootItem;
    }

    return parentItem->childCount();
}

int DbSchemaModel::columnCount ( const QModelIndex& parent ) const
{
    if(parent.isValid()) {
        return static_cast<DbSchemaItem*>(parent.internalPointer())->columnCount();
    } else {
        return rootItem->columnCount();
    }
}

QVariant DbSchemaModel::data ( const QModelIndex& index, int role ) const
{
    if(!index.isValid()) {
        return QModelIndex();
    }

    if(role != Qt::DisplayRole) {
        return QVariant();
    }

    DbSchemaItem *item;
    item = static_cast<DbSchemaItem*>(index.internalPointer());

    return item->data(index.column());
}

QVariant DbSchemaModel::headerData ( int section, Qt::Orientation orientation, int role ) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return rootItem->data(section);
    }
    return QVariant();
}

void DbSchemaModel::refreshModelData()
{
    beginResetModel();

    if(rootItem) {
        delete rootItem;
    }
    rootItem = new DbSchemaItem();
    DbSchemaItem *tablesFolder, *systemTablesFolder, *viewsFolder;
    tablesFolder = new DbSchemaItem(i18n("Tables"), rootItem);
    systemTablesFolder = new DbSchemaItem(i18n("System Tables"), tablesFolder);
    viewsFolder = new DbSchemaItem(i18n("Views"), rootItem);
    rootItem->appendChild(tablesFolder);
    tablesFolder->appendChild(systemTablesFolder);
    rootItem->appendChild(viewsFolder);

    loadTablesFromDb(QSql::TableType::Tables, tablesFolder);
    loadTablesFromDb(QSql::TableType::Views, viewsFolder);
    loadTablesFromDb(QSql::TableType::SystemTables, systemTablesFolder);

    endResetModel();
}

void DbSchemaModel::loadTablesFromDb(QSql::TableType tableType, DbSchemaItem *parent)
{
    if(!parent) {
        parent = rootItem;
    }

    QStringList tables = dbInfo->getTables(tableType);
    DbSchemaItem *tableItem;
    for(int i = 0; i < tables.count(); i++) {
        tableItem = new DbSchemaItem(tables.at(i), tableType, parent);
        parent->appendChild(tableItem);
        loadColumnsForTable(tableItem);
        loadIndexesForTable(tableItem);
    }
}

void DbSchemaModel::loadColumnsForTable(DbSchemaItem *tableItem)
{
    QString tableName = tableItem->data(0).toString();
    QVector<QSqlField> allColumns;
    DbSchemaItem *columnItem, *columnFolder;
    columnFolder = new DbSchemaItem("Columns", tableItem);
    tableItem->appendChild(columnFolder);

    allColumns = dbInfo->getColumns(tableName);

    QSqlField column;
    for(int i = 0; i < allColumns.count(); i++) {
        column = allColumns[i];
        columnItem = new DbSchemaItem(column, columnFolder);
        columnFolder->appendChild(columnItem);
    }
}

void DbSchemaModel::loadIndexesForTable(DbSchemaItem *tableItem)
{
    QString tableName = tableItem->data(0).toString();
    QVector<QSqlIndex> allIndexes;
    allIndexes = dbInfo->getIndexes(tableName);
    if(allIndexes.isEmpty()) {
        return;
    }

    DbSchemaItem *indexFolder, *indexItem, *columnItem;
    indexFolder = new DbSchemaItem("Indexes", tableItem);
    tableItem->appendChild(indexFolder);

    QString indexName;
    QSqlIndex index;
    for(int i = 0; i < allIndexes.count(); i++) {
        indexName = allIndexes[i].name();
        index = allIndexes[i];

        indexItem = new DbSchemaItem(indexName, indexFolder);
        indexFolder->appendChild(indexItem);
        for(int i = 0; i < index.count(); i++) {
            columnItem = new DbSchemaItem(index.field(i), indexItem);
            indexItem->appendChild(columnItem);
        }
    }
}

Qt::ItemFlags DbSchemaModel::flags ( const QModelIndex& index ) const
{
    if(!index.isValid()) {
        return Qt::NoItemFlags;
    }
    return QAbstractItemModel::flags(index);
}

}
