win32 {
    LIBS += -L$$PWD/dependencies/glew/lib/ -lglew32

    PWD_WIN = $$replace(PWD,"/","\\")
    OUT_PWD_WIN = $$replace(OUT_PWD,"/","\\")
    QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\dependencies\\glew\\bin\\glew32.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
}

INCLUDEPATH +=\
    $$PWD/dependencies/glew/include