// xllfunction.cpp - std::function<OPERX(const OPERX&)>
#include "xllfunction.h"

using namespace xll;

static AddInX xai_function(
	FunctionX(XLL_HANDLEX, _T("?xll_function"), _T("XLL.BIND"))
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
//		handle<function> H(new udf(regid, &pa));
		udf f(regid, &pa);
		handle<std::function<LOPERX(const LPOPERX*)>> H(
			new std::function<LOPERX(const LPOPERX*)>(
				[f](const LPOPERX* ppa) -> LOPERX { return f.call(ppa); }
			)
		);

		h = H.get();
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
		handle<std::function<LOPERX(const LPOPERX*)>> pf(f);
		handle<std::function<LOPERX(const LPOPERX*)>> pg(g);
		handle<std::function<LOPERX(const LPOPERX*)>> ph(
			new std::function<LOPERX(const LPOPERX*)>(
				[pf,pg](const LPOPERX* ppa) -> LOPERX { 
					LOPERX f = (*pf)(ppa);
					LOPERX g = (*pg)(ppa); 
					ensure (f.xltype == xltypeNum);
					ensure (g.xltype == xltypeNum);

					f.val.num += g.val.num;

					return f;
				}
			)
		);

		h = ph.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

// allow for multiple arguments???
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
);
LPOPERX WINAPI xll_function_call(HANDLEX f, LPOPERX pa)
{
#pragma XLLEXPORT
	static OPERX y;

	try {
		handle<std::function<LOPERX(const LPOPERX*)>> F(f);

		y = (*F)(&pa);

	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		y = OPERX(xlerr::NA);
	}

	return &y;
}

// XLL.BIND(IDENTITY, Constant) creates a constant function.
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