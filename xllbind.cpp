// xllbind.cpp
#include "xllfunction.h"

using namespace xll;

static AddInX xai_function_bind(
	FunctionX(XLL_HANDLEX, _T("?xll_function_bind"), CATEGORY_ _T("BIND"))
	.Arg(XLL_LPOPERX, _T("Function"), _T("is the register id or name of a function."))
	.Arg(XLL_LPOPERX, _T("Args..."), _T("is an range of optional argument to be curried."))
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(_T("Return a handle to a function with curried args. Use XLL.MISSING() to indicate missing arguments."))
	.Documentation(_T(""))
);
HANDLEX WINAPI xll_function_bind(LPOPERX func, const LPOPERX pa)
{
#pragma XLLEXPORT
	handlex h;

	try {
		handle<xll::function> ph(new xll::bind(*func, *pa));

		h = ph.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

