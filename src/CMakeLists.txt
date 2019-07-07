MESSAGE(STATUS "Checking ${CMAKE_CURRENT_SOURCE_DIR} ")
SET(BINARY_NAME "QtTestGadget")

# Gui has extra QT dependencies: QML and QTquick
find_package(Qt5 COMPONENTS Qml Core Quick REQUIRED)

#Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#-------------------------------------------------------------------------------
# App Depends on library
#-------------------------------------------------------------------------------

SET(PROJECT_CORE_LIB "")

#-------------------------------------------------------------------------------
# C++ Srcs
#-------------------------------------------------------------------------------
SET(SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/logger.cpp
  )

#------------------------------
# QT MOC, UIC RCC input
#------------------------------
SET(UI_HDRS
  )

SET(MOC_SRC
  )

SET(QRC_SRC
  qml/qml.qrc
  externalRes/external.qrc
  )

#------------------------------
# add compile definitions
#------------------------------
SET(CPP_DEFS "")
LIST(APPEND CPP_DEFS "" )

#------------------------------
# Binary
#------------------------------
# Executable (for target)
ADD_EXECUTABLE(${BINARY_NAME}
  ${SRCS}
  ${UI_HDRS}
  ${MOC_SRC}
  ${QRC_SRC}
  )

SET_TARGET_PROPERTIES(
  ${BINARY_NAME}
  PROPERTIES
  VERSION ${PROJECT_VERSION}
  )

TARGET_COMPILE_DEFINITIONS(${BINARY_NAME}
  # no public flags, nobody should use the main app as dependency
  PRIVATE
  ${CPP_DEFS}
  )

TARGET_COMPILE_OPTIONS(${BINARY_NAME} PRIVATE
  $<$<COMPILE_LANGUAGE:CXX>:${CUSTOM_CXX_FLAGS}>
  $<$<COMPILE_LANGUAGE:C>:${CUSTOM_C_FLAGS}>
  )

# Linkage with gtest_main coverage etc.
TARGET_LINK_LIBRARIES(
  ${BINARY_NAME}
  PRIVATE
  ${PROJECT_CORE_LIB}
  Qt5::Quick
  Qt5::Qml
  Qt5::Core
  ${CUSTOM_LINK_FLAGS}
  )

#-----
# Install
#-----
MESSAGE(STATUS "** Generating Package Configurations **")

INSTALL(TARGETS ${BINARY_NAME}
  EXPORT ${targets_export_name}
  COMPONENT APPLICATION
  ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
  LIBRARY DESTINATION ${INSTALL_LIB_DIR}
  RUNTIME DESTINATION ${INSTALL_BIN_DIR}
  INCLUDES DESTINATION ${INSTALL_INCLUDE_DIR}
  )
