//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef TWSMW_MAIN_SITE_HPP
#define TWSMW_MAIN_SITE_HPP


#include "header.hpp"
#include "navigation_bar.hpp"
#include "serverbrowser.hpp"
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

		// site factory, sites
		site_factory m_sitefactory{m_mainwidget};
		mlk::sptr<site_serverbrowser> m_srvbrowser_site;

		// ui components (ctor order is important)
		navigation_bar<main_site> m_navbar{*this};
		header<main_site> m_header{*this};

		// workers
		serverbrowser<main_site> m_srvbrowser{*this};

		// update timer
		Wt::WTimer m_updatetimer;

	public:
		main_site(const Wt::WEnvironment& e) :
			Wt::WApplication{e}
		{this->init();}


		// getters
		Wt::WStackedWidget& mainwidget() noexcept
		{return m_mainwidget;}

		auto srvbrowser_site() noexcept
		-> decltype(m_srvbrowser_site)&
		{return m_srvbrowser_site;}

		auto srvbrowser() noexcept
		-> decltype(m_srvbrowser)&
		{return m_srvbrowser;}


	private:
		void init()
		{
			this->init_style();

			// construct main sites
			m_sitefactory.create_site<site_home>();
			m_srvbrowser_site = m_sitefactory.create_site<site_serverbrowser>(m_srvbrowser);
			m_mainwidget.setCurrentIndex(0);
			this->root()->addWidget(&m_mainwidget);


			// setup timer
			m_updatetimer.setInterval(0);
			m_updatetimer.start();
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
			m_srvbrowser.update();
		}
	};
}


#endif // TWSMW_MAIN_SITE_HPP
