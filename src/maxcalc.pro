CONFIG += ordered
TEMPLATE = subdirs
wince* {
    SUBDIRS += engine gui
} else {
    SUBDIRS += engine cli gui test
}
