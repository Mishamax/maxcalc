CONFIG += ordered
TEMPLATE = subdirs

# Compile MathML support
win32:CONFIG += maxcalc_mathml


wince* {
    SUBDIRS += engine gui
} else {
    SUBDIRS += engine cli gui test
}
