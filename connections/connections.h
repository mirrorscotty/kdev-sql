/*
    Copyright (C) 2010 Niko Sams <niko.sams@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#ifndef PROJECTCONFIGPAGE_H
#define PROJECTCONFIGPAGE_H

#include <interfaces/configpage.h>
#include <project/projectconfigpage.h>
#include <ui_connections.h>
#include <KConfigGroup>

namespace KDevelop
{
class IProject;
}

namespace Ui
{
class Connections;
}


class QModelIndex;

namespace Sql
{

class ConnectionsModel;

class ProjectConfigPage : public KDevelop::ConfigPage
{
    Q_OBJECT

public:
    ProjectConfigPage(KDevelop::IPlugin *plugin,
            const KDevelop::ProjectConfigOptions &options,
            QWidget *parent);
    ~ProjectConfigPage() override;

    QString name() const override;
    QIcon icon() const override;

    void apply() override;
    void reset() override;

private slots:
    void currentRowChanged(const QModelIndex& index);
    void connectionEdited();
    void testConnection();
    void setFileName();

    void setFileMode();
    void setNetworkMode();
    void setModeForDriver(int index);

private:
    Ui_Connections *ui;
    ConnectionsModel *conn;
    KDevelop::IProject *project;
};

}

#endif // PROJECTCONFIGPAGE_H
