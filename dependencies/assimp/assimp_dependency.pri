
CONFIG(debug, debug|release){
    COMPILATION = debug
}
else {
    COMPILATION = release
}

win32 {
    LIBS += -L$$PWD/lib/$$COMPILATION/ -lassimp

    PWD_WIN = $$replace(PWD,"/","\\")
    OUT_PWD_WIN = $$replace(OUT_PWD,"/","\\")

    equals(COMPILATION,debug){
        QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\bin\\release\\assimp.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
    }
    else {
        QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\bin\\$$COMPILATION\\assimp.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
    }
}

INCLUDEPATH +=\
    $$PWD/include
