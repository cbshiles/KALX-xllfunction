// function.h - function: OPER^n -> OPER
/*
Assemble OPERs? RANGE(o0, o1, ...)
*/
#pragma once
#include <functional>
#include "xllrange.h"

namespace xll { 

	// base class for all functions
	class function {
	protected:
		std::function<OPERX(const OPERX&)> f;
	public:
 		function()
		{ }
		function (const std::function<OPERX(const OPERX&)> _f)
			: f(_f)
		{ }
		function(const function&) = default;
		function& operator=(const function&) = default;
		virtual ~function()
		{ }

		OPERX operator()(const OPERX& o) const
		{
			return f(o);
		}
	};

	// register id and curried args
	class bind : public function {
		typedef traits<XLOPERX>::xword xword;
		OPERX arg;
		std::vector<xword> ind;
	public:
		bind()
			: function()
		{ }
		bind(double regid, const LPOPERX* ppa)
		{
			const XAddIn<XLOPERX>* pai;		
			ensure (0 != (pai = XAddIn<XLOPERX>::Find(regid)));

			std::vector<xword> ind;
			OPERX arg(1, pai->Args().Arity() + 1); // regid, args...

			arg[0] = regid;
			for (xword i = 1; i < arg.size(); ++i, ++ppa) {
				arg[i] = *(*ppa); // peel args off the call stack
				if (arg[i].xltype == xltypeMissing)
					ind.push_back(i);
			}
		
			f = [arg,ind](const OPERX& a) {
				ensure (ind.size() == a.size());

				OPERX arg_(arg);

				std::vector<const LPXLOPERX> parg(arg.size());
				// fill missing args
				for (xword i = 0; i < ind.size(); ++i) {
					ensure (a[i].xltype != xltypeMissing || !"arity mismatch");

					arg_[ind[i]] = a[i];
				}

				for (xword i = 0; i < parg.size(); ++i)
					parg[i] = &arg_[i];

				LOPERX ret;
				ensure (xlretSuccess == xll::traits<XLOPERX>::Excelv(xlUDF, &ret, parg.size(), &parg[0]));

				return ret;
			};
		}
	};

	struct add : public function {
		add(const function& f, const function& g)
			: function([f,g](const OPERX& o) { return OPERX(f(o) + g(o)); })
		{ }
		add(const add&) = default;
		add& operator=(const add&) = default;
		~add()
		{ }
	};
	struct sub : public function {
		sub(const function& f, const function& g)
			: function([f,g](const OPERX& o) { return OPERX(f(o) - g(o)); })
		{ }
		sub(const sub&) = default;
		sub& operator=(const sub&) = default;
		~sub()
		{ }
	};
	struct mul : public function {
		mul(const function& f, const function& g)
			: function([f,g](const OPERX& o) { return OPERX(f(o) * g(o)); })
		{ }
		mul(const mul&) = default;
		mul& operator=(const mul&) = default;
		~mul()
		{ }
	};
	struct div : public function {
		div(const function& f, const function& g)
			: function([f,g](const OPERX& o) { return OPERX(f(o) / g(o)); })
		{ }
		div(const div&) = default;
		div& operator=(const div&) = default;
		~div()
		{ }
	};
	struct neg : public function {
		neg(const function& f)
			: function([f](const OPERX& o) { return OPERX(-f(o)); })
		{ }
		neg(const neg&) = default;
		neg& operator=(const neg&) = default;
		~neg()
		{ }
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