// xllcall.cpp
#include "xllfunction.h"

using namespace xll;

#define P4 XLL_LPOPERX XLL_LPOPERX XLL_LPOPERX XLL_LPOPERX 

static AddInX xai_function_call(
	FunctionX(XLL_LPOPERX, _T("?xll_function_call"), CATEGORY_ _T("CALL"))
	.Arg(XLL_HANDLEX, _T("Function"), _T("is a handle returned by XLL.BIND."))
	.Arg(XLL_LPOPERX, _T("Args..."), _T("are zero or more optional argument."))
	.TypeText(P4 P4) // lie about having 8 args + 1 for missing
	.Category(CATEGORY)
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
		handle<xll::function> pf(f);

		y = (*pf)(&pa);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		y = OPERX(xlerr::NA);
	}

	return &y;
}

