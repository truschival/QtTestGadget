/******************************************************************************
 * \filename
 * \brief   Main entry point for QML Gui
 *
 * \details
 *
 * \author Thomas Ruschival
 * \copyright 2018 Thomas Ruschival <thomas@ruschival.de>
 * 			  This file is licensed under GNU PUBLIC LICENSE Version 3 or later
 * 			  SPDX-License-Identifier: GPL-3.0-or-later
 *****************************************************************************/

/* only allow QML debugging for Debug builds */
#ifndef NDEBUG
#define QT_QML_DEBUG
#endif

#include <QCommandLineParser>
#include <QDebug>
#include <QFontDatabase>
#include <QGuiApplication>
#include <QLoggingCategory>
#include <QQmlApplicationEngine>
#include <QtQuick>

#include <cstdio>
#include <cstdlib>
#include <sys/reboot.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input-event-codes.h>
#include <linux/input.h>

// STD C++
#include <iostream>
#include <memory>
// Local classes
#include "appconstants.hpp"
#include "logger.hpp"

Q_DECLARE_LOGGING_CATEGORY(MAIN)
Q_LOGGING_CATEGORY(MAIN, "main")


/*****************************************************************************
 * Application constants
 ****************************************************************************/
/**
 * Log file path
 */
const QString DEFAULT_LOG_PATH(
    QDir(QStandardPaths::writableLocation(QStandardPaths::TempLocation))
        .filePath(APPLICATION_NAME + ".log"));

/*****************************************************************************/
int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setApplicationName(APPLICATION_NAME);
    QCoreApplication::setApplicationVersion(PROJECT_VERSION);

    /*
     * Setup Commandline Parser
     */
    QCommandLineParser cmdline;
    QCommandLineOption logstdout({"s", "stdout"}, QString("log to stdout"));
    QCommandLineOption logfile({"l", "logfile"},
        QString("application log <file>"), // description
        QString("logfile")                 // value name
    );

    cmdline.addOption(logfile);
    cmdline.addHelpOption();
    cmdline.addVersionOption();
    cmdline.process(app);
    /*
     * Setup Logfacility
     */
    if (cmdline.isSet(logfile)) {
        setup_logger_file(cmdline.value(logfile));
    } else { // Default behavour as before
        setup_logger_stdout(); // Write log to stdout
    }
    qCInfo(MAIN) << APPLICATION_NAME << " - " << GIT_REVISION;
    qCDebug(MAIN) << "SSL Support: " << QSslSocket::supportsSsl()
                  << QSslSocket::sslLibraryVersionString();

    
    int rotary_filehandle = ::open("/dev/input/event1", O_RDONLY);
    if(rotary_filehandle < 0){
    	qCWarning(MAIN) << " failed to open/dev/input/event1";
    }

    QSocketNotifier qsnf(rotary_filehandle, QSocketNotifier::Read);
    qsnf.setEnabled(true);
    QObject::connect(&qsnf, &QSocketNotifier::activated, [&]() {
        std::cout << "Activated";
        struct input_event evt_raw;
        auto s = ::read(rotary_filehandle, &evt_raw, sizeof(evt_raw));
        if (s < 0) {
            std::cout << "ERROR ";
        }

        if (s > 0) {
            std::cout << "T:" << evt_raw.type << "V:" << evt_raw.value
                      << std::endl;
        }
        std::cout << "-" << std::endl;
    });

    int push_button_filehandle = open("/sys/class/gpio/gpio22/value", O_RDONLY);
    if(push_button_filehandle < 0){
    	qCWarning(MAIN) << " failed to open push-button GPIO";
    }
    /* connect notifier and handler for push button */
    QSocketNotifier button_notifier(
    		push_button_filehandle, QSocketNotifier::Read);

    QObject::connect(&button_notifier, &QSocketNotifier::activated, [&]() {
        // disable during read, otherwise QSocketNotifier is triggered again
        button_notifier.setEnabled(false);
        char value = 0;
        lseek(push_button_filehandle, 0, SEEK_SET);
        auto s = ::read(push_button_filehandle, &value, sizeof(char));
        std::cout << "pb: " << s << " -" << std::endl;

        button_notifier.setEnabled(true);
    });


    QQmlApplicationEngine view;
    QQmlContext* ctxt = view.rootContext();

    ctxt->setContextProperty(
    "DEFAULT_ICON_WIDTH", QVariant::fromValue(DEFAULT_ICON_WIDTH));

    view.load(QUrl("qrc:/main.qml"));

    return app.exec();
}
