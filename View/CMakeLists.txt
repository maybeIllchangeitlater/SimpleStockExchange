cmake_minimum_required(VERSION 3.5)

project(View VERSION 0.1 LANGUAGES CXX)

ADD_SUBDIRECTORY(../3rdParty/libpqxx build-pqxx)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
ADD_COMPILE_OPTIONS(-Wall -Werror -Wextra -pedantic)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
FIND_PACKAGE(Boost 1.40 COMPONENTS thread system chrono REQUIRED)
FIND_PACKAGE(Threads REQUIRED)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})


set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.hpp
        mainwindow.ui
        ../Client/Client.hpp
        ../Client/Client.cpp
        ../Utility/RequestBuilder.cpp
        ../Utility/RequestBuilder.hpp
        ../Connection/Connection.cpp
        ../Connection/Connection.hpp
        ../Controller/ClientController.hpp
        ../Utility/ThreadSafeQ.hpp
        ../Utility/ExtraJSONKeys.hpp
        ../Utility/ResponseParser.hpp
        ../Utility/ResponseParser.cpp
        ../Utility/ClientDisplayMessages.hpp
        ../Repository/BdNames.hpp
        LoginPopup.hpp
        LoginPopup.cpp
        LoginPopup.ui
        RegisterPopup.hpp
        RegisterPopup.cpp
        RegisterPopup.ui
        CreateBidPopup.hpp
        CreateBidPopup.cpp
        CreateBidPopup.ui
        BidTransactionMadePopup.hpp
        BidTransactionMadePopup.cpp
        BidTransactionMadePopup.ui
        ViewBids.hpp
        ViewBids.cpp
        ViewBids.ui
        UpdateBidPopup.hpp
        UpdateBidPopup.cpp
        UpdateBidPopup.ui
        ViewTransactionsPopup.hpp
        ViewTransactionsPopup.cpp
        viewtransactionspopup.ui
        UserSettings.hpp
        UserSettings.cpp
        UserSettings.ui
        PasswordConfirmationPopup.hpp
        PasswordConfirmationPopup.cpp
        PasswordConfirmationPopup.ui
        ChangePassPopup.hpp
        ChangePassPopup.cpp
        ChangePassPopup.ui
        ChangeNamPopup.hpp
        ChangeNamPopup.cpp
        ChangeNamPopup.ui
        QuotationsPopup.hpp
        QuotationsPopup.cpp
        QuotationsPopup.ui

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(View
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET View APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(View SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(View
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(View PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Threads::Threads ${Boost_LIBRARIES} pqxx)

set_target_properties(View PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(View)
endif()
