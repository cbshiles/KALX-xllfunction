// function.h - function: OPER^n -> OPER
#pragma once
//#include <functional>
#include "../xll8/xll/xll.h"

namespace xll {

	class function {
		typedef traits<XLOPERX>::xword xword;

		mutable OPERX arg;
		std::vector<xword> index;
	public:
		function()
		{ }
		// pass in the address of the first argument on the call stack
		function(double regid, LPOPERX* ppa)
		{
			const XAddIn<XLOPERX>* pai = XAddIn<XLOPERX>::Find(regid);
			ensure (pai);
			arg.resize(pai->Args().Arity() + 1, 1);
			arg[0] = regid;
			for (xword i = 1; i < arg.size(); ++i, ++ppa) {
				arg[i] = *(*ppa); // peel args off the call stack
				if (arg[i].xltype == xltypeMissing)
					index.push_back(i);
			}
		}
		~function()
		{ }

		LOPERX call(LPOPERX* ppa) const {
			for (xword i = 0; i < index.size(); ++i) {
				ensure ((*ppa)->xltype != xltypeMissing || !"arity mismatch");

				arg[index[i]] = *(*ppa);
				++ppa;
			}
			ensure ((*ppa)->xltype == xltypeMissing || !"arity mismatch");

			std::vector<LPXLOPER> parg(arg.size());
			for (xword i = 0; i < parg.size(); ++i)
				parg[i] = &arg[i];

			LOPERX ret;
			ensure (xlretSuccess == xll::traits<XLOPERX>::Excelv(xlUDF, &ret, parg.size(), &parg[0]));

			// unfill missing args
			for (xword i = 0; i < index.size(); ++i) {
				arg[index[i]] = OPERX(xltype::Missing);
			}

			return ret;
		}
	};

} // xll

/*
namespace detail {
	template<class Op>
	inline xll::function operator_op(Op op, const xll::function& f, const xll::function& g)
	{
		return xll::function([&op, &f,&g](const OPERX& x) { return OPERX(op(f(x).val.num, g(x).val.num)); });
	}
}

#define OPERATOR_OP(op,fun) \
inline xll::function operator op (const xll::function& f, const xll::function& g) \
	{ return detail::operator_op(fun<double>(), f, g); }

OPERATOR_OP(+, std::plus)
OPERATOR_OP(-, std::minus)
OPERATOR_OP(*, std::multiplies)
OPERATOR_OP(/, std::divides)

#undef OPERATOR_OP
*/