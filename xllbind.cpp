// xllbind.cpp
#include "xllfunction.h"

using namespace xll;

static AddInX xai_function_bind(
	FunctionX(XLL_HANDLEX, _T("?xll_function_bind"), CATEGORY_ _T("BIND"))
	.Arg(XLL_LPOPERX, _T("Function"), _T("is the register id or name of a function."))

	.Arg(XLL_LPOPERX, _T("Args..."), _T("is an optional argument to be curried."))
	.Arg(XLL_LPOPERX, _T(""), _T("is an optional argument to be curried."))
	.Arg(XLL_LPOPERX, _T(""), _T("is an optional argument to be curried."))
	.Arg(XLL_LPOPERX, _T(""), _T("is an optional argument to be curried."))
// 4
	.Arg(XLL_LPOPERX, _T(""), _T("is an optional argument to be curried."))
	.Arg(XLL_LPOPERX, _T(""), _T("is an optional argument to be curried."))
	.Arg(XLL_LPOPERX, _T(""), _T("is an optional argument to be curried."))
	.Arg(XLL_LPOPERX, _T(""), _T("is an optional argument to be curried."))
// 8
	.Arg(XLL_LPOPERX, _T(""), _T("is an optional argument to be curried."))
	.Arg(XLL_LPOPERX, _T(""), _T("is an optional argument to be curried."))
	.Arg(XLL_LPOPERX, _T(""), _T("is an optional argument to be curried."))
	.Arg(XLL_LPOPERX, _T(""), _T("is an optional argument to be curried."))

	.Arg(XLL_LPOPERX, _T(""), _T("is an optional argument to be curried."))
	.Arg(XLL_LPOPERX, _T(""), _T("is an optional argument to be curried."))
	.Arg(XLL_LPOPERX, _T(""), _T("is an optional argument to be curried."))
	.Arg(XLL_LPOPERX, _T(""), _T("is an optional argument to be curried."))
//16
	//.Arg(XLL_LPOPERX, _T(""), _T("is an optional argument to be curried."))
	//.Arg(XLL_LPOPERX, _T(""), _T("is an optional argument to be curried."))
	//.Arg(XLL_LPOPERX, _T(""), _T("is an optional argument to be curried."))
	//.Arg(XLL_LPOPERX, _T(""), _T("is an optional argument to be curried."))

	//.Arg(XLL_LPOPERX, _T(""), _T("is an optional argument to be curried."))
	//.Arg(XLL_LPOPERX, _T(""), _T("is an optional argument to be curried."))
	//.Arg(XLL_LPOPERX, _T(""), _T("is an optional argument to be curried."))
	//.Arg(XLL_LPOPERX, _T(""), _T("is an optional argument to be curried."))

	//.Arg(XLL_LPOPERX, _T(""), _T("is an optional argument to be curried."))
	//.Arg(XLL_LPOPERX, _T(""), _T("is an optional argument to be curried."))
	//.Arg(XLL_LPOPERX, _T(""), _T("is an optional argument to be curried."))
	//.Arg(XLL_LPOPERX, _T(""), _T("is an optional argument to be curried."))

	//.Arg(XLL_LPOPERX, _T(""), _T("is an optional argument to be curried."))
	//.Arg(XLL_LPOPERX, _T(""), _T("is an optional argument to be curried."))
//	.Arg(XLL_LPOPERX, _T(""), _T("is an optional argument to be curried."))
//	.Arg(XLL_LPOPERX, _T(""), _T("is an optional argument to be curried."))
// 32
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(_T("Return a handle to a function with curried args."))
	.Documentation(_T(""))
);
HANDLEX WINAPI xll_function_bind(LPOPERX func, const LPXLOPERX pa /*, up to 32 total */)
{
#pragma XLLEXPORT
	handlex h;

	try {
		handle<xll::bind> ph(new xll::bind(*func, &pa));

		h = ph.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

