

INCLUDEPATH += \
               $$PWD/inc \

DEPENDPATH += \
              $$PWD/inc \
              $$PWD/cpp \



HEADERS += \
    $$PWD/inc/tile.h \
    $$PWD/inc/tile_types.h \
    $$PWD/inc/tile_manager.h \
    $$PWD/inc/tile_cache.h \
    $$PWD/inc/tile_swap.h \
    $$PWD/inc/tile_rowhints.h \

SOURCES += \
    $$PWD/cpp/tile.cpp \
    $$PWD/cpp/tile_manager.cpp \
    $$PWD/cpp/tile_cache.cpp \
    $$PWD/cpp/tile_swap.cpp \
    $$PWD/cpp/tile_rowhints.cpp \

