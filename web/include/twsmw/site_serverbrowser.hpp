//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef TWSMW_SITE_SERVERBROWSER_HPP
#define TWSMW_SITE_SERVERBROWSER_HPP


#include "basic_site.hpp"
#include "serverbrowser.hpp"
#include "utils.hpp"

#include <twl/network/server_info.hpp>

#include <Wt/WPushButton>
#include <Wt/WStandardItem>
#include <Wt/WStandardItemModel>
#include <Wt/WTableView>


namespace twsmw
{
	class main_site;

	class site_serverbrowser : public basic_site
	{
		serverbrowser<main_site>& m_srvbrowser;

		Wt::WPushButton m_refreshbutton{"Refresh"};
		Wt::WTableView m_table;
		Wt::WStandardItemModel m_model;

	public:
		site_serverbrowser(Wt::WStackedWidget& parent, serverbrowser<main_site>& srvb) :
			basic_site{"Serverbrowser", parent},
			m_srvbrowser{srvb}
		{ }

		void construct_site_impl() override
		{
			m_conatiner.setStyleClass("site_serverbrowser");

			// setup buttons
			connect(m_refreshbutton.clicked(), [this]{m_srvbrowser.refresh();});
			m_conatiner.addWidget(&m_refreshbutton);

			// setup table
			create_breaks<2>(m_conatiner);
			this->clear_table();

			auto* cnt(new Wt::WContainerWidget{&m_conatiner});
			cnt->setStyleClass("table_container");
			m_table.setMaximumSize(1920, 500);
			m_table.setSelectionMode(Wt::SelectionMode::SingleSelection);
			m_table.setSelectionBehavior(Wt::SelectionBehavior::SelectRows);
			m_table.setModel(&m_model);
			cnt->addWidget(&m_table);
		}

		void clear_table()
		{
			m_model.clear();
			m_model.insertColumns(0, 5);
			m_model.setHeaderData(0, "Name");
			m_model.setHeaderData(1, "Type");
			m_model.setHeaderData(2, "Map");
			m_model.setHeaderData(3, "Players");
			m_model.setHeaderData(4, "Ping");
		}

		void add_infos(const twl::server_infos& infos)
		{
			for(auto& a : infos)
			{
				auto row(m_model.rowCount());
				m_model.setItem(row, 0, new Wt::WStandardItem{a.name()});
				m_model.setItem(row, 1, new Wt::WStandardItem{a.gametype()});
				m_model.setItem(row, 2, new Wt::WStandardItem{a.mapname()});
				m_model.setItem(row, 3, new Wt::WStandardItem{Wt::WString{"{1}/{2}"}.arg(a.numclients()).arg(a.maxclients())});
				m_model.setItem(row, 4, new Wt::WStandardItem{Wt::WString{"{1}"}.arg(a.ping())});
			}
		}

		Wt::WTableView& table() noexcept
		{return m_table;}

		Wt::WStandardItemModel& model() noexcept
		{return m_model;}
	};
}


#endif // TWSMW_SITE_SERVERBROWSER_HPP
