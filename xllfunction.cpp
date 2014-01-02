// xllfunction.cpp - std::function<OPERX(const OPERX&)>
#include "xllfunction.h"

using namespace xll;
static AddInX xai_function_identity(
	FunctionX(XLL_LPXLOPERX, _T("?xll_function_identity"), CATEGORY_ _T("IDENTITY"))
	.Arg(XLL_LPXLOPERX, _T("Arg"), _T("is and argument."))
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns Arg."))
);
LPXLOPERX WINAPI xll_function_identity(LPXLOPERX px)
{
#pragma XLLEXPORT
	return px;
}

// building block functions
static AddInX xai_function_constant(
	FunctionX(XLL_HANDLEX, _T("?xll_function_constant"), CATEGORY_ _T("CONSTANT"))
	.Arg(XLL_LPOPERX, _T("Argument"), _T("is the constant value of the function returned."))
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(_T("Return a handle to a constant function."))
);
HANDLEX WINAPI xll_function_constant(const LPXLOPERX pc)
{
#pragma XLLEXPORT
	handlex h;

	try {
		handle<xll::bind> ph(new xll::bind(OPERX(xai_function_identity.RegisterId()), &pc));

		h = ph.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

#ifdef _DEBUG

typedef traits<XLOPERX>::xword xword;

void xll_test_bind()
{
	// phony up an arg stack
	OPERX o, a[XLL_ARGSMAX];
	LPXLOPERX pa[XLL_ARGSMAX];
	for (int i = 0; i < XLL_ARGSMAX; ++i) {
		a[i] = OPERX(xltype::Missing);
		pa[i] = &a[i];
	}

	o = Excel<XLOPERX>(xlfProduct, OPERX(2), OPERX(3));
	ensure (o == 6);

	a[0] = 2;
	a[1] = OPERX(xltype::Missing);
	auto f1 = xll::bind(OPERX(xlfProduct), &pa[0]);
	a[1] = 3;
	o = f1.call(pa + 1);
	ensure (o == 6);

	a[0] = OPERX(xltype::Missing);
	a[1] = 3;
	auto f2 = xll::bind(OPERX(xlfProduct), &pa[0]);
	a[0] = 2;
	a[1] = OPERX(xltype::Missing);
	o = f2.call(pa);
	ensure (o == 6);
}

int xll_test_function(void)
{
	try {
		xll_test_bind();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}
static Auto<OpenAfterX> xao_test_function(xll_test_function);
static AddInX xai_test_function(_T("?xll_test_fun"), _T("TEST.FUN"));
int WINAPI xll_test_fun()
{
#pragma XLLEXPORT
	return xll_test_function();
}

#endif //  _DEBUG