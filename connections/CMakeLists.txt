include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(kcm_kdev_sqlconnections_SRCS
   connections.cpp
   connectionsmodel.cpp
)
qt5_wrap_ui( kcm_kdev_sqlconnections_SRCS connections.ui )
add_library(kcm_kdev_sqlconnections ${kcm_kdev_sqlconnections_SRCS})
qt5_use_modules(kcm_kdev_sqlconnections Widgets Core Sql)

target_link_libraries(kcm_kdev_sqlconnections 
    KF5::KCMUtils
    KDev::Interfaces
    KDev::Language
    Qt5::Sql
)
install(TARGETS kcm_kdev_sqlconnections  DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES  kcm_kdev_sqlconnections.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
