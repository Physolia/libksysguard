
set(ksignalplotter_WIDGETS "ksignalplotter.widgets")

set(ksignalplotter_LIB_SRCS
    ksignalplotter.cpp
    kgraphicssignalplotter.cpp kgraphicssignalplotter.h
)
ecm_qt_declare_logging_category(ksignalplotter_LIB_SRCS HEADER ksignalplotter_debug.h IDENTIFIER LIBKSYSGUARD_KSIGNALPLOTTER CATEGORY_NAME org.kde.libksysguard.ksignalplotter
    DESCRIPTION "libksysguard (ksignalplotter)" EXPORT LIBKSYSGUARD)
add_library(ksignalplotter ${ksignalplotter_LIB_SRCS})
add_library(KSysGuard::SignalPlotter ALIAS ksignalplotter)

generate_export_header(ksignalplotter)

target_link_libraries(ksignalplotter
    PUBLIC
        Qt::Widgets
        KF6::I18n # klocalizedstring.h
        KF6::IconThemes
    PRIVATE
        KSysGuard::ProcessCore
)

set_target_properties(ksignalplotter PROPERTIES
   VERSION ${KSYSGUARD_VERSION}
   SOVERSION ${KSYSGUARD_SOVERSION}
   EXPORT_NAME SignalPlotter
)

install(TARGETS ksignalplotter EXPORT libksysguardLibraryTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES ksignalplotter.h
    ${CMAKE_CURRENT_BINARY_DIR}/ksignalplotter_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/ksysguard COMPONENT Devel)

if(BUILD_DESIGNERPLUGIN)
    add_subdirectory(designer)
endif()
