#ifndef SQL_CONNECTIONSALLPROJECTSMODEL_H
#define SQL_CONNECTIONSALLPROJECTSMODEL_H
#include <QAbstractListModel>

namespace KDevelop
{
    class IProject;
}

namespace Sql
{

struct Connection;
class ConnectionsModel;

class ConnectionsAllProjectsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    ConnectionsAllProjectsModel(QObject *parent);
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data ( const QModelIndex& index, int role = Qt::DisplayRole ) const override;
    Connection connection(int row);

private slots:
    void projectOpened(KDevelop::IProject* project);
    void childModelReset();
    void projectClosed(KDevelop::IProject* project);
    void reloadModels();

private:
    QHash<KDevelop::IProject*, ConnectionsModel*> m_models;
};

}

#endif //SQL_CONNECTIONSALLPROJECTSMODEL_H
