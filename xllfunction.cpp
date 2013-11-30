// xllfunction.cpp - std::function<OPERX(const OPERX&)>
#include "xllfunction.h"

using namespace xll;

static AddInX xai_function(
	FunctionX(XLL_HANDLEX, _T("?xll_function"), _T("XLL.FUNCTION"))
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
	.FunctionHelp(_T("Return a handle to a function wot curried args."))
	.Documentation(_T(""))
);
HANDLEX WINAPI xll_function(double regid, LPOPERX pa)
{
#pragma XLLEXPORT
	handlex h;

	try {
		handle<function> H(new function(regid, &pa));

		h = H.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

// allow for multiple arguments???
static AddInX xai_function_call(
	FunctionX(XLL_LPOPERX, _T("?xll_function_call"), _T("XLL.FUNCTION.CALL"))
	.Arg(XLL_HANDLEX, _T("Function"), _T("is a handle returned by XLL.FUNCTION."))
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
);
LPOPERX WINAPI xll_function_call(HANDLEX f, LPOPERX pa)
{
#pragma XLLEXPORT
	static OPERX y;

	try {
		handle<function> F(f);

		y = F->call(&pa);

	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		y = OPERX(xlerr::NA);
	}

	return &y;
}

// XLL.FUNCTION(IDENTITY, Constant) creates a constant function.
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

/*
static AddInX xai_constant(
	FunctionX(XLL_HANDLEX, _T("?xll_constant"), _T("XLL.FUNCTION.CONSTANT"))
	.Arg(XLL_LPOPERX, _T("Constant"), _T("is a cell or range of cells."))
	.Uncalced()
	.Category(_T("XLL")) 
	.FunctionHelp(_T("Return a handle to a function that returns Constant."))
);
HANDLEX WINAPI xll_constant(LPOPERX pc)
{
#pragma XLLEXPORT
	handlex h;

	try {
		handle<function> h_(new function([pc](const OPERX&) { return *pc; }));

		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

static AddInX xai_identity(
	FunctionX(XLL_HANDLEX, _T("?xll_identity"), _T("XLL.FUNCTION.IDENTITY"))
	.Uncalced()
	.Category(_T("XLL"))
	.FunctionHelp(_T("Return Argument."))
);
HANDLEX WINAPI xll_identity(LPOPERX px)
{
#pragma XLLEXPORT
	handlex h;

	try {
		handle<function> h_(new function([](const OPERX& x) { return x; }));

		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

#ifdef _DEBUG

int xll_test_function(void)
{
	try {
		function f([](OPERX x) { x.val.num += 1; return x; });
		function g([](OPERX x) { x.val.num *= 2; return x; });
		function h = f.compose(g);

		OPERX y;
		y = h(OPERX(1));
		ensure (y == 3);

		h = f + g;
		y = h(OPER(1));
		ensure (y == 4);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}
static Auto<OpenAfterX> xao_test_function(xll_test_function);

#endif // _DEBUG

*/