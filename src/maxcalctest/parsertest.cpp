
// Local
#include "parsertest.h"
#include "utility.h"
// Engine
#include "parser.h"

void ParserTest::basic()
{
	ParserContext context;
	Parser parser(_T(""), context);
	FAIL_TEST(context = parser.parse(), "Empty expression", std::exception);
	Complex result;
	FAIL_TEST(result = context.result(), "No result", std::exception);

	parser.setExpression(_T("0"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), 0);

	parser.setExpression(_T("     "));
	FAIL_TEST(context = parser.parse(), "Empty expression", std::exception);

	parser.setExpression(_T("  0 "));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), 0);
}

void ParserTest::numbers()
{
	ParserContext context;
	Parser parser(_T("1"), context);
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), 1);

	parser.setExpression(_T("0"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), 0);

	parser.setExpression(_T("2.56"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), "2.56");

	parser.setExpression(_T("2e-3"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), "0.002");

	parser.setExpression(_T("3e+4"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), 30000);

	parser.setExpression(_T("2.546e-3"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), "0.002546");

	parser.setExpression(_T("2.11e+1"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), "21.1");

	parser.setExpression(_T(".1"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), "0.1");

	parser.setExpression(_T("1."));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), 1);

	parser.setExpression(_T(".0"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), 0);

	parser.setExpression(_T("0."));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), 0);

	parser.setExpression(_T(".256e-2"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), "0.00256");
}

void ParserTest::complexNumbers()
{
	ParserContext context;
	Parser parser(_T("i"), context);
	context = parser.parse();
	Complex result = context.result();
	COMPARE_COMPLEX(context.result(), Complex::i);

	parser.setExpression(_T("1i"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), Complex::i);

	parser.setExpression(_T("i1"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), Complex::i);

	parser.setExpression(_T("0i"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), 0);

	parser.setExpression(_T("2.56i"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), Complex("0", "2.56"));

	parser.setExpression(_T("2e-3i"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), Complex("0", "0.002"));

	parser.setExpression(_T("3e+4i"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), Complex(0, 30000));

	parser.setExpression(_T("2.546e-3i"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), Complex("0", "0.002546"));

	parser.setExpression(_T("2.11e+1i"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), Complex("0", "21.1"));

	parser.setExpression(_T("i0"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), 0);

	parser.setExpression(_T("i2.56"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), Complex("0", "2.56"));

	parser.setExpression(_T("i2e-3"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), Complex("0", "0.002"));

	parser.setExpression(_T("i3e+4"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), Complex(0, 30000));

	parser.setExpression(_T("i2.546e-3"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), Complex("0", "0.002546"));

	parser.setExpression(_T("i2.11e+1"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), Complex("0", "21.1"));

	parser.setExpression(_T(".0i"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), 0);

	parser.setExpression(_T("0.i"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), 0);

	parser.setExpression(_T(".1i"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), Complex("0", "0.1"));

	parser.setExpression(_T("i.1"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), Complex("0", "0.1"));

	parser.setExpression(_T("1.i"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), Complex(0, 1));

	parser.setExpression(_T("i1."));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), Complex(0, 1));

	parser.setExpression(_T(".27863e-2i"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), Complex("0", "0.0027863"));
}

void ParserTest::addSub()
{
	ParserContext context;
	Parser parser(_T("1-2+1"), context);
	context = parser.parse();
	Complex result = context.result();
	COMPARE_COMPLEX(context.result(), 0);

	parser.setExpression(_T("  1  +  2  "));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), 3);

	parser.setExpression(_T("  .1  +  2e-1 - i.1 + 2e-1i  "));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), Complex("0.3", "0.1"));

	parser.setExpression(_T("1-2i+1e-2-.2"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), Complex("0.81", "-2"));
}

void ParserTest::mulDiv()
{
	ParserContext context;
	Parser parser(_T("1/2"), context);
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), "0.5");

	parser.setExpression(_T("1 / 0"));
	FAIL_TEST(parser.parse(), "Division by zero", std::exception);
	COMPARE_COMPLEX(parser.context().result(), "0.5");

	parser.setExpression(_T("1 / i"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), -Complex::i);
}

