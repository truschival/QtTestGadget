/******************************************************************************
 * \filename
 * \brief  Application global compile time configurations
 *
 * \details
 *
 * \copyright (c) 2018  Thomas Ruschival <thomas@ruschival.de>
 * \license {This file is licensed under GNU PUBLIC LICENSE Version 3 or later
 * 			 SPDX-License-Identifier: GPL-3.0-or-later}
 *
 *****************************************************************************/
#ifndef _APPCONSTANTS_HPP_
#define _APPCONSTANTS_HPP_

#include <QDir>
#include <QStandardPaths>
#include <QString>
#include <chrono>

#include "config.h"

#define STRINGIZER(arg) #arg
#define STR_VALUE(arg) STRINGIZER(arg)


/*****************************************************************************
 CMake build configurations from config.h
 *****************************************************************************/

/**
 * Application Name
 */
const QString APPLICATION_NAME(STR_VALUE(CMAKE_PROJECT_NAME));

/**
 * Project release version for configuration compatibility etc.
 */
const QString PROJECT_VERSION(STR_VALUE(CMAKE_PROJECT_VERSION));

/**
 * Git revision as QString
 */
const QString GIT_REVISION(STR_VALUE(GIT_DESCRIBE_REV));

/**
 * Project build time stamp
 */
const QString PROJECT_BUILD_TIME(STR_VALUE(CMAKE_BUILD_TIME_STAMP));


/****************************************************************************/
/**
 * Constant Podcast Icon size
 * Shared between C++ and QML
 */
const int DEFAULT_ICON_WIDTH = 88;

/**
 * Log file path
 * initialized in main.cpp or test.cpp
 */
extern const QString DEFAULT_LOG_PATH;



#endif /* _APPCONSTANTS_HPP_ */
