import random
import subprocess

test_program = '../src/release/maxcalc.exe'

def generate_expr():
    length = random.randint(10, 100)
    expr = ''
    for i in range(length):
        expr += chr(random.randint(1, 255))
    return expr

def test():
    expr = generate_expr()
    output = subprocess.Popen([test_program, '-c', expr], stdout=subprocess.PIPE).communicate()[0]
    if b'Unknown token' in output or b'Incorrect unit conversion syntax' in output:
        print('PASS')
    else:
        print('FAIL')
        print(output)

for i in range(100):
    test()
