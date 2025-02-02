cmake_minimum_required(VERSION 3.14)
project(MyQtProject LANGUAGES CXX)

# Find Qt packages
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(RESOURCES resources.qrc)

# find_package(PkgConfig REQUIRED)
# pkg_check_modules(MySQL REQUIRED mysqlclient)
#set (XX /opt/homebrew/Cellar/qt-mysql/6.7.3/share/qt/plugins/sqldrivers/libqsqlmysql.dylib)
#set(MYSQL_LIBRARY_DIR /opt/homebrew/opt/mysql-client/lib)
#set(MYSQL_INCLUDE_DIR /opt/homebrew/opt/mysql-client/include)

#find_library(MYSQLCLIENT_LIB
#    NAMES mysqlclient libmysqlclient
#    PATHS ${MYSQL_LIBRARY_DIR}
#    REQUIRED
#)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql Quick Core WebSockets Charts)
qt_add_resources(RESOURCES_RCC ${RESOURCES})
# Add executable
add_executable(qt_sqlite_todo qtit.cc db.cc)

# Link Qt libraries
target_link_libraries(qt_sqlite_todo PRIVATE Qt6::Widgets Qt6::Sql )

add_executable(qt_qmlit qmlit.cc ${RESOURCES_RCC})

target_link_libraries(qt_qmlit PRIVATE Qt6::Quick)

add_executable(qt_mysql qt_mysql.cc)
target_link_libraries(qt_mysql PRIVATE Qt6::Widgets Qt6::Sql)

add_executable(qt_websocket_stock qt_websocket_stock.cc)
target_link_libraries(qt_websocket_stock PRIVATE Qt6::Widgets Qt6::WebSockets Qt6::Charts)