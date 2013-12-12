// xllrange.cpp
#include "xllrange.h"

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
