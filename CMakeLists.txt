cmake_minimum_required(VERSION 3.14)

project(quickm3u)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/W4>)
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/permissive->)
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/std:c++latest>)
add_compile_options($<$<CXX_COMPILER_ID:GNU,AppleClang,Clang>:-Wall>)
add_compile_options($<$<CXX_COMPILER_ID:GNU,AppleClang,Clang>:-pedantic>)
add_compile_options($<$<CXX_COMPILER_ID:Clang>:-stdlib=libc++>)
add_link_options($<$<CXX_COMPILER_ID:Clang>:-stdlib=libc++>)

set(QT5_BASE_ROOT "" CACHE PATH "Qt5 installation root directory.")
find_package(Qt5 REQUIRED COMPONENTS Widgets
    HINTS ${QT5_BASE_ROOT} $ENV{QT5_ROOT}/msvc2015 $ENV{QT5_ROOT}/msvc2015_64 $ENV{QT5_ROOT}/msvc2017 $ENV{QT5_ROOT}/msvc2017_64
)

set(GHULBUS_BASE_ROOT "" CACHE PATH "GhulbusBase installation root directory.")
if(GHULBUS_BASE_ROOT)
    list(APPEND CMAKE_PREFIX_PATH ${GHULBUS_BASE_ROOT}/cmake)
endif()
find_package(gbBase REQUIRED)


set(QUICKM3U_SOURCE_DIRECTORY ${PROJECT_SOURCE_DIR}/src)

set(QUICKM3U_SOURCE_FILES
    ${QUICKM3U_SOURCE_DIRECTORY}/quickm3u/m3u_file.cpp
    ${QUICKM3U_SOURCE_DIRECTORY}/quickm3u/main.cpp
)

set(QUICKM3U_HEADER_FILES
    ${QUICKM3U_SOURCE_DIRECTORY}/quickm3u/m3u_file.hpp
    ${QUICKM3U_SOURCE_DIRECTORY}/quickm3u/version.hpp
)

set(QUICKM3U_QT_SOURCE_FILES
    ${QUICKM3U_SOURCE_DIRECTORY}/quickm3u/ui/about_dialog.cpp
    ${QUICKM3U_SOURCE_DIRECTORY}/quickm3u/ui/central_widget.cpp
    ${QUICKM3U_SOURCE_DIRECTORY}/quickm3u/ui/list_view.cpp
    ${QUICKM3U_SOURCE_DIRECTORY}/quickm3u/ui/m3u_file_model.cpp
    ${QUICKM3U_SOURCE_DIRECTORY}/quickm3u/ui/main_window.cpp
)

set(QUICKM3U_QT_HEADER_FILES
)

set(QUICKM3U_QT_MOC_HEADER_FILES
    ${QUICKM3U_SOURCE_DIRECTORY}/quickm3u/ui/about_dialog.hpp
    ${QUICKM3U_SOURCE_DIRECTORY}/quickm3u/ui/central_widget.hpp
    ${QUICKM3U_SOURCE_DIRECTORY}/quickm3u/ui/list_view.hpp
    ${QUICKM3U_SOURCE_DIRECTORY}/quickm3u/ui/m3u_file_model.hpp
    ${QUICKM3U_SOURCE_DIRECTORY}/quickm3u/ui/main_window.hpp
)
qt5_wrap_cpp(QUICKM3U_QT_MOC_SOURCE_FILES ${QUICKM3U_QT_MOC_HEADER_FILES})

source_group(moc FILES
    ${QUICKM3U_QT_MOC_SOURCE_FILES}
)

qt5_add_resources(QUICKM3U_QT_RESOURCE_FILES ${PROJECT_SOURCE_DIR}/resources/quickm3u.qrc)
source_group(resources FILES
    ${QUICKM3U_QT_RESOURCE_FILES}
)


add_executable(quickm3u WIN32
    ${QUICKM3U_SOURCE_FILES}
    ${QUICKM3U_HEADER_FILES}
    ${QUICKM3U_QT_SOURCE_FILES}
    ${QUICKM3U_QT_HEADER_FILES}
    ${QUICKM3U_QT_MOC_HEADER_FILES}
    ${QUICKM3U_QT_MOC_SOURCE_FILES}
    ${QUICKM3U_QT_RESOURCE_FILES}
)
target_include_directories(quickm3u PUBLIC ${QUICKM3U_SOURCE_DIRECTORY})
target_link_libraries(quickm3u PUBLIC
    Qt5::Core
    Qt5::Widgets
)
target_sources(quickm3u PUBLIC $<$<CXX_COMPILER_ID:MSVC>:${PROJECT_SOURCE_DIR}/resources/resource.rc>)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT quickm3u)

if(WIN32)
    function(getPDBForDLL DLL_PATH OUT_VAR)
        get_filename_component(dll_dir ${DLL_PATH} DIRECTORY)
        get_filename_component(dll_we ${DLL_PATH} NAME_WE)
        set(${OUT_VAR} "${dll_dir}/${dll_we}.pdb" PARENT_SCOPE)
    endfunction()


    function(getQt5Dlls QT_TARGET OUT_VAR)
        unset(DLLS)
        get_property(tmp TARGET ${QT_TARGET} PROPERTY IMPORTED_LOCATION_DEBUG)
        list(APPEND DLLS ${tmp})
        getPDBForDLL(${tmp} tmp_pdb)
        if (EXISTS ${tmp_pdb})
            list(APPEND DLLS ${tmp_pdb})
        endif()
        get_property(tmp TARGET ${QT_TARGET} PROPERTY IMPORTED_LOCATION_RELEASE)
        list(APPEND DLLS ${tmp})
        get_property(deps TARGET ${QT_TARGET} PROPERTY IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG)
        foreach(dep ${deps})
            if(TARGET ${dep})
                getQt5Dlls(${dep} tmp)
                list(APPEND DLLS ${tmp})
            endif()
        endforeach()
        set(result ${${OUT_VAR}})
        list(APPEND result ${DLLS})
        list(REMOVE_DUPLICATES result)
        set(${OUT_VAR} ${result} PARENT_SCOPE)
    endfunction()

    getQt5Dlls(Qt5::Widgets qt_DLLS)

    file(COPY ${qt_DLLS} DESTINATION ${PROJECT_BINARY_DIR})

endif()
