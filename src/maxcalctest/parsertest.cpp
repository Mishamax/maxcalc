
// Local
#include "parsertest.h"
#include "utility.h"
// Engine
#include "parser.h"

void ParserTest::addSub()
{
	ParserContext context;
	Parser parser(_T("1-2+1"), context);
	context = parser.parse();
	Complex result = context.getResult();
	COMPARE_COMPLEX(context.getResult(), 0);
}

void ParserTest::mulDiv()
{
	ParserContext context;
	Parser parser(_T("1/2"), context);
	context = parser.parse();
	Complex result = context.getResult();
	COMPARE_COMPLEX(context.getResult(), "0.5");
}
