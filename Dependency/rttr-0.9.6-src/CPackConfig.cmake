# This file will be configured to contain variables for CPack. These variables
# should be set in the CMake list file of the project before CPack module is
# included. The list of available CPACK_xxx variables and their associated
# documentation may be obtained using
#  cpack --help-variable-list
#
# Some variables are common to all generators (e.g. CPACK_PACKAGE_NAME)
# and some are specific to a generator
# (e.g. CPACK_NSIS_EXTRA_INSTALL_COMMANDS). The generator specific variables
# usually begin with CPACK_<GENNAME>_xxxx.


set(CPACK_BUILD_SOURCE_DIRS "C:/GA3rdFinalProject_DUOL/5_Project/Dependency/rttr-0.9.6-src;C:/GA3rdFinalProject_DUOL/5_Project/Dependency/rttr-0.9.6-src")
set(CPACK_CMAKE_GENERATOR "Visual Studio 17 2022")
set(CPACK_COMPONENTS_ALL "Devel;Unspecified")
set(CPACK_COMPONENT_UNSPECIFIED_HIDDEN "TRUE")
set(CPACK_COMPONENT_UNSPECIFIED_REQUIRED "TRUE")
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_FILE "C:/Program Files/CMake/share/cmake-3.25/Templates/CPack.GenericDescription.txt")
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_SUMMARY "rttr built using CMake")
set(CPACK_DMG_SLA_USE_RESOURCE_FILE_LICENSE "ON")
set(CPACK_GENERATOR "7Z")
set(CPACK_INSTALL_CMAKE_PROJECTS "C:/GA3rdFinalProject_DUOL/5_Project/Dependency/rttr-0.9.6-src;rttr;ALL;/")
set(CPACK_INSTALL_PREFIX "C:/GA3rdFinalProject_DUOL/5_Project/Dependency/rttr-0.9.6-src/install")
set(CPACK_MODULE_PATH "C:/GA3rdFinalProject_DUOL/5_Project/Dependency/rttr-0.9.6-src/CMake")
set(CPACK_NSIS_DISPLAY_NAME "AMS/RTTR")
set(CPACK_NSIS_INSTALLER_ICON_CODE "")
set(CPACK_NSIS_INSTALLER_MUI_ICON_CODE "")
set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
set(CPACK_NSIS_PACKAGE_NAME "AMS/RTTR")
set(CPACK_NSIS_UNINSTALL_NAME "Uninstall")
set(CPACK_OUTPUT_CONFIG_FILE "C:/GA3rdFinalProject_DUOL/5_Project/Dependency/rttr-0.9.6-src/CPackConfig.cmake")
set(CPACK_PACKAGE_DEFAULT_LOCATION "/")
set(CPACK_PACKAGE_DESCRIPTION_FILE "C:/GA3rdFinalProject_DUOL/5_Project/Dependency/rttr-0.9.6-src/README.md")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "RTTR is a C++ runtime reflection library")
set(CPACK_PACKAGE_FILE_NAME "rttr-0.9.6-win64-")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "AMS/RTTR")
set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "AMS/RTTR")
set(CPACK_PACKAGE_NAME "RTTR")
set(CPACK_PACKAGE_RELOCATABLE "true")
set(CPACK_PACKAGE_VENDOR "AMS")
set(CPACK_PACKAGE_VERSION "0.9.6")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "9")
set(CPACK_PACKAGE_VERSION_PATCH "6")
set(CPACK_RESOURCE_FILE_LICENSE "C:/GA3rdFinalProject_DUOL/5_Project/Dependency/rttr-0.9.6-src/LICENSE.txt")
set(CPACK_RESOURCE_FILE_README "C:/GA3rdFinalProject_DUOL/5_Project/Dependency/rttr-0.9.6-src/README.md")
set(CPACK_RESOURCE_FILE_WELCOME "C:/Program Files/CMake/share/cmake-3.25/Templates/CPack.GenericWelcome.txt")
set(CPACK_SET_DESTDIR "OFF")
set(CPACK_SOURCE_7Z "ON")
set(CPACK_SOURCE_GENERATOR "7Z;ZIP")
set(CPACK_SOURCE_OUTPUT_CONFIG_FILE "C:/GA3rdFinalProject_DUOL/5_Project/Dependency/rttr-0.9.6-src/CPackSourceConfig.cmake")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "rttr-0.9.6-src")
set(CPACK_SOURCE_ZIP "ON")
set(CPACK_SYSTEM_NAME "win64")
set(CPACK_THREADS "1")
set(CPACK_TOPLEVEL_TAG "win64")
set(CPACK_WIX_SIZEOF_VOID_P "8")

if(NOT CPACK_PROPERTIES_FILE)
  set(CPACK_PROPERTIES_FILE "C:/GA3rdFinalProject_DUOL/5_Project/Dependency/rttr-0.9.6-src/CPackProperties.cmake")
endif()

if(EXISTS ${CPACK_PROPERTIES_FILE})
  include(${CPACK_PROPERTIES_FILE})
endif()
