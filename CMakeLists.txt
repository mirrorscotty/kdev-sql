PROJECT(sql)
include(CMakePrintHelpers)

set(VERSION_MAJOR 5)
set(VERSION_MINOR 2)
set(VERSION_PATCH 40)
# KDevplatform dependency version
set( KDEVPLATFORM_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}" )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${sql_SOURCE_DIR}/cmake)

cmake_minimum_required(VERSION 3.14.6)

find_package(ECM "5.14.0"  REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

find_package(KF5 REQUIRED COMPONENTS
    Plasma
    CoreAddons
    XmlGui
    TextEditor
    I18n
    KDELibs4Support
    KCMUtils
    ConfigWidgets
)
find_package(KDevPlatform ${KDEVPLATFORM_VERSION} REQUIRED)
find_package(KDevelop ${KDEVPLATFORM_VERSION} REQUIRED)
find_package(Qt5 REQUIRED
    Core
    Widgets
    Sql
)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/version.h.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/version.h"
  @ONLY
)

enable_testing()

include_directories(
    ${KDEVPLATFORM_INCLUDE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/connections
    ${CMAKE_CURRENT_SOURCE_DIR}/results
    ${CMAKE_CURRENT_SOURCE_DIR}/schema
)

include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(ECMQtDeclareLoggingCategory)
include(FeatureSummary)

set(kdevsqllanguagesupport_SRCS
    sqllanguagesupport.cpp
    results/resulttablewidget.cpp
    results/queryworker.cpp
    connections/connectionsmodel.cpp
    connections/connections.cpp
    connections/connectionsallprojectsmodel.cpp
    schema/schemabrowserwidget.cpp
    schema/dbschemamodel.cpp
    schema/dbschemaitem.cpp
    dbinfo/dbinfobase.cpp
    dbinfo/dbinfopostgres.cpp
    dbinfo/dbinfosqlite.cpp
    sqltoolviewbase.cpp
)

ki18n_wrap_ui( kdevsqllanguagesupport_SRCS
    results/results.ui
    schema/schemabrowser.ui
    connections/connections.ui
)

#add_library(kdevsqllanguagesupport ${kdevsqllanguagesupport_SRCS})

qt5_add_resources( kdevsqllanguagesupport_SRCS kdevsqlui.qrc )
kdevplatform_add_plugin(kdevsqllanguagesupport
    JSON sqllanguagesupport.json
    SOURCES ${kdevsqllanguagesupport_SRCS}
)

target_link_libraries(kdevsqllanguagesupport
    KDev::Interfaces
    KDev::Language
    Qt5::Widgets
    Qt5::Core
    Qt5::Sql
    KF5::XmlGui
    KF5::CoreAddons
    KF5::TextEditor
    KF5::I18n
    KF5::KDELibs4Support
    KF5::KCMUtils
    KF5::ConfigWidgets
)


install(TARGETS kdevsqllanguagesupport DESTINATION ${PLUGIN_INSTALL_DIR})

install( FILES kdevsqlui.rc DESTINATION ${KXMLGUI_INSTALL_DIR}/kdevsqlsupport )

# kdebugsettings file
if (ECM_VERSION VERSION_GREATER "5.58.0")
    install(FILES kdevsqlsupport.categories DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR})
else()
    install(FILES kdevsqlsupport.categories DESTINATION ${KDE_INSTALL_CONFDIR})
endif()

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
