import random
import subprocess

test_program = 'release/maxcalc.exe'

def generate_expr():
    length = random.randint(10, 100)
    expr = ''
    for i in range(length):
        expr += chr(random.randint(1, 255))
    return expr

def test():
    expr = generate_expr()
    result = subprocess.Popen([test_program, '-c', expr], stdout=subprocess.PIPE).communicate()[0]
    if result == b'Error!':
        print('PASS')
    else:
        print('FAIL')
        print(result)

for i in range(100):
    test()
