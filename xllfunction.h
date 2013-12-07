// function.h - function: OPER^n -> OPER
/*
Assemble OPERs? RANGE(o0, o1, ...)
*/
#pragma once
//#include <functional>
#include "../xll8/xll/xll.h"

namespace xll {

	namespace range {

		// assemble pointers into a multi
		inline OPERX grab(const LPOPERX* ppo)
		{
			OPERX o;

			while ((*ppo)->xltype != xltypeMissing) {
				if ((*ppo)->xltype == xltypeMulti) {
					o.push_back(OPERX(xltype::Missing));// push_back(new handle<OPER>(*(*pro))); ???
					o.back() = (*(*ppo));
				}
				else {
					o.push_back(*(*ppo));
				}
				++ppo;
			}

			return o;
		}
		// replace heirarchical multi with handles
		void pack(OPERX& o)
		{
			for (traits<XLOPERX>::xword i = 0; i < o.size(); ++i) {
				if (o[i].xltype == xltypeMulti) {
					o[i] = handle<OPERX>(new OPERX(o[i])).get();
				}
			}
		}
		// replace handles with multis
		void unpack(OPERX& o)
		{
			for (traits<XLOPERX>::xword i = 0; i < o.size(); ++i) {
				if (o[i].xltype == xltypeNum) {
					try {
						handle<OPERX> oi(o[i].val.num);
						o[i] = *oi;
					}
					catch (...) { }; // not a handle
				}
			}
		}
	}

	// JSON like object.
	// First row are keys, second row are objects
	class object {
		typedef traits<XLOPERX>::xword xword;

		xword n_;
		OPERX* pk_;
		OPERX* pv_;
	public:
		object()
			: pk_(0), pv_(0), n_(0)
		{ }
		object(const object&) = default;
		object& operator=(const object&) = default;
		~object()
		{ }

		object(OPERX& o)
			: pk_(&o(0,0)), pv_(&o(1,0)), n_(o.columns())
		{
			ensure (o.xltype == xltypeMulti);
			ensure (o.rows() == 2);
		}
		object(OPERX& k, OPER& v)
			: pk_(&k[0]), pv_(&v[0]), n_(k.size())
		{
			ensure (k.size() == v.size());
		}

		xword size(void) const
		{
			return n_;
		}
		xword find(const OPERX& key) const
		{
			xword i;

			// not efficent!!!
			for (i = 0; i < n_; ++i) {
				if ((*pk_)[i] == key)
					break;
			}

			return i;
		}
		bool has(const OPERX& key) const
		{
			return find(key) < size();
		}

		OPERX& operator[](const OPERX& key)
		{
			return (*pv_)[find(key)];
		}
		const OPERX& operator[](const OPERX& key) const
		{
			return (*pv_)[find(key)];
		}
	};

	struct function {

		// call from call stack
		OPERX operator()(const LPOPERX *ppa) const { return call(ppa); };
		// call using multi
		OPERX operator()(const OPERX& a) const { return call(a); }
		virtual ~function() { }
	private:
		virtual OPERX call(const LPOPERX*) const = 0;
		virtual OPERX call(const OPERX&) const = 0;
	};
	
	class lambda : public function {
		const std::function<OPERX(const OPERX&)>& f_;
	public:
		lambda(const std::function<OPERX(const OPERX&)>& f)
			: f_(f)
		{ }
		~lambda()
		{ }
		OPERX call(const LPOPERX* ppo)
		{
			return OPERX();
		}
		OPERX call(const OPERX& o)
		{
			return f_(o);
		}
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


		OPERX call(const LPOPERX* ppa) const 
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
		OPERX call(const OPERX& o) const 
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