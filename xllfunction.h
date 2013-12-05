// function.h - function: OPER^n -> OPER
/*
Assemble OPERs? RANGE(o0, o1, ...)
*/
#pragma once
//#include <functional>
#include "../xll8/xll/xll.h"

namespace xll {

	struct function {
		// call from call stack
		LOPERX operator()(const LPOPERX *ppa) const { return call(ppa); };
		// call using multi
		LOPERX operator()(const OPERX& a) const { return call(a); }
		virtual ~function() { }
	private:
		virtual LOPERX call(const LPOPERX*) const = 0;
		virtual LOPERX call(const OPERX&) const = 0;
	};
	
	class udf : public function {
		typedef traits<XLOPERX>::xword xword;

		mutable OPERX arg;
		std::vector<xword> index;
		mutable std::vector<LPXLOPER> parg;
	public:
		udf()
		{ }
		// pass in the address of the first argument on the call stack
		udf(double regid, const LPOPERX* ppa)
		{
			const XAddIn<XLOPERX>* pai = XAddIn<XLOPERX>::Find(regid);
			ensure (pai);
			arg.resize(pai->Args().Arity() + 1, 1);
			parg.resize(arg.size());

			arg[0] = regid;
			for (xword i = 1; i < arg.size(); ++i, ++ppa) {
				arg[i] = *(*ppa); // peel args off the call stack

				if (arg[i].xltype == xltypeMissing)
					index.push_back(i);
			}
		}
		udf(const udf&) = default;
		udf& operator=(const udf&) = default;
		virtual ~udf()
		{ }

		xword arity(void) const
		{
			return arg.size() - 1;
		}

		LOPERX call(const LPOPERX* ppa) const 
		{
			// fill missing args
			for (xword i = 0; i < index.size(); ++i) {
				ensure ((*ppa)->xltype != xltypeMissing || !"arity mismatch");

				arg[index[i]] = *(*ppa);
				++ppa;
			}
			ensure ((*ppa)->xltype == xltypeMissing || !"arity mismatch");

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
		LOPERX call(const OPERX& o) const 
		{
			if (arity() == 1 && index.size() == 1) {
				// handle functions with one arg of type multi
				arg[1] = o;
			}
			else {
				ensure (o.size() == index.size());
				// fill missing args
				for (xword i = 0; i < index.size(); ++i) {
					ensure (o[i].xltype != xltypeMissing || !"arity mismatch");

					arg[index[i]] = o[i];
				}
			}

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

	namespace unary {
		
		class negate : public function {
			const function& f; // const udf& ???
		public:
			negate()
			{ }
			negate(const function& _f)
				: f(_f)
			{ }
			~negate()
			{ }

			LOPERX call(const LPOPERX* ppa)
			{
				LOPERX y = f(ppa);

				if (y.xltype == xltypeNum)
					y.val.num = -y.val.num;
				else if (y.xltype == xltypeBool)
					y.val.xbool = !y.val.xbool;
				else
					throw std::runtime_error("xll::unary::negate: must be number or boolean value");

				return y;
			}
		};

	} // unary

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