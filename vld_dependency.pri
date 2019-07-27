PWD_WIN = $$replace(PWD,"/","\\")
OUT_PWD_WIN = $$replace(OUT_PWD,"/","\\")

LIBS += -L$$PWD/dependencies/vld/lib/ -lvld

QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\dependencies\\vld\\bin\\vld_x64.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\dependencies\\vld\\bin\\dbghelp.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\dependencies\\vld\\bin\\Microsoft.DTfW.DHL.manifest" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))

INCLUDEPATH +=\
    $$PWD/dependencies/vld/include
