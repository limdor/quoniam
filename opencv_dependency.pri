CONFIG(debug, debug|release){
    COMPILATION = debug
}
else {
    COMPILATION = release
}

win32 {
    PWD_WIN = $$replace(PWD,"/","\\")
    OUT_PWD_WIN = $$replace(OUT_PWD,"/","\\")
    equals(COMPILATION,debug){
        LIBS += -L$$PWD/dependencies/opencv/lib/$$COMPILATION/ -lopencv_core331d
        LIBS += -L$$PWD/dependencies/opencv/lib/$$COMPILATION/ -lopencv_imgproc331d
        LIBS += -L$$PWD/dependencies/opencv/lib/$$COMPILATION/ -lopencv_imgcodecs331d
        LIBS += -L$$PWD/dependencies/opencv/lib/$$COMPILATION/ -lopencv_highgui331d

        QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\dependencies\\opencv\\bin\\$$COMPILATION\\opencv_core331d.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\dependencies\\opencv\\bin\\$$COMPILATION\\opencv_imgproc331d.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\dependencies\\opencv\\bin\\$$COMPILATION\\opencv_imgcodecs331d.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\dependencies\\opencv\\bin\\$$COMPILATION\\opencv_highgui331d.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
    }
    else {
        LIBS += -L$$PWD/dependencies/opencv/lib/$$COMPILATION/ -lopencv_core331
        LIBS += -L$$PWD/dependencies/opencv/lib/$$COMPILATION/ -lopencv_imgproc331
        LIBS += -L$$PWD/dependencies/opencv/lib/$$COMPILATION/ -lopencv_imgcodecs331
        LIBS += -L$$PWD/dependencies/opencv/lib/$$COMPILATION/ -lopencv_highgui331

        QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\dependencies\\opencv\\bin\\$$COMPILATION\\opencv_core331.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\dependencies\\opencv\\bin\\$$COMPILATION\\opencv_imgproc331.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\dependencies\\opencv\\bin\\$$COMPILATION\\opencv_imgcodecs331.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\dependencies\\opencv\\bin\\$$COMPILATION\\opencv_highgui331.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
    }
}

INCLUDEPATH +=\
    $$PWD/dependencies/opencv/include
