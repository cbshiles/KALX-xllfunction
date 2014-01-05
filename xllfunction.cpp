// xllfunction.cpp - std::function<OPERX(const OPERX&)>
#include "xllfunction.h"

using namespace xll;

static AddInX xai_missing(
	FunctionX(XLL_DOUBLEX, _T("?xll_missing"), CATEGORY_ _T("MISSING"))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return value indicating a missing argument"))
);
double WINAPI xll_missing(void)
{
#pragma XLLEXPORT
	return -0.; // negative zero
}

static AddInX xai_function_identity(
	FunctionX(XLL_LPOPERX, _T("?xll_function_identity"), CATEGORY_ _T("IDENTITY"))
	.Arg(XLL_LPOPERX, _T("Arg"), _T("is and argument."))
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns Arg."))
);
LPOPERX WINAPI xll_function_identity(LPOPERX px)
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
HANDLEX WINAPI xll_function_constant(const LPOPERX pc)
{
#pragma XLLEXPORT
	handlex h;

	try {
		handle<xll::bind> ph(new xll::bind(OPERX(xai_function_identity.RegisterId()), *pc));

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
	LPOPERX pa[XLL_ARGSMAX];
	for (int i = 0; i < XLL_ARGSMAX; ++i) {
		a[i] = OPERX(xltype::Missing);
		pa[i] = &a[i];
	}

	o = Excel<XLOPERX>(xlfProduct, OPERX(2), OPERX(3));
	ensure (o == 6);

	OPERX b(2,1);
	b[0] = 2;
	b[1] = OPERX(xltype::Missing);
	auto f1 = xll::bind(OPERX(xlfProduct), b);
	a[0] = 3;
	o = f1.call(pa);
	ensure (o == 6);

	b[0] = OPERX(xltype::Missing);
	b[1] = 3;
	auto f2 = xll::bind(OPERX(xlfProduct), b);
	a[0] = 2;
	a[1] = OPERX(xltype::Missing);
	o = f2.call(pa);
	ensure (o == 6);

	xll::bind f3(f2);
	o = f3.call(pa);
	ensure (o == 6);

	xll::bind f4;
	f4 = f3;
	o = f4.call(pa);
	ensure (o == 6);

	const xll::function& f5(f4);
	o = f5(pa);
	ensure (o == 6);
}

void xll_test_reify()
{
	xll::bind f(OPERX(xlfSum), OPERX(2));

	OPERX o;
	o = reify(f);
	ensure (o == 2);

	OPERX a(2,1);
	a[0] = 3;
	a[1] = OPERX(xltype::Missing);
	xll::bind f2(OPERX(xlfSum), a);
	o = reify(f2, 4.1);
	ensure (o == 7.1);

	a[1] = 3;
	a[0] = OPERX(xltype::Missing);
	xll::bind f3(OPERX(xlfSum), a);
	o = reify(f3, 4.1);
	ensure (o == 7.1);

	o = reify(f3, o);
	ensure (o == 3 + 7.1);
}

int xll_test_function(void)
{
	try {
		xll_test_bind();
		xll_test_reify();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}
static Auto<OpenAfterX> xao_test_function(xll_test_function);

#endif //  _DEBUG