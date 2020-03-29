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

#include <QAbstractItemModel>
#include <QSqlRecord>
#include <QDebug>

namespace Sql
{

DbSchemaModel::DbSchemaModel(QSqlDatabase *database, QObject *parent)
    : QAbstractItemModel(parent)
{
    db = database;
    rootItem = nullptr;
    refreshModelData();
}

DbSchemaModel::~DbSchemaModel()
{
    delete rootItem;
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
    if(rootItem) {
        delete rootItem;
    }
    rootItem = new DbSchemaItem();

    QStringList tables;
    loadTablesFromDb(QSql::TableType::Tables);
    loadTablesFromDb(QSql::TableType::Views);
    loadTablesFromDb(QSql::TableType::SystemTables);

    for(int i = 0; i < rootItem->childCount(); i++) {
        loadColumnsForTable(rootItem->child(i));
    }
}

void DbSchemaModel::loadTablesFromDb(QSql::TableType tableType)
{
    QStringList tables = db->tables(tableType);
    DbSchemaItem *tableItem;
    for(int i = 0; i < tables.count(); i++) {
        tableItem = new DbSchemaItem(tables.at(i), tableType);
        rootItem->appendChild(tableItem);
    }
}

void DbSchemaModel::loadColumnsForTable(DbSchemaItem *tableItem)
{
    QString tableName;
    QSqlRecord record;
    DbSchemaItem *columnItem;
    tableName = tableItem->data(0).toString();
    record = db->record(tableName);

    QSqlField column;
    for(int i = 0; i < record.count(); i++) {
        column = record.field(i);
        columnItem = new DbSchemaItem(column, tableItem);
        tableItem->appendChild(columnItem);
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
