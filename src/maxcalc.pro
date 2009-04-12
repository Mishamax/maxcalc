CONFIG += ordered
TEMPLATE = subdirs
wince* {
    SUBDIRS += maxcalcengine maxcalc-gui
} else {
    SUBDIRS += maxcalcengine maxcalc-cli maxcalc-gui maxcalctest
}
