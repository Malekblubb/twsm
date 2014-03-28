//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef TWSMW_HEADER_HPP
#define TWSMW_HEADER_HPP


#include <Wt/WAnchor>
#include <Wt/WContainerWidget>
#include <Wt/WImage>
#include <Wt/WText>


namespace twsmw
{
	template<typename Main_Site>
	class header
	{
		Main_Site& m_mainsite;

		Wt::WContainerWidget m_header;
		Wt::WImage m_twsm_logo{"img/twsm.png", "Twsm"};
		Wt::WText m_versiontext{"v1.0.0(web)"};
		Wt::WText m_infotext;

	public:
		header(Main_Site& ms) :
			m_mainsite{ms}
		{this->init();}

	private:
		void init()
		{
			// header container
			m_header.setStyleClass("header");
			m_mainsite.root()->addWidget(&m_header);

			// logo
			m_twsm_logo.setStyleClass("twsm_logo");
			m_header.addWidget(&m_twsm_logo);

			// text next to logo (left)
			m_versiontext.setStyleClass("version_text");
			m_header.addWidget(&m_versiontext);

			// info text (right)
			m_infotext.setText("Programmed by Christoph Malek | <a href='#'>Donate</a> | <a href='#'>PC Version</a> | <a href='#'>Android Version</a> | <a href='https://github.com/Malekblubb/twsm' target='_blank'>Fork on GitHub</a>");
			m_infotext.setStyleClass("info_text");
			m_header.addWidget(&m_infotext);
		}
	};
}


#endif // TWSMW_HEADER_HPP

