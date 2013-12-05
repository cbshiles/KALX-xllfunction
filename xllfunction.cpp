// xllfunction.cpp - std::function<OPERX(const OPERX&)>
#include "xllfunction.h"

using namespace xll;

static AddInX xai_range(
	FunctionX(XLL_LPOPERX, _T("?xll_range"), _T("RANGE"))
	.Arg(XLL_LPOPERX, _T("Arg"), _T("is an optional argument."))
	.Arg(XLL_LPOPERX, _T("Arg"), _T("is an optional argument."))
	.Arg(XLL_LPOPERX, _T("Arg"), _T("is an optional argument."))
	.Arg(XLL_LPOPERX, _T("Arg"), _T("is an optional argument."))
	.Arg(XLL_LPOPERX, _T("Arg"), _T("is an optional argument."))
	.Arg(XLL_LPOPERX, _T("Arg"), _T("is an optional argument."))
	.Arg(XLL_LPOPERX, _T("Arg"), _T("is an optional argument."))
	.Category(_T("XLL"))
	.FunctionHelp(_T("Return a one column range consisting of Args."))
	.Documentation(_T(""))
);
LPOPERX WINAPI xll_range(LPOPERX pa)
{
#pragma XLLEXPORT
	static OPERX o;

	try {
		o = OPERX();

		LPOPERX* ppa = &pa;
		while ((*ppa)->xltype != xltypeMissing) {
			if ((*ppa)->xltype == xltypeMulti) {
				// range.set???
				// hierarchical multi
				o.push_back(OPERX(xltype::Missing));
				o.back() = *(*ppa);
			}
			else {
				o.push_back(*(*ppa));
			}

			++ppa;
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		o = OPERX(xlerr::NA);
	}

	return &o;
}
}
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
	.FunctionHelp(_T("Return a handle to a function wot curried args."))
	.Documentation(_T(""))
);
HANDLEX WINAPI xll_function_bind(double regid, LPOPERX pa)
{
#pragma XLLEXPORT
	handlex h;

	try {
		handle<function> H(new udf(regid, &pa));

		h = H.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}
/*
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
*/
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
	.Documentation(
		_T("If only one argument of type Multi is passed then assume then ")
		_T("Assume the range contains the individual arguments. ")
	)
);
LPOPERX WINAPI xll_function_call(HANDLEX f, LPOPERX pa)
{
#pragma XLLEXPORT
	static OPERX y;

	try {
		handle<function> F(f);

		if (pa->xltype == xltypeMulti && (&pa)[1]->xltype == xltypeMissing)
			y = (*F)(*pa);
		else
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

#ifdef _DEBUG

int xll_test_function(void)
{
	try {
		// phony up an arg stack
		OPERX a[3];
		LPOPERX pa[3];
		pa[0] = &a[0];
		pa[1] = &a[1];
		pa[2] = &a[2];

		a[0] = OPERX(xltype::Missing);
		a[1] = OPERX(xltype::Missing);
		a[2] = OPERX(xltype::Missing);
		udf f0(xai_mul.RegisterId(), pa);
		a[0] = 2;
		a[1] = 3;
		OPERX o = f0.call(pa);
		ensure (o == 6);

		a[0] = OPERX(4);
		a[1] = OPERX(xltype::Missing);
		udf f1(xai_mul.RegisterId(), pa);
		a[0] = 5;
		a[1] = OPERX(xltype::Missing);
		o = f1.call(pa);
		ensure (o == 20);

		a[0] = OPERX(6);
		a[1] = OPERX(7);
		a[2] = OPERX(xltype::Missing);
		udf f2(xai_mul.RegisterId(), pa);
		a[0] = OPERX(xltype::Missing);
		o = f2.call(pa);
		ensure (o == 42);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}
static Auto<OpenAfterX> xao_test_function(xll_test_function);

#endif // _DEBUG