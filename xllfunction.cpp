// xllfunction.cpp - std::function<OPERX(const OPERX&)>
#include "xllfunction.h"

using namespace xll;

static AddInX xai_function_bind(
	FunctionX(XLL_HANDLEX, _T("?xll_function_bind"), _T("XLL.BIND"))
	.Arg(XLL_DOUBLEX, _T("RegId"), _T("is the register id of a function."))
	.Arg(XLL_LPOPERX, _T("Arg"), _T("is an optional argument to be curried."))
	.Arg(XLL_LPOPERX, _T("Arg"), _T("is an optional argument to be curried."))
	.Arg(XLL_LPOPERX, _T("Arg"), _T("is an optional argument to be curried."))
	.Arg(XLL_LPOPERX, _T("Arg"), _T("is an optional argument to be curried."))
	.Arg(XLL_LPOPERX, _T("Arg"), _T("is an optional argument to be curried."))
	.Arg(XLL_LPOPERX, _T("Arg"), _T("is an optional argument to be curried."))
	.Arg(XLL_LPOPERX, _T("Arg"), _T("is an optional argument to be curried."))
	.Arg(XLL_LPOPERX, _T("Arg"), _T("is an optional argument to be curried."))
	.Uncalced()
	.Category(_T("XLL"))
	.FunctionHelp(_T("Return a handle to a function with curried args."))
	.Documentation(_T(""))
);
HANDLEX WINAPI xll_function_bind(double regid, LPOPERX pa)
{
#pragma XLLEXPORT
	handlex h;

	try {
		handle<function> ph(new function(xll::bind(regid, &pa)));

		h = ph.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

static AddInX xai_function_call(
	FunctionX(XLL_LPOPERX, _T("?xll_function_call"), _T("XLL.CALL"))
	.Arg(XLL_HANDLEX, _T("Function"), _T("is a handle returned by XLL.BIND."))
	.Arg(XLL_LPOPERX, _T("Argument"), _T("is an argument to Function."))
	.Arg(XLL_LPOPERX, _T("Argument"), _T("is an argument to Function."))
	.Arg(XLL_LPOPERX, _T("Argument"), _T("is an argument to Function."))
	.Arg(XLL_LPOPERX, _T("Argument"), _T("is an argument to Function."))
	.Arg(XLL_LPOPERX, _T("Argument"), _T("is an argument to Function."))
	.Arg(XLL_LPOPERX, _T("Argument"), _T("is an argument to Function."))
	.Arg(XLL_LPOPERX, _T("Argument"), _T("is an argument to Function."))
	.Arg(XLL_LPOPERX, _T("Argument"), _T("is an argument to Function."))
	.Category(_T("XLL"))
	.FunctionHelp(_T("Retun the value of the function using the supplied arguments."))
	.Documentation(
		_T("If only one argument of type Multi is passed then assume  ")
		_T("that range contains the individual arguments. ")
	)
);
LPOPERX WINAPI xll_function_call(HANDLEX f, LPOPERX pa)
{
#pragma XLLEXPORT
	static OPERX y;

	try {
		handle<function> pf(f);

		if (pa->xltype == xltypeMulti && (&pa)[1]->xltype == xltypeMissing)
			y = (*pf)(*pa);
		else
			y = (*pf)(range::grab(&pa));

	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		y = OPERX(xlerr::NA);
	}

	return &y;
}

// building block functions

static AddInX xai_function_index(
	FunctionX(XLL_HANDLEX, _T("?xll_function_index"), _T("XLL.INDEX"))
	.Arg(XLL_LPOPERX, _T("Argument"), _T("is the index value of the function returned."))
	.Uncalced()
	.Category(_T("XLL"))
	.FunctionHelp(_T("Return a handle to a index function."))
);
HANDLEX WINAPI xll_function_index(const LPOPERX pi)
{
#pragma XLLEXPORT
	handlex h;

	try {
		OPERX i(*pi);
		handle<function> ph(new function([i](const OPERX& o) { return range::index(o, i); }));

		h = ph.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

static AddInX xai_function_constant(
	FunctionX(XLL_HANDLEX, _T("?xll_function_constant"), _T("XLL.CONSTANT"))
	.Arg(XLL_LPOPERX, _T("Argument"), _T("is the constant value of the function returned."))
	.Uncalced()
	.Category(_T("XLL"))
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
	FunctionX(XLL_HANDLEX, _T("?xll_function_identity"), _T("XLL.IDENTITY"))
	.Uncalced()
	.Category(_T("XLL"))
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
	FunctionX(XLL_HANDLEX, _T("?xll_function_add"), _T("XLL.ADD"))
	.Arg(XLL_HANDLEX, _T("Function"), _T("is a handle to a function."))
	.Arg(XLL_HANDLEX, _T("Function"), _T("is a handle to a function."))
	.Uncalced()
	.Category(_T("XLL"))
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
	FunctionX(XLL_HANDLEX, _T("?xll_function_sub"), _T("XLL.SUB"))
	.Arg(XLL_HANDLEX, _T("Function"), _T("is a handle to a function."))
	.Arg(XLL_HANDLEX, _T("Function"), _T("is a handle to a function."))
	.Uncalced()
	.Category(_T("XLL"))
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
	FunctionX(XLL_HANDLEX, _T("?xll_function_mul"), _T("XLL.MUL"))
	.Arg(XLL_HANDLEX, _T("Function"), _T("is a handle to a function."))
	.Arg(XLL_HANDLEX, _T("Function"), _T("is a handle to a function."))
	.Uncalced()
	.Category(_T("XLL"))
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
	FunctionX(XLL_HANDLEX, _T("?xll_function_div"), _T("XLL.DIV"))
	.Arg(XLL_HANDLEX, _T("Function"), _T("is a handle to a function."))
	.Arg(XLL_HANDLEX, _T("Function"), _T("is a handle to a function."))
	.Uncalced()
	.Category(_T("XLL"))
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
	FunctionX(XLL_HANDLEX, _T("?xll_function_neg"), _T("XLL.NEG"))
	.Arg(XLL_HANDLEX, _T("Function"), _T("is a handle to a function."))
	.Uncalced()
	.Category(_T("XLL"))
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
	.Category(_T("XLL"))
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
	.Category(_T("XLL"))
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
	.Category(_T("XLL"))
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
	.Category(_T("XLL"))
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
	.Category(_T("XLL"))
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
	.Category(_T("XLL"))
	.FunctionHelp(_T("Return the negative of the argument."))
);
double WINAPI xll_neg(double x)
{
#pragma XLLEXPORT
	return -x;
}

#ifdef _DEBUG

typedef traits<XLOPERX>::xword xword;

void xll_test_bind()
{
	// phony up an arg stack
	OPERX a[3];
	LPOPERX pa[3];
	pa[0] = &a[0];
	pa[1] = &a[1];
	pa[2] = &a[2];

	a[0] = OPERX(xltype::Missing);
	a[1] = OPERX(xltype::Missing);
	a[2] = OPERX(xltype::Missing);
	auto f0 = bind(xai_mul.RegisterId(), pa);
	a[0] = 2;
	a[1] = 3;
	OPERX o = f0(range::grab(pa));
	ensure (o == 6);

	a[0] = OPERX(4);
	a[1] = OPERX(xltype::Missing);
	auto f1 = bind(xai_mul.RegisterId(), pa);
	a[0] = 5;
	a[1] = OPERX(xltype::Missing);
	o = f1(range::grab(pa));
	ensure (o == 20);

	a[0] = OPERX(6);
	a[1] = OPERX(7);
	a[2] = OPERX(xltype::Missing);
	auto f2 = bind(xai_mul.RegisterId(), pa);
	a[0] = OPERX(xltype::Missing);
	o = f2(range::grab(pa));
	ensure (o == 42);

	auto f3 = function([](const OPERX& o) { return OPERX(o[0] + o[1]); });
	OPERX o3(1,2);
	o3[0] = 2;
	o3[1] = 3;
	ensure (f3(o3) == 5);
}

void xll_test_op()
{
	function id([](const OPERX& o) { return o; });
	function two([](const OPERX&) { return OPERX(2); });

	auto f = sub(mul(id,id), two);
	ensure (f(OPERX(3)) == 3*3 - 2);

	auto a = add(id,id);
	ensure (a(OPERX(1)) == 2);

	auto s = sub(id,two);
	ensure (s(OPERX(2)) == 0);

	const function& m = mul(id,id);
	ensure (m(OPERX(3)) == 9);

	const function& d(xll::div(id,id));
	ensure (d(OPERX(4)) == 1);

	auto n = neg(id);
	ensure (n(OPERX(5)) == -5);
}

int xll_test_function(void)
{
	try {
		xll_test_bind();
		xll_test_op();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}
static Auto<OpenAfterX> xao_test_function(xll_test_function);

#endif // _DEBUG

