CONFIG(debug, debug|release){
    COMPILATION = debug
}
else {
    COMPILATION = release
}

win32 {
    OUT_PWD_WIN = $$replace(OUT_PWD,"/","\\")

    equals(COMPILATION,debug){
        QT_INSTALL_BINS_WIN = $$[QT_INSTALL_BINS]
        QT_INSTALL_BINS_WIN = $$replace(QT_INSTALL_BINS_WIN,"/","\\")
        QT_INSTALL_PLUGINS_WIN = $$[QT_INSTALL_PLUGINS]
        QT_INSTALL_PLUGINS_WIN = $$replace(QT_INSTALL_PLUGINS_WIN,"/","\\")
        QMAKE_POST_LINK += $$quote(copy "$$QT_INSTALL_BINS_WIN\\Qt5Cored.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$QT_INSTALL_BINS_WIN\\Qt5Guid.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$QT_INSTALL_BINS_WIN\\Qt5OpenGLd.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$QT_INSTALL_BINS_WIN\\Qt5Widgetsd.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(xcopy "$$QT_INSTALL_PLUGINS_WIN\\platforms\\qwindowsd.dll" "$$OUT_PWD_WIN\\bin\\platforms\\" /Y $$escape_expand(\\n))
    }
    else {
        QT_INSTALL_BINS_WIN = $$[QT_INSTALL_BINS]
        QT_INSTALL_BINS_WIN = $$replace(QT_INSTALL_BINS_WIN,"/","\\")
        QT_INSTALL_PLUGINS_WIN = $$[QT_INSTALL_PLUGINS]
        QT_INSTALL_PLUGINS_WIN = $$replace(QT_INSTALL_PLUGINS_WIN,"/","\\")
        QMAKE_POST_LINK += $$quote(copy "$$QT_INSTALL_BINS_WIN\\Qt5Core.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$QT_INSTALL_BINS_WIN\\Qt5Gui.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$QT_INSTALL_BINS_WIN\\Qt5OpenGL.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$QT_INSTALL_BINS_WIN\\Qt5Widgets.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(xcopy "$$QT_INSTALL_PLUGINS_WIN\\platforms\\qwindows.dll" "$$OUT_PWD_WIN\\bin\\platforms\\" /Y $$escape_expand(\\n))
    }
}