void ParserTest::unaryPlusMinus()
{
	ParserContext context;
	Parser parser(_T("-1"), context);
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), "-1");

	parser.setExpression(_T("+1"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), "1");

	parser.setExpression(_T("+++++1"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), "1");

	parser.setExpression(_T("-----1"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), "-1");

	parser.setExpression(_T("------1"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), "1");

	parser.setExpression(_T("+-+-+-+-1"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), "1");
}

void ParserTest::power()
{
	ParserContext context;
	Parser parser(_T("2^4"), context);
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), "16");

	// SpeedCrunch fails this test
	parser.setExpression(_T("2^2^3"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), "64");

	// SpeedCrunch fails this test
	parser.setExpression(_T("2^3^2"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), "64");

	parser.setExpression(_T("4 ^ 0.5"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), "2");
}

void ParserTest::powerAndUnaryPlusMinus()
{
	ParserContext context;
	Parser parser(_T("2^+4"), context);
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), "16");

	parser.setExpression(_T("+2^+2^+3"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), "64");

	parser.setExpression(_T("2^-1"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), "0.5");

	parser.setExpression(_T("-2^-1"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), "-0.5");

	parser.setExpression(_T("2^-2"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), "0.25");

	parser.setExpression(_T("-4 ^ 2"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), "16");

	parser.setExpression(_T("-1^0.5"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), Complex(0, 1));

	parser.setExpression(_T(" - -+- 1 ^ +  ++- - 0.5"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), Complex(0, 1));

	parser.setExpression(_T("-1^0.5^2"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), -1);

	parser.setExpression(_T("-1^-0.5^-2"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), -1);
}

void ParserTest::brackets()
{
	ParserContext context;
	Parser parser(_T("(1)"), context);
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), 1);

	parser.setExpression(_T("2*(1+3)"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), 8);

	parser.setExpression(_T("2^(3^2)"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), 512);

	parser.setExpression(_T("2^-(2)"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), "0.25");

	parser.setExpression(_T("(1)(2)"));
	FAIL_TEST(parser.parse(), "Incorrect expression", std::exception);
}

void ParserTest::constsAndVars()
{
	ParserContext context;
	Parser parser(_T("res"), context);
	FAIL_TEST(parser.parse(), "No res", std::exception);

	parser.setExpression(_T("e"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), BigDecimal::E);

	parser.setExpression(_T("pi"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), BigDecimal::PI);

	parser.setExpression(_T("res"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), BigDecimal::PI);
}

void ParserTest::functions()
{
	ParserContext context;
	Parser parser(_T("abs(-1)"), context);
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), 1);

	parser.setExpression(_T("abs(1"));
	FAIL_TEST(parser.parse(), "Incorrect expression", std::exception);

	parser.setExpression(_T("sqr(-2)"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), 4);

	parser.setExpression(_T("sqrt(-4)"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), Complex::i * 2);

	parser.setExpression(_T("pow(2, -2)"));
	context = parser.parse();
	COMPARE_COMPLEX(context.result(), "0.25");

	parser.setExpression(_T("pow(2 -2)"));
	FAIL_TEST(parser.parse(), "Incorrect expression", std::exception);

	parser.setExpression(_T("pow(2,, -2)"));
	FAIL_TEST(parser.parse(), "Incorrect expression", std::exception);

	parser.setExpression(_T("pow(2, -2, 3)"));
	FAIL_TEST(parser.parse(), "Incorrect expression", std::exception);

	parser.setExpression(_T("pow(2, 3"));
	FAIL_TEST(parser.parse(), "Incorrect expression", std::exception);

	parser.setExpression(_T("sin 3"));
	FAIL_TEST(parser.parse(), "Incorrect expression", std::exception);

	parser.setExpression(_T("sin3)"));
	FAIL_TEST(parser.parse(), "Incorrect expression", std::exception);

	parser.setExpression(_T("sin 3)"));
	FAIL_TEST(parser.parse(), "Incorrect expression", std::exception);
}

void ParserTest::fails()
{
	ParserContext context;
	Parser parser(_T("/"), context);
	FAIL_TEST(parser.parse(), "Incorrect expression", std::exception);
	FAIL_TEST(parser.context().result(), "No result", std::exception);

	parser.setExpression(_T("1+"));
	FAIL_TEST(parser.parse(), "Incorrect expression", std::exception);

	parser.setExpression(_T(" 1 * "));
	FAIL_TEST(parser.parse(), "Incorrect expression", std::exception);
}
