// function.h - function: OPER^n -> OPER
// use BIND to curry arguments for CALL
// use PICK to rearrange arguments
#pragma once
//#define EXCEL12
#include <functional>
#include <memory>
#include "xllrange.h"

#define CATEGORY _T("XLL")
#define CATEGORY_ CATEGORY _T(".")

#define XLL_ARGSMAX 8 // for now!!!

namespace xll {

	// register id and curried args
	class bind {
		typedef traits<XLOPERX>::xword xword;
		OPERX regid_;
		std::vector<xword> ind;
		std::vector<OPERX> arg;
		mutable std::vector<LPXLOPER> parg;
		const XAddIn<XLOPERX>* pai;
	public:
		bind()
		{ }
		bind(const OPERX& regid, const LPXLOPERX* ppa)
			: regid_(regid), pai(0)
		{
			xword nargs = XLL_ARGSMAX;
			pai = XAddIn<XLOPERX>::Find(regid);
			if (pai) {
				regid_ = pai->RegisterId();
				nargs = pai->Args().Arity();
			}

			parg.resize(1 + nargs);
			arg.resize(1 + nargs);

			parg[0] = &regid_;

			for (xword i = 1; i <= nargs; ++i, ++ppa) {
				arg[i] = *(*ppa); // peel args off the call stack
				parg[i] = &arg[i];
				if (arg[i].xltype == xltypeMissing) {
					ind.push_back(i);
				}
			}
		}
		bind(const bind&) = default;
		bind& operator=(const bind&) = default;
		~bind()
		{ }

		// supply missing args off call stack
		OPERX call(LPXLOPERX* ppa) const
		{
			LOPERX ret;

			for (xword i = 0; i < ind.size() && (*ppa)->xltype != xltypeMissing; ++i, ++ppa) {
				parg[ind[i]] = *ppa;
			}

			if (pai) {
				ensure (xlretSuccess == xll::traits<XLOPERX>::Excelv(xlUDF, &ret, 1 + ind.size(), &parg[0]));
			}
			else {
				// xlUDF fails for built-ins???
				ensure (xlretSuccess == xll::traits<XLOPERX>::Excel(static_cast<int>(parg[0]->val.num), &ret, ind.size(), 
					parg[1], parg[2], parg[3], parg[4], parg[5], parg[6], parg[7], parg[8]));
			}

			// restore const promise
			for (xword i = 0; i < ind.size(); ++i, ++ppa) {
				parg[ind[i]] = const_cast<LPOPERX>(&arg[ind[i]]);
			}

			return ret;
		}
		/*
		// supply missing args from a
		OPERX call(const OPERX& a) const
		{
			for (xword i = 0; i < static_cast<xword>(ind.size()); ++i) {
				parg[ind[i]] = const_cast<LPOPERX>(&a[i]);
			}

			LOPERX ret;
			ensure (xlretSuccess == xll::traits<XLOPERX>::Excelv(xlUDF, &ret, parg.size(), &parg[0]));

			return ret;
		}
		*/
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

