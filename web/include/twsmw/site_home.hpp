//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef TWSMW_SITE_HOME_HPP
#define TWSMW_SITE_HOME_HPP


#include "basic_site.hpp"
#include "utils.hpp"

#include <Wt/WLineEdit>
#include <Wt/WPushButton>


namespace twsmw
{
	class site_home : public basic_site
	{

	public:
		site_home(Wt::WStackedWidget& parent) :
			basic_site{"Home", parent}
		{ }

		void construct_site_impl() override
		{
			m_conatiner.setStyleClass("site_home");

			auto* username(new Wt::WLineEdit);
			username->setPlaceholderText("Username");

			auto* password(new Wt::WLineEdit);
			password->setPlaceholderText("Password");
			password->setEchoMode(Wt::WLineEdit::EchoMode::Password);

			auto* login_btn(new Wt::WPushButton{"Login"});
			connect(login_btn->clicked(), []{/* TODO: SOME LOGIN ACTION */});

			m_conatiner.addWidget(username);
			m_conatiner.addWidget(new Wt::WBreak);
			m_conatiner.addWidget(password);
			m_conatiner.addWidget(new Wt::WBreak);
			m_conatiner.addWidget(login_btn);
		}
	};
}


#endif // TWSMW_SITE_HOME_HPP

