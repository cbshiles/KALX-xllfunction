// xllfunction.cpp - std::function<OPERX(const OPERX&)>
#include "xllfunction.h"

using namespace xll;

// building block functions
#if 0
static AddInX xai_function_index(
	FunctionX(XLL_HANDLEX, _T("?xll_function_index"), CATEGORY_ _T("PICK"))
	.Arg(XLL_LPOPERX, _T("Argument"), _T("is the index value of the function returned."))
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(_T("Return a handle to a index function."))
);
HANDLEX WINAPI xll_function_index(const LPOPERX pi)
{
#pragma XLLEXPORT
	handlex h;

	try {
		OPERX i(*pi);
		handle<function> ph(new function([i](const OPERX& o) { return range::pick(o, i); }));

		h = ph.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

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
		OPERX c(*pc);
		handle<function> ph(new function([c](const OPERX&) { return c; }));

		h = ph.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}
static AddInX xai_function_identity(
	FunctionX(XLL_HANDLEX, _T("?xll_function_identity"), CATEGORY_ _T("IDENTITY"))
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(_T("Return a handle to a identity function."))
);
HANDLEX WINAPI xll_function_identity()
{
#pragma XLLEXPORT
	handlex h;

	try {
		handle<function> ph(new function([](const OPERX& x) { return x; }));

		h = ph.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

static AddInX xai_function_add(
	FunctionX(XLL_HANDLEX, _T("?xll_function_add"), CATEGORY_ _T("ADD"))
	.Arg(XLL_HANDLEX, _T("Function"), _T("is a handle to a function."))
	.Arg(XLL_HANDLEX, _T("Function"), _T("is a handle to a function."))
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(_T("Return a handle to the sum of functions."))
	.Documentation(_T(""))
);
HANDLEX WINAPI xll_function_add(HANDLEX f, HANDLEX g)
{
#pragma XLLEXPORT
	handlex h;

	try {
		handle<function> pf(f);
		handle<function> pg(g);

		handle<function> ph(new add(*pf, *pg));

		h = ph.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}
static AddInX xai_function_sub(
	FunctionX(XLL_HANDLEX, _T("?xll_function_sub"), CATEGORY_ _T("SUB"))
	.Arg(XLL_HANDLEX, _T("Function"), _T("is a handle to a function."))
	.Arg(XLL_HANDLEX, _T("Function"), _T("is a handle to a function."))
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(_T("Return a handle to the difference of functions."))
	.Documentation(_T(""))
);
HANDLEX WINAPI xll_function_sub(HANDLEX f, HANDLEX g)
{
#pragma XLLEXPORT
	handlex h;

	try {
		handle<function> pf(f);
		handle<function> pg(g);

		handle<function> ph(new sub(*pf, *pg));

		h = ph.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}
static AddInX xai_function_mul(
	FunctionX(XLL_HANDLEX, _T("?xll_function_mul"), CATEGORY_ _T("MUL"))
	.Arg(XLL_HANDLEX, _T("Function"), _T("is a handle to a function."))
	.Arg(XLL_HANDLEX, _T("Function"), _T("is a handle to a function."))
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(_T("Return a handle to the product of functions."))
	.Documentation(_T(""))
);
HANDLEX WINAPI xll_function_mul(HANDLEX f, HANDLEX g)
{
#pragma XLLEXPORT
	handlex h;

	try {
		handle<function> pf(f);
		handle<function> pg(g);

		handle<function> ph(new mul(*pf, *pg));

		h = ph.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}
static AddInX xai_function_div(
	FunctionX(XLL_HANDLEX, _T("?xll_function_div"), CATEGORY_ _T("DIV"))
	.Arg(XLL_HANDLEX, _T("Function"), _T("is a handle to a function."))
	.Arg(XLL_HANDLEX, _T("Function"), _T("is a handle to a function."))
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(_T("Return a handle to the quotient of functions."))
	.Documentation(_T(""))
);
HANDLEX WINAPI xll_function_div(HANDLEX f, HANDLEX g)
{
#pragma XLLEXPORT
	handlex h;

	try {
		handle<function> pf(f);
		handle<function> pg(g);

		handle<function> ph(new xll::div(*pf, *pg));

		h = ph.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}
static AddInX xai_function_neg(
	FunctionX(XLL_HANDLEX, _T("?xll_function_neg"), CATEGORY_ _T("NEG"))
	.Arg(XLL_HANDLEX, _T("Function"), _T("is a handle to a function."))
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(_T("Return a handle to the negative of Function."))
	.Documentation(_T(""))
);
HANDLEX WINAPI xll_function_neg(HANDLEX f)
{
#pragma XLLEXPORT
	handlex h;

	try {
		handle<function> pf(f);

		handle<function> ph(new neg(*pf));

		h = ph.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

// primative functions
static AddInX xai_identity(
	FunctionX(XLL_LPOPERX, _T("?xll_identity"), _T("IDENTITY"))
	.Arg(XLL_LPOPERX, _T("Argument"), _T("is the argument to be returned."))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return Argument unmodified."))
);
LPOPERX WINAPI xll_identity(LPOPERX px)
{
#pragma XLLEXPORT
	return px;
}
static AddInX xai_add(
	FunctionX(XLL_DOUBLEX, _T("?xll_add"), _T("ADD"))
	.Num(_T("x"), _T("is the first number of the sum."))
	.Num(_T("y"), _T("is the second number of the sum."))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return the sum of the arguments"))
);
double WINAPI xll_add(double x, double y)
{
#pragma XLLEXPORT
	return x + y;
}
static AddInX xai_sub(
	FunctionX(XLL_DOUBLEX, _T("?xll_sub"), _T("SUB"))
	.Num(_T("x"), _T("is the first number of the difference."))
	.Num(_T("y"), _T("is the second number of the difference."))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return the difference of the arguments"))
);
double WINAPI xll_sub(double x, double y)
{
#pragma XLLEXPORT
	return x - y;
}
static AddInX xai_mul(
	FunctionX(XLL_DOUBLEX, _T("?xll_mul"), _T("MUL"))
	.Num(_T("x"), _T("is the first number of the product."))
	.Num(_T("y"), _T("is the second number of the product."))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return the product of the arguments"))
);
double WINAPI xll_mul(double x, double y)
{
#pragma XLLEXPORT
	return x*y;
}
static AddInX xai_div(
	FunctionX(XLL_DOUBLEX, _T("?xll_div"), _T("DIV"))
	.Num(_T("x"), _T("is the first number of the quotient."))
	.Num(_T("y"), _T("is the second number of the quotient."))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return the quotient of the arguments"))
);
double WINAPI xll_div(double x, double y)
{
#pragma XLLEXPORT
	return x/y;
}
static AddInX xai_neg(
	FunctionX(XLL_DOUBLEX, _T("?xll_neg"), _T("NEG"))
	.Num(_T("x"), _T("is a number."))
	.Category(CATEGORY)
	.FunctionHelp(_T("Return the negative of the argument."))
);
double WINAPI xll_neg(double x)
{
#pragma XLLEXPORT
	return -x;
}
#endif // 0
#ifdef _DEBUG

typedef traits<XLOPERX>::xword xword;

void xll_test_bind()
{
	// phony up an arg stack
	OPERX a[XLL_ARGSMAX];
	LPXLOPERX pa[XLL_ARGSMAX];
	for (int i = 0; i < XLL_ARGSMAX; ++i) {
		a[i] = OPERX(xltype::Missing);
		pa[i] = &a[i];
	}

	auto f0 = xll::bind(OPERX(xlfProduct), &pa[0]);

	OPERX o;
	a[0] = 2;
	a[1] = 3;

	o = Excel<XLOPERX>(xlfProduct, a[0], a[1]);
	o = f0.call(pa);
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

#endif // _DEBUG

