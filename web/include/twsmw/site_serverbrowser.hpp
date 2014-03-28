//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef TWSMW_SITE_SERVERBROWSER_HPP
#define TWSMW_SITE_SERVERBROWSER_HPP


#include "basic_site.hpp"


namespace twsmw
{
	class site_serverbrowser : public basic_site
	{

	public:
		site_serverbrowser(Wt::WStackedWidget& parent) :
			basic_site{"Serverbrowser", parent}
		{ }

		void construct_site_impl() override
		{
		}
	};
}


#endif // TWSMW_SITE_SERVERBROWSER_HPP
