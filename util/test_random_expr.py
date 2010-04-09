import random
import subprocess

number_format = '%.15f'
test_program = '../src/release/maxcalc.exe'

def operations():
    expr = brackets()

    if (random.random() < 0.75):
        for i in range(random.randint(1, 5)):
            rand = random.random()
            if (rand < 0.25):
                op = '+'
            elif (rand < 0.5):
                op = '-'
            elif (rand < 0.75):
                op = '*'
            else:
                op = '/'
            expr += op + brackets()

    return expr

def brackets():
    if (random.random() < 0.25):
        return '(' + operations() + ')'
    else:
        return number()

def number():
    return number_format % random.uniform(-1000.0, 1000.0)

def compare_numbers(num1, num2, max_diff):
    if len(num1) != len(num2):
        return FALSE

    diff_count = 0
    for i in range(len(num1)):
        if (num1[i] != num2[i]):
            diff_count += 1
    return diff_count <= max_diff

def test():
    expr = operations()
    result1 = number_format % eval(expr)
    output = subprocess.Popen([test_program, '-c', expr], stdout=subprocess.PIPE).communicate()[0]
    result2 = number_format % float(output)
    if (compare_numbers(result1, result2, 5)):
        print('PASS')
    else:
        print('FAIL!')
        print('Expr    = ' + expr)
        print('Python  = ' + result1)
        print('MaxCalc = ' + result2)

for i in range(100):
    test()
