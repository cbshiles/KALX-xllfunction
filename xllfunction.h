// function.h - function: OPER -> OPER
/*
XLL.BIND(Function, Range) takes two arguments where size(Range) = arity of Function
Use XLL.MISSING() to specify non curried arguments
XLL.CALL(Function, Args...) can take one arg range, or a list of arguments
*/
#pragma once
//#define EXCEL12
#include <functional>
#include <memory>
#include "../xll8/xll/xll.h"
//#include "xllrange.h"

#define CATEGORY _T("XLL")
#define CATEGORY_ CATEGORY _T(".")

#define XLL_ARGSMAX 8

namespace xll {

	inline bool is_missing(const OPERX& o)
	{
		return o.xltype == xltypeMissing || o.xltype == xltypeNum && o.val.num == 0 && copysign(1, o.val.num) < 0;
	}

	class function {
	public:
		function()
		{ }
		virtual ~function() 
		{ }
		OPERX operator()(const LPOPERX* ppa) const
		{
			return call(ppa);
		}
	private:
		virtual OPERX call(const LPOPERX*) const = 0;
	};

	// arbitrary list of arguments that can be converted to OPERs
	template<class ...A>
	inline OPERX reify(const xll::function& f, const A& ...as)
	{
		std::vector<OPERX> a = {OPERX(as...)};
		a.push_back(OPERX(xltype::Missing));
		std::vector<LPOPERX> ppa(a.size());

		for (size_t i = 0; i < a.size(); ++i)
			ppa[i] = &a[i];

		return f(&ppa[0]);
	}

	// register id and curried args
	class bind : public function {
		typedef xll::traits<XLOPERX>::xword xword;
		std::vector<xword> ind;
		std::vector<OPERX> arg;
		mutable std::vector<LPXLOPER> parg;
		const xll::XAddIn<XLOPERX>* pai;
	public:
		bind()
		{ }
		bind(const OPERX& regid, const OPERX& a)
			: arg(1 + a.size()), parg(1 + a.size()), pai(xll::XAddIn<XLOPERX>::Find(regid))
		{
			if (pai) {
				ensure (a.size() == pai->Args().Arity());
				arg[0] = pai->RegisterId();
			}
			else {
				arg[0] = regid;
			}
			parg[0] = &arg[0];

			for (xword i = 1; i < arg.size(); ++i) {
				if (is_missing(a[i - 1])) {
					ind.push_back(i);
					arg[i] = OPERX(xltype::Missing);
				}
				else {
					arg[i] = a[i-1];
				}
				parg[i] = &arg[i];
			}

		}
		bind(const bind&) = default;
		bind& operator=(const bind&) = default;
		~bind()
		{ }

		// supply missing args off call stack
		OPERX call(const LPOPERX* ppa) const
		{
			LOPERX ret;

			for (xword i = 0; i < ind.size() && ppa && *ppa && (*ppa)->xltype != xltypeMissing; ++i, ++ppa) {
				parg[ind[i]] = *ppa;
			}

			if (pai) {
				ensure (xlretSuccess == xll::traits<XLOPERX>::Excelv(xlUDF, &ret, parg.size(), &parg[0]));
			}
			else {
				// xlUDF fails for built-ins???
				ensure (xlretSuccess == xll::traits<XLOPERX>::Excelv(static_cast<int>(parg[0]->val.num), &ret, parg.size() - 1, &parg[1]));
			}

			// restore const promise
			for (xword i = 0; i < ind.size(); ++i, ++ppa) {
				parg[ind[i]] = const_cast<LPOPERX>(&arg[ind[i]]);
			}

			return ret;
		}
	};
/*
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
*/
} // xll

