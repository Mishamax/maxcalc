
// Local
#include "parsertest.h"
#include "utility.h"
// Engine
#include "parser.h"

void ParserTest::basic()
{
	ParserContext context;
	Parser parser(_T(""), context);
	FAIL_TEST(parser.parse(), "Empty expression", std::exception);
	FAIL_TEST(parser.context().result(), "No result", std::exception);
	context = parser.context();
	FAIL_TEST(context.result(), "No result", std::exception);

	PARSER_TEST(parser, _T("0"), 0);

	PARSER_FAIL_TEST(parser, _T("     "), "Empty expression", std::exception);

	PARSER_TEST(parser, _T("  0 "), 0);
}

void ParserTest::numbers()
{
	Parser parser;

	PARSER_TEST(parser, _T("1"), 1);
	PARSER_TEST(parser, _T("0"), 0);
	PARSER_TEST(parser, _T("2.56"), "2.56");
	PARSER_TEST(parser, _T("2e-3"), "0.002");
	PARSER_TEST(parser, _T("3e+4"), 30000);
	PARSER_TEST(parser, _T("2.546e-3"), "0.002546");
	PARSER_TEST(parser, _T("2.11e+1"), "21.1");
	PARSER_TEST(parser, _T(".1"), "0.1");
	PARSER_TEST(parser, _T(".0"), 0);
	PARSER_TEST(parser, _T("0."), 0);
	PARSER_TEST(parser, _T(".256e-2"), "0.00256");
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

	PARSER_TEST(parser, _T("0+0"), "0");
	PARSER_TEST(parser, _T("0+1"), "1");
	PARSER_TEST(parser, _T("1+0"), "1");
	PARSER_TEST(parser, _T("1+1"), "2");
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

	PARSER_FAIL_TEST(parser, _T("1 / sin(0)"), "Division by zero", std::exception);
	PARSER_FAIL_TEST(parser, _T("1/sin(pi)"), "Division by zero", std::exception);
	PARSER_FAIL_TEST(parser, _T(" 1 / cos ( pi / 2 ) "), "Division by zero", std::exception);
	PARSER_FAIL_TEST(parser, _T("1 / tan(0)"), "Division by zero", std::exception);
	PARSER_FAIL_TEST(parser, _T("1 / ctan(0)"), "Division by zero", std::exception);
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

	PARSER_TEST(parser, _T("-0"), 0);
	PARSER_TEST(parser, _T("--1"), 1);
	PARSER_TEST(parser, _T("---1"), -1);
	PARSER_TEST(parser, _T("----1"), 1);
	PARSER_TEST(parser, _T("-abs(1)"), -1);
	PARSER_TEST(parser, _T("-abs(-2)"), -2);
	PARSER_TEST(parser, _T("--abs(-3)"), 3);
	PARSER_TEST(parser, _T("---abs(-4)"), -4);
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

	PARSER_TEST(parser, _T("0^0"), 1);
	PARSER_TEST(parser, _T("1^0"), "1" );
	PARSER_TEST(parser, _T("1^1"), "1" );
	PARSER_TEST(parser, _T("1^2"), "1" );
	PARSER_TEST(parser, _T("1^3"), "1" );

	PARSER_TEST(parser, _T("-1^0"), "1"  );
	PARSER_TEST(parser, _T("-1^1"), "-1" );
	PARSER_TEST(parser, _T("-1^2"), "1"  );
	PARSER_TEST(parser, _T("-1^3"), "-1" );

	PARSER_TEST(parser, _T("2^0"), "1"     );
	PARSER_TEST(parser, _T("2^1"), "2"     );
	PARSER_TEST(parser, _T("2^2"), "4"     );
	PARSER_TEST(parser, _T("2^3"), "8"     );
	PARSER_TEST(parser, _T("2^4"), "16"    );
	PARSER_TEST(parser, _T("2^5"), "32"    );
	PARSER_TEST(parser, _T("2^6"), "64"    );
	PARSER_TEST(parser, _T("2^7"), "128"   );
	PARSER_TEST(parser, _T("2^8"), "256"   );
	PARSER_TEST(parser, _T("2^9"), "512"   );
	PARSER_TEST(parser, _T("2^10"), "1024" );

	PARSER_TEST(parser, _T("-2^0"), "1"     );
	PARSER_TEST(parser, _T("-2^1"), "-2"     );
	PARSER_TEST(parser, _T("-2^2"), "4"     );
	PARSER_TEST(parser, _T("-2^3"), "-8"     );
	PARSER_TEST(parser, _T("-2^4"), "16"    );
	PARSER_TEST(parser, _T("-2^5"), "-32"    );
	PARSER_TEST(parser, _T("-2^6"), "64"    );
	PARSER_TEST(parser, _T("-2^7"), "-128"   );
	PARSER_TEST(parser, _T("-2^8"), "256"   );
	PARSER_TEST(parser, _T("-2^9"), "-512"   );
	PARSER_TEST(parser, _T("-2^10"), "1024" );

	PARSER_TEST(parser, _T("(5-7)^2"),  "4"  );
	PARSER_TEST(parser, _T("-(5-7)^2"), "4" );
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

void ParserTest::functionsBasic()
{
	Parser parser;

	PARSER_TEST(parser, _T("ABS(0)"),    "0" );
	PARSER_TEST(parser, _T("ABS(1)"),    "1" );
	PARSER_TEST(parser, _T("ABS(-1)"),   "1" );
	PARSER_TEST(parser, _T("ABS(--1)"),  "1" );
	PARSER_TEST(parser, _T("ABS(---1)"), "1" );
	PARSER_TEST(parser, _T("ABS((1))"),  "1" );
	PARSER_TEST(parser, _T("ABS((-1))"), "1" );

	PARSER_TEST(parser, _T("ABS(1/2)"),   "0.5" );
	PARSER_TEST(parser, _T("ABS(-1/2)"),  "0.5" );
	PARSER_TEST(parser, _T("ABS(-1/-2)"), "0.5" );
	PARSER_TEST(parser, _T("ABS(1/-2)"),  "0.5" );
}

void ParserTest::functionsTrig()
{
	Parser parser;

	PARSER_TEST(parser, _T("pi"),    "3.14159265358979323846264338327950288419716939937511" );

	PARSER_TEST(parser, _T("sin(0)"), "0" );
	PARSER_TEST(parser, _T("cos(0)"), "1" );
	PARSER_TEST(parser, _T("tan(0)"), "0" );

	PARSER_TEST(parser, _T("sin(pi)"), "0"  );
	PARSER_TEST(parser, _T("cos(pi)"), "-1" );
	PARSER_TEST(parser, _T("tan(pi)"), "0"  );

	PARSER_TEST(parser, _T("sin(-pi)"), "0"  );
	PARSER_TEST(parser, _T("cos(-pi)"), "-1" );
	PARSER_TEST(parser, _T("tan(-pi)"), "0"  );

	PARSER_TEST(parser, _T("sin(--pi)"), "0"  );
	PARSER_TEST(parser, _T("cos(--pi)"), "-1" );
	PARSER_TEST(parser, _T("tan(--pi)"), "0"  );

	PARSER_TEST(parser, _T("sin(pi/2)"), "1" );
	PARSER_TEST(parser, _T("cos(pi/2)"), "0" );

	PARSER_TEST(parser, _T("sin(-pi/2)"), "-1" );
	PARSER_TEST(parser, _T("cos(-pi/2)"), "0"  );

	PARSER_TEST(parser, _T("sin(-pi/2) + sin(pi/2)"), "0"  );
	PARSER_TEST(parser, _T("sin(-pi/2) - sin(pi/2)"), "-2" );
	PARSER_TEST(parser, _T("cos(-pi/2) + cos(pi/2)"), "0"  );
	PARSER_TEST(parser, _T("cos(-pi/2) - cos(pi/2)"), "0"  );

	PARSER_TEST(parser, _T("asin(sin(1))"), "1" );
	PARSER_TEST(parser, _T("acos(cos(1))"), "1" );
}

void ParserTest::functionsLog()
{
	Parser parser;

	PARSER_TEST(parser, _T("exp((1)/2) + exp((1)/2)"),    "3.29744254140025629369730157562832714330755220142030" );
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

void ParserTest::realWorld()
{
	Parser parser;

	PARSER_TEST(parser, _T("-0.25-2.43*sqrt(0.52)"), "-2.00229791987549879645594153599066102987813013488879");
	PARSER_TEST(parser, _T("-1/(1000-1)"), "-0.001001001001001001001001001001001001001001001001001001001001001001");
	PARSER_TEST(parser, _T("(1000^2)/(((1000-1)^2)/(1000-2))"), "999.99899799699599499399299199098998898798698598498398");
	PARSER_TEST(parser, _T("sqrt(res)"), "31.62276075862124968674132991108077921161867982304985");
	PARSER_TEST(parser, _T("sqrt((1000^2)/(((1000-1)^2)*(1000-2)))"), "0.03168613302467059086847828648404887696554977938181347253796979483");
	PARSER_TEST(parser, _T("sqrt(result)"), "0.1780059915414944421035302560547959211378834659058566185708033849");
	PARSER_TEST(parser, _T("19*19"), "361");
	PARSER_TEST(parser, _T("81+16*18"), "369");
	PARSER_TEST(parser, _T("18 * 81"), "1458");
	PARSER_TEST(parser, _T("(1440-171)/361"), "3.51523545706371191135734072022160664819944598337950");
	PARSER_TEST(parser, _T("(2708-1064-1269)/361"), "1.03878116343490304709141274238227146814404432132964");
	PARSER_TEST(parser, _T("0.88*0.95"), "0.836");
	PARSER_TEST(parser, _T("log10(110)/1.05"), "1.944183509674500038809714258326689754006383038539479137710989542");
	PARSER_TEST(parser, _T("log10(110)/1.0528748"), "1.93887505443023713811955606805578805923240084240448");
	PARSER_TEST(parser, _T("0.4444 + 1.084*exp(-0.2034*2) - 0.09168*sin(1*2+0.1123) - 0.565*exp(-1.067*2) + 0.06262*exp(-5.086*2) - 0.01573*exp(-8.578*2) + 0.000413*exp(-18.07*2)"), "1.02066622043541853227435085151712578946216128386806");
	PARSER_TEST(parser, _T("2/0.05"), "40");
	PARSER_TEST(parser, _T("0.4444 + 1.084*(-0.2034)*exp(-0.2034*2) - 0.09168*cos(1*2+0.1123) - 0.565*(-1.067)*exp(-1.067*2) + 0.06262*(-5.086)*exp(-5.086*2) - 0.01573*(-8.578)*exp(-8.578*2) + 0.000413*(-18.07)*exp(-18.07*2)"), "0.41620335366185292064835042124058419491838127532895");
	PARSER_TEST(parser, _T("0.4444 + 1.084*((-0.2034)^4)*exp(-0.2034*2) -0.09168*sin(1*2+0.1123) - 0.565*((-1.067)^4)*exp(-1.067*2) + 0.06262*((-5.086)^4)*exp(-5.086*2) - 0.01573*((-8.578)^4)*exp(-8.578*2) + 0.000413*((-18.07)^4)*exp(-18.07*2)"), "0.28198960676373864448299646367266679390878957236408");
	PARSER_TEST(parser, _T("(0.02+9*0.01+0.1)/19"), "0.01105263157894736842105263157894736842105263157894736842105263158");
	PARSER_TEST(parser, _T("((492-0.25*1000)^2)/(0.25*1000)"), "234.25600000000000000000000000000000000000000000000000");
	PARSER_TEST(parser, _T("0.6396*ln(0.9784)^2*(0.9784)^(3*10) - 0.121*ln(0.911)^2*(0.911)^(3*10) - 0.7423*ln(0.9048)^2*(0.9048)^(3*10) + 0.4112*ln(0.5778)^2*(0.5778)^(3*10) - 0.187*ln(0.4351)^2*(0.4351)^(3*10) - 0.0004501*ln(0.1583)^2*(0.1583)^(3*10)"), "-2.751631655951235923961285515679582556626416401198469537065650108e-4");
}
