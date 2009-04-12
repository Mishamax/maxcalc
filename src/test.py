from random import *
from subprocess import *

number_format = '%.15f'
test_program = 'debug/maxcalc.exe'

def operations():
    expr = brackets()

    if (random() < 0.75):
        for i in xrange(randint(1, 5)):
            rand = random()
            if (rand < 0.2):
                op = '+';
            elif (rand < 0.4):
                op = '-';
            elif (rand < 0.6):
                op = '*';
            elif (rand < 0.8):
                op = '/';
            else:
                op = '+';   # change to '^'
            expr += op + brackets()

    return expr

def brackets():
    if (random() < 0.25):
        return '(' + operations() + ')';
    else:
        return number();

def number():
    return number_format % uniform(-1000.0, 1000.0);

def test():
    expr = operations()
    result1 = '%f' % eval(expr)
    result2 = '%f' % float(Popen([test_program, '-c', expr], stdout=PIPE).communicate()[0])
    if (result1 == result2):
        print 'PASS'
    else:
        print 'FAIL!'
        print 'Expr    = ' + expr
        print 'Python  = ' + result1
        print 'MaxCalc = ' + result2

for i in xrange(100):
    test()
