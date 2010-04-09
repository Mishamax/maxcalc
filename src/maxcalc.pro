
include(maxcalc_config.pri)

CONFIG += ordered
TEMPLATE = subdirs

wince* {
    SUBDIRS = engine gui
} else {
    SUBDIRS = engine cli
    maxcalc_unicode {
        SUBDIRS += gui test
    }
}
