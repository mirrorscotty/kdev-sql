#include "results/resulttablewidget.h"
#include "connectionsmodel.h"
#include "connectionsallprojectsmodel.h"

#include <KSettings/Dispatcher>
#include <interfaces/icore.h>
#include <interfaces/iprojectcontroller.h>
#include <interfaces/iproject.h>
#include <QObject>
#include <QAbstractListModel>

namespace Sql
{

ConnectionsAllProjectsModel::ConnectionsAllProjectsModel(QObject *parent)
    : QAbstractListModel(parent)
{
    connect(KDevelop::ICore::self()->projectController(), SIGNAL(projectOpened(KDevelop::IProject*)),
            SLOT(projectOpened(KDevelop::IProject*)));
    connect(KDevelop::ICore::self()->projectController(), SIGNAL(projectClosed(KDevelop::IProject*)),
            SLOT(projectClosed(KDevelop::IProject*)));

    foreach (KDevelop::IProject *p, KDevelop::ICore::self()->projectController()->projects())
        projectOpened(p);

    KSettings::Dispatcher::registerComponent("kdevplatformproject", this, "reloadModels");
}

int ConnectionsAllProjectsModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;
    int ret = 0;
    foreach (ConnectionsModel *m, m_models) {
        ret += m->rowCount()-1;
    }
    return ret;
}

QVariant ConnectionsAllProjectsModel::data(const QModelIndex& index, int role) const
{
    if (index.parent().isValid()) return QVariant();
    if (index.column() > 0) return QVariant();
    if (role != Qt::DisplayRole) return QVariant();
    int i = 0;
    QHashIterator<KDevelop::IProject*, ConnectionsModel*> it(m_models);
    while (it.hasNext()) {
        it.next();
        ConnectionsModel *m = it.value();
        KDevelop::IProject *p = it.key();
        if (index.row() < i + m->rowCount()-1) {
            return p->name() + ": " + m->data(m->index(index.row()-i, 0), role).toString();
        }
        i += m->rowCount()-1;
    }
    Q_ASSERT(0);
    return QVariant();
}

Connection ConnectionsAllProjectsModel::connection(int row)
{
    int i = 0;
    foreach (ConnectionsModel *m, m_models) {
        if (row < i + m->rowCount()-1) {
            return m->connection(row - i);
        }
        i += m->rowCount()-1;
    }
    Q_ASSERT(0);
    return Connection();
}

void ConnectionsAllProjectsModel::projectOpened(KDevelop::IProject* project)
{
    beginResetModel();
    m_models[project] = new ConnectionsModel(project, this);
    connect(m_models[project], SIGNAL(modelReset()), SLOT(childModelReset()));
    endResetModel();
}

void ConnectionsAllProjectsModel::childModelReset()
{
    beginResetModel();
    endResetModel();
}

void ConnectionsAllProjectsModel::projectClosed(KDevelop::IProject* project)
{
    beginResetModel();
    delete m_models[project];
    m_models.remove(project);
    endResetModel();
}

void ConnectionsAllProjectsModel::reloadModels()
{
    foreach (ConnectionsModel *m, m_models) {
        m->revert();
    }
}

}
