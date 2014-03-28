//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef TWSMW_BASIC_SITE_HPP
#define TWSMW_BASIC_SITE_HPP


#include <Wt/WContainerWidget>
#include <Wt/WStackedWidget>
#include <Wt/WText>


namespace twsmw
{
	class basic_site
	{
	protected:
		Wt::WContainerWidget m_conatiner;

	private:
		const std::string m_title;
		Wt::WStackedWidget& m_parent;

	public:
		basic_site(const std::string& title, Wt::WStackedWidget& parent) :
			m_title{title},
			m_parent{parent}
		{ }

		virtual void construct_site()
		{
			// add title
			auto* title(new Wt::WText{m_title});
			title->setStyleClass("site_title");
			m_conatiner.addWidget(title);
			m_conatiner.addWidget(new Wt::WBreak);

			// call overriden construct func
			this->construct_site_impl();

			// add container to stacked widget
			m_parent.addWidget(&m_conatiner);
		}

		virtual void construct_site_impl() = 0;
	};
}


#endif // TWSMW_BASIC_SITE_HPP

