
// Local
#include "bigdecimalbench.h"
#include "utility.h"
// MaxCalcEngine
#include "bigdecimal.h"
// STL
#include <string>

using namespace MaxCalcEngine;
using namespace std;

void BigDecimalBench::fact5()
{
	BENCHMARK(BigDecimal::factorial(5));
}

void BigDecimalBench::fact10()
{
	BENCHMARK(BigDecimal::factorial(10));
}

void BigDecimalBench::fact15()
{
	BENCHMARK(BigDecimal::factorial(15));
}

void BigDecimalBench::fact20()
{
	BENCHMARK(BigDecimal::factorial(20));
}

void BigDecimalBench::fact50()
{
	BENCHMARK(BigDecimal::factorial(50));
}

void BigDecimalBench::fact100()
{
	BENCHMARK(BigDecimal::factorial(100));
}

void BigDecimalBench::fact500()
{
	BENCHMARK(BigDecimal::factorial(500));
}

void BigDecimalBench::fact200000()
{
	BENCHMARK(BigDecimal::factorial(200000));
}
