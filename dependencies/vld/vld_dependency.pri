PWD_WIN = $$replace(PWD,"/","\\")
OUT_PWD_WIN = $$replace(OUT_PWD,"/","\\")

LIBS += -L$$_PRO_FILE_PWD_/dependencies/vld/lib/ -lvld

QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\bin\\vld_x64.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\bin\\dbghelp.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\bin\\Microsoft.DTfW.DHL.manifest" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))

INCLUDEPATH +=\
    $$PWD/include
