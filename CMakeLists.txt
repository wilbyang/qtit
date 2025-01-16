cmake_minimum_required(VERSION 3.14)
project(MyQtProject)

# Find Qt packages
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(RESOURCES resources.qrc)


find_package(Qt6 REQUIRED COMPONENTS Widgets Sql Quick Core)
qt_add_resources(RESOURCES_RCC ${RESOURCES})
# Add executable
add_executable(qt_sqlite_todo qtit.cc db.cc)

# Link Qt libraries
target_link_libraries(qt_sqlite_todo PRIVATE Qt6::Widgets Qt6::Sql)

add_executable(qt_qmlit qmlit.cc ${RESOURCES_RCC})

target_link_libraries(qt_qmlit PRIVATE Qt6::Quick)
