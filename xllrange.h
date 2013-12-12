// xllrange.h - range functions
#pragma once
#include "../xll8/xll/xll.h"

namespace xll {
	namespace range {

		// template<OPERX o, ...> ???
		template<class O, class... Os>
		inline OPERX gather(const O& o, const Os&... os)
		{
			return sizeof...(os) == 0 ? o : o.push_back(gather(os)); // !!! handle o.xltype == xltypeMulti
		}

		// reposition arguments
		inline OPERX index(const OPERX& o, const OPERX& i)
		{
			typedef traits<XLOPERX>::xword xword;

			OPERX oi(1, i.size());

			for (xword j = 0; j < i.size(); ++j)
				oi[j] = o[static_cast<xword>(i[j])];

			return oi;
		}

		// assemble pointers into a multi
		inline OPERX grab(const LPOPERX* ppo)
		{
			OPERX o;

			//???use XLOPER and just hook up pointers???
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
		inline void pack(OPERX& o)
		{
			for (traits<XLOPERX>::xword i = 0; i < o.size(); ++i) {
				if (o[i].xltype == xltypeMulti) {
					o[i] = handle<OPERX>(new OPERX(o[i])).get();
				}
			}
		}
		// replace handles with multis
		inline void unpack(OPERX& o)
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
	} // range

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

} // xll
