add_definitions(-DTRANSLATION_DOMAIN=\"processui\")

check_include_files(sys/endian.h HAVE_SYS_ENDIAN_H)
check_include_files(byteswap.h HAVE_BYTESWAP_H)

set(ksysguard_WIDGETS "ksysguard.widgets")

set(processui_LIB_SRCS
   ksysguardprocesslist.cpp
   ProcessFilter.cpp
   ProcessModel.cpp
   ReniceDlg.cpp ReniceDlg.h
   KTextEditVT.cpp
   scripting.cpp scripting.h
   processdetails/GeneralTab.cpp processdetails/GeneralTab.h
   processdetails/MemoryMapsTab.cpp processdetails/MemoryMapsTab.h
   processdetails/OpenFilesTab.cpp processdetails/OpenFilesTab.h
   processdetails/ProcessDetailsDialog.cpp processdetails/ProcessDetailsDialog.h
)

ecm_qt_declare_logging_category(processui_LIB_SRCS HEADER processui_debug.h IDENTIFIER LIBKSYSGUARD_PROCESSUI CATEGORY_NAME org.kde.libksysguard.processui
    DESCRIPTION "libksysguard (processui)" EXPORT LIBKSYSGUARD)


ki18n_wrap_ui( processui_LIB_SRCS
   ReniceDlgUi.ui
   ProcessWidgetUI.ui
)

add_library(processui ${processui_LIB_SRCS})
add_library(KSysGuard::ProcessUi ALIAS processui)

if(HAVE_XRES)
    target_link_libraries(processui PRIVATE ${X11_XRes_LIB} ${X11_LIBRARIES})
    include_directories(${X11_XRes_INCLUDE_PATH})
endif()

generate_export_header(processui)

target_link_libraries(processui
    PUBLIC
        KSysGuard::ProcessCore
        Qt::Widgets
        KF6::ConfigCore
    PRIVATE
        Qt::DBus
        Qt::Core5Compat
        KF6::I18n
        KF6::WindowSystem
        KF6::AuthCore
        KF6::ConfigWidgets
        KF6::WidgetsAddons
        KF6::IconThemes
        KF6::JobWidgets
        KF6::GlobalAccel
        KF6::Service
        KF6::KIOGui
)
target_include_directories(processui
    PUBLIC
        "$<BUILD_INTERFACE:${libksysguard_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/ksysguard>"
)
if(HAVE_X11)
    target_link_libraries(processui PRIVATE Qt6::GuiPrivate KF6::WindowSystem)
endif()

if(WEBENGINE_SCRIPTING_ENABLED)
    target_link_libraries(processui PRIVATE Qt::WebEngineWidgets Qt::WebChannel)
endif()

if(NOT HAVE_CLOCK_GETTIME_C)
    target_link_libraries(processui PRIVATE rt)
endif()

set_target_properties(processui PROPERTIES
   VERSION ${KSYSGUARD_VERSION}
   SOVERSION ${KSYSGUARD_SOVERSION}
   EXPORT_NAME ProcessUi
)

install(TARGETS processui EXPORT libksysguardLibraryTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} )

install( FILES
    ProcessModel.h
    ProcessFilter.h
    KTextEditVT.h
    ksysguardprocesslist.h
    ${CMAKE_CURRENT_BINARY_DIR}/processui_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/ksysguard/processui
    COMPONENT Devel
)

if(BUILD_DESIGNERPLUGIN)
    add_subdirectory(designer)
endif()
