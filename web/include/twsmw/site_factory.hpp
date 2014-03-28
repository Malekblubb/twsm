//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef TWSMW_SITE_FACTORY_HPP
#define TWSMW_SITE_FACTORY_HPP


#include "basic_site.hpp"

#include <mlk/types/types.h>

#include <vector>


namespace twsmw
{
	class site_factory
	{
		Wt::WStackedWidget& m_parent;

		std::vector<mlk::sptr<basic_site>> m_sites;

	public:
		site_factory(Wt::WStackedWidget& parent) :
			m_parent{parent}
		{ }

		template<typename Site_Type>
		void create_site()
		{
			auto ptr(std::make_shared<Site_Type>(m_parent));
			ptr->construct_site();
			m_sites.emplace_back(ptr);
		}
	};
}


#endif // TWSMW_SITE_FACTORY_HPP

