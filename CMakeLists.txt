cmake_minimum_required(VERSION 3.27)
project(CMCL)

get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId} CXX)
project(CMCL)
file(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
foreach(file ${files})
    get_filename_component(name ${file} NAME)
endforeach()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "D:/tools/vcpkg/installed/x64-windows")
set(CMAKE_PREFIX_PATH "G:/Qt/6.8.0/mingw_64/lib")
set(Qt6_DIR "G:/Qt/6.8.0/mingw_64/lib/cmake/Qt6")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Xml
        Widgets
        REQUIRED)

find_package(Qt6 COMPONENTS Core Widgets REQUIRED)
qt6_wrap_cpp(MOC_FILES
        modules/interface/controls/myControls.h
        modules/interface/controls/myTitleBar.h
        modules/interface/controls/myMainSideBar.h
        modules/interface/controls/pageChange.h
        modules/interface/controls/myContentList.h
        modules/interface/pages/pageBase.h
        modules/interface/pages/mainLayout.h
        modules/interface/pages/pages.h
        modules/interface/animation/bounceAnimation.h
        #modules/interface/animation/myAnimator.h
        modules/interface/baseWindow.h
        modules/interface/mainApplication.h
)

add_executable(CMCL main.cpp
        core/consolas.cpp
        core/consolas.h
        core/json/json_tool.h
        core/json/json_reader.cpp
        core/json/json_valueiterator.inl
        core/json/json_writer.cpp
        core/json/assertions.h
        core/json/allocator.h
        core/json/config.h
        core/json/json_features.h
        core/json/forwards.h
        core/json/json.h
        core/json/reader.h
        core/json/value.h
        core/json/version.h
        core/json/writer.h
        core/json/json_value.cpp
        core/io/fileReader.cpp
        core/io/fileReader.h
        core/io/fileWriter.cpp
        core/io/fileWriter.h
        CMCL.rc
        modules/minecraft/libraries.cpp
        modules/minecraft/assets.h
        modules/minecraft/natives.cpp
        modules/minecraft/arguments.cpp
        modules/minecraft/launch.h
        modules/minecraft/objects.cpp
        modules/minecraft/assets.cpp
        modules/minecraft/launch.cpp
        modules/minecraft/modloader/mergeJSON.cpp
        modules/minecraft/modloader/mergeJSON.h
        modules/minecraft/modloader/forge.cpp
        modules/minecraft/modloader/fabric.cpp
        modules/minecraft/modloader/optifine.cpp
        modules/minecraft/modloader/loader.cpp
        modules/minecraft/modloader/loader.h
        modules/interface/pages/mainPage.cpp
        modules/interface/pages/pages.h
        core/url/download/simpleDownloader.cpp
        modules/minecraft/download/downloadVersion.cpp
        modules/minecraft/download/downloadAssets.cpp
        modules/minecraft/download/downloadObjects.cpp
        modules/minecraft/download/downloads.h
        modules/minecraft/download/downloads.cpp
        modules/interface/mainApplication.cpp
        modules/interface/mainApplication.h
        ${MOC_FILES}
        modules/interface/controls/myControls.cpp
        modules/interface/controls/myControls.h
        modules/interface/controls/myButton.cpp
        modules/interface/pages/mainLayout.cpp
        modules/interface/pages/mainLayout.h
        modules/interface/controls/myTitleBar.cpp
        modules/interface/controls/myTitleBar.h
        core/core.cpp core/core.h
        modules/interface/baseWindow.cpp
        modules/interface/baseWindow.h
        modules/interface/controls/myContentCard.cpp
        modules/interface/controls/myTextButton.cpp
        modules/interface/controls/myMessageBox.cpp
        modules/interface/animation/bounceAnimation.cpp
        modules/interface/animation/bounceAnimation.h
        modules/interface/controls/myMainSideBar.cpp
        modules/interface/controls/myMainSideBar.h
        modules/interface/controls/pageChange.cpp
        modules/interface/controls/pageChange.h
        modules/interface/pages/pageSetup.cpp
        modules/interface/controls/myIconButton.cpp
        modules/interface/controls/myContentList.cpp
        modules/interface/controls/myContentList.h
        modules/interface/pages/pageBase.cpp
        modules/interface/pages/pageBase.h
        modules/interface/controls/myRoundButton.cpp
        modules/interface/controls/myTextIconButton.cpp
        modules/interface/controls/myRoundIconButton.cpp
        modules/interface/animation/myAnimator.cpp
        modules/interface/animation/myAnimator.h
        core/url/download/multiThreadDownloader.cpp
        core/url/download/multiThreadDownloader.h
        core/url/webFetcher.cpp
        core/url/webFetcher.h
        modules/minecraft/versions/manifestParser.h
        core/url/HTMLParser.cpp
        core/url/HTMLParser.h
        core/io/jarExtractor.cpp
        core/io/jarExtractor.h
        core/url/download/concurrentDownloader.cpp
        core/url/download/concurrentDownloader.h
        core/url/download/simpleDownloader.h
        core/url/download/downloaderToolkit.h
        core/data/unzip/zipExtractor.cpp
        core/data/unzip/zipExtractor.h
        modules/minecraft/versions/manifestParser.cpp
        # core/data/AES256Encryptor.cpp
        # core/data/AES256Encryptor.h
        core/integration/pugixml/pugixml.cpp
        core/integration/pugixml/pugixml.h
        modules/minecraft/modloader/modCurseForge.cpp
        modules/minecraft/modloader/modCurseForge.h
        modules/minecraft/modloader/modModrinth.cpp
        modules/minecraft/modloader/modModrinth.h
        modules/interface/animation/rotatedWidget.cpp
        modules/interface/animation/rotatedWidget.h
)

find_package(cryptopp CONFIG REQUIRED)
find_package(CURL REQUIRED)
find_package(miniz CONFIG REQUIRED)

target_link_libraries(CMCL PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Xml
        Qt::Widgets
)

target_link_libraries(CMCL PRIVATE
        CURL::libcurl
        miniz::miniz
        cryptopp::cryptopp
)

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")


