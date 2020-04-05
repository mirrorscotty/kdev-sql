/*
   Copyright (C) 2010 Niko Sams <niko.sams@gmail.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "sqllanguagesupport.h"
#include "connections/connections.h"

#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTableView>

#include <KPluginFactory>
#include <KPluginLoader>
#include <KAboutData>
#include <KActionCollection>
#include <QAction>
#include <KTextEditor/Document>
#include <klocalizedstring.h>

#include <interfaces/iuicontroller.h>
#include <interfaces/icore.h>
#include <interfaces/idocumentcontroller.h>

#include "results/resulttablewidget.h"
#include "schema/schemabrowserwidget.h"
#include "connections/connectionsallprojectsmodel.h"
//#include "version.h"

/*
K_PLUGIN_FACTORY(KDevSqlSupportFactory, registerPlugin<Sql::LanguageSupport>();)
K_EXPORT_PLUGIN(KDevSqlSupportFactory(KAboutData("kdevsqlsupport","kdevsql", ki18n("SQL Support"), VERSION_STR, ki18n("Support for SQL Language"), KAboutData::License_GPL)
    .addAuthor(ki18n("Niko Sams"), ki18n("Author"), "niko.sams@gmail.com", "http://nikosams.blogspot.com")
))
*/
K_PLUGIN_FACTORY_WITH_JSON(KDevSqlSupportFactory,
                           "sqllanguagesupport.json",
                           registerPlugin<Sql::LanguageSupport>();
                          )

namespace Sql
{
LanguageSupport* LanguageSupport::m_self = 0;

// FIXME //int debugArea() { static int s_area = KDebug::registerArea("kdevsqlsupport"); return s_area; }

template<class T>
class ToolFactory : public KDevelop::IToolViewFactory
{
public:
  ToolFactory(const QString &id, Qt::DockWidgetArea defaultArea, QComboBox *selectConnection = nullptr)
  : m_id(id), m_defaultArea(defaultArea), m_connection(selectConnection)
  {}

  virtual QWidget* create(QWidget *parent = 0) override
  {
    T *toolView = new T(m_connection, parent);
    return toolView;
  }

  virtual QString id() const override
  {
    return m_id;
  }

  virtual Qt::DockWidgetArea defaultPosition() override
  {
    return m_defaultArea;
  }

  virtual bool allowMultiple() const override
  {
    return false;
  }

  virtual QList<QAction *>toolBarActions(QWidget *viewWidget) const override
  {
    SqlToolViewBase *view = static_cast<SqlToolViewBase*>(viewWidget);
    return view->toolbarActions();
  }

private:
  QString m_id;
  Qt::DockWidgetArea m_defaultArea;
  QComboBox *m_connection;
  QList<QAction*> m_toolbarActions;
};

#define RESULTS_VIEW_NAME i18n("SQL Query")
#define SCHEMA_VIEW_NAME i18n("Schema Browser")

LanguageSupport::LanguageSupport(QObject* parent, const QVariantList& /*args*/)
    : KDevelop::IPlugin("SQL Support", parent),
      KDevelop::ILanguageSupport()
{
    setComponentName("kdevsqlsupport", "SQL Support");
    setXMLFile("kdevsqlui.rc");

    // Buttons/widgets for the toolbar
    KActionCollection* ac = actionCollection();

    QAction * action = new QAction(QIcon::fromTheme("system-run"), i18n("Run SQL"), this);
    connect(action, SIGNAL(triggered(bool)), SLOT(runSql()));
    ac->addAction("run_sql", action);
    ac->setDefaultShortcut(action, Qt::CTRL + Qt::Key_E);

    databaseComboBox = new QComboBox();
    databaseComboBox->setModel(new ConnectionsAllProjectsModel(this));
    databaseComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    QWidgetAction *selectConnection = new QWidgetAction(this);
    selectConnection->setDefaultWidget(databaseComboBox);
    ac->addAction("select_connection", selectConnection);

    // Set up the tool views
    m_self = this;
    m_resultTableFactory = new ToolFactory<ResultTableWidget>(
        "org.kdevelop.sql.ResultTable", Qt::BottomDockWidgetArea, databaseComboBox);
    core()->uiController()->addToolView(RESULTS_VIEW_NAME ,m_resultTableFactory);
    m_schemaBrowserFactory = new ToolFactory<SchemaBrowserWidget>(
        "org.kdevelop.sql.SchemaBrowser", Qt::RightDockWidgetArea, databaseComboBox);
    core()->uiController()->addToolView(SCHEMA_VIEW_NAME, m_schemaBrowserFactory);
}

QString LanguageSupport::name() const
{
    return "Sql";
}

LanguageSupport *LanguageSupport::self()
{
    return m_self;
}

void LanguageSupport::runSql()
{
    QWidget* w = core()->uiController()->findToolView(RESULTS_VIEW_NAME, m_resultTableFactory, KDevelop::IUiController::CreateAndRaise);
    Q_ASSERT(w);
    ResultTableWidget* resTable = dynamic_cast<ResultTableWidget*>(w);
    Q_ASSERT(resTable);
    KTextEditor::Range selection = core()->documentController()->activeDocument()->textSelection();
    QString text;
    if (selection.isEmpty()) {
        text = core()->documentController()->activeDocument()->textDocument()->text();
    } else {
        text = core()->documentController()->activeDocument()->textDocument()->text(selection);
    }
    resTable->runSql(text);
}

int LanguageSupport::perProjectConfigPages() const
{
    return 1;
}

KDevelop::ConfigPage* LanguageSupport::perProjectConfigPage(int number,
        const KDevelop::ProjectConfigOptions& options,
        QWidget *parent)
{
    if(number==0) {
        return new ProjectConfigPage(this, options, parent);
    }
    return nullptr;
}

}

#include "sqllanguagesupport.moc"
