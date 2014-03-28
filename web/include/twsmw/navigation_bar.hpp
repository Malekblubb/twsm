//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef TWSMW_NAVIGATION_BAR_HPP
#define TWSMW_NAVIGATION_BAR_HPP


#include "utils.hpp"

#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WText>


namespace twsmw
{
	template<typename Main_Site>
	class navigation_bar
	{
		Main_Site& m_mainsite;

		Wt::WContainerWidget m_navbarcnt{m_mainsite.root()};
		Wt::WContainerWidget m_entrycnt{&m_navbarcnt};

	public:
		navigation_bar(Main_Site& ms) :
			m_mainsite{ms}
		{this->init();}

	private:
		void init()
		{
			m_navbarcnt.setStyleClass("navbar_bg");
			m_entrycnt.setStyleClass("navbar_entrys");

			this->create_navbar_entry<0>("Home");
			this->create_navbar_entry<1>("Serverbrowser");
			this->create_navbar_entry<2>("My Servers");
			this->create_navbar_entry<3>("External Console");
		}

		template<std::size_t index>
		void create_navbar_entry(const std::string& text)
		{
			// create new navbar entry
			auto* entry_cnt(new Wt::WContainerWidget{&m_entrycnt});
			entry_cnt->setStyleClass("navbar_entry");
			connect(entry_cnt->clicked(), [this]{m_mainsite.mainwidget().setCurrentIndex(index);});
			new Wt::WText{text, entry_cnt}; // create text

			// create html break
			new Wt::WBreak{&m_entrycnt};
		}
	};
}


#endif // TWSMW_NAVIGATION_BAR_HPP
