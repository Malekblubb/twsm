//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef TWSMW_MAIN_SITE_HPP
#define TWSMW_MAIN_SITE_HPP


#include "header.hpp"
#include "navigation_bar.hpp"
#include "site_factory.hpp"
#include "site_home.hpp"
#include "site_serverbrowser.hpp"
#include "utils.hpp"

#include <Wt/WApplication>
#include <Wt/WBootstrapTheme>
#include <Wt/WStackedWidget>
#include <Wt/WTimer>


namespace twsmw
{
	class main_site : public Wt::WApplication
	{
		Wt::WStackedWidget m_mainwidget;

		// site factory
		site_factory m_sitefactory{m_mainwidget};

		// components (ctor order is important)
		navigation_bar<main_site> m_navbar{*this};
		header<main_site> m_header{*this};


		// update timer
		Wt::WTimer m_updatetimer;

	public:
		main_site(const Wt::WEnvironment& e) :
			Wt::WApplication{e}
		{this->init();}


		Wt::WStackedWidget& mainwidget() noexcept
		{return m_mainwidget;}


	private:
		void init()
		{
			this->init_style();

			// construct main sites
			m_sitefactory.create_site<site_home>();
			m_sitefactory.create_site<site_serverbrowser>();
			m_mainwidget.setCurrentIndex(0);
			this->root()->addWidget(&m_mainwidget);


			// setup timer
			m_updatetimer.setInterval(10);
//			m_updatetimer.start();
			connect(m_updatetimer.timeout(), this, &main_site::update);
		}

		void init_style()
		{
			// set theme
			this->setTheme(new Wt::WBootstrapTheme);

			// using style.css
			this->useStyleSheet("style.css");
			m_mainwidget.setStyleClass("mainwidget");
		}

		void update()
		{
//			std::cout << "update" << std::endl;
		}
	};
}


#endif // TWSMW_MAIN_SITE_HPP
