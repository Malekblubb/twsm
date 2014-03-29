//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef TWSMW_SERVERBROWSER_HPP
#define TWSMW_SERVERBROWSER_HPP


#include <twl/network/network.hpp>


namespace twsmw
{
	template<typename Main_Site>
	class serverbrowser
	{
		Main_Site& m_mainsite;

		twl::master_server m_masters;
		twl::game_server m_servers;

		bool m_refreshing{false};

	public:
		serverbrowser(Main_Site& ms) :
			m_mainsite{ms}
		{this->init();}

		void update()
		{
			m_masters.update();
			m_servers.update();
		}

		void refresh()
		{
			if(m_refreshing)
				return;

			m_refreshing = true;
			m_servers.reset();
			m_masters.request_list();
		}

	private:
		void init()
		{
			m_masters.add_master({"master1.teeworlds.com:8300"});
			m_masters.add_master({"master2.teeworlds.com:8300"});
			m_masters.add_master({"master3.teeworlds.com:8300"});
			m_masters.add_master({"master4.teeworlds.com:8300"});

			m_masters.on_finish = [this]{this->masters_finished();};
			m_servers.on_finish = [this]{this->servers_finished();};
		}

		void masters_finished()
		{
			m_servers.add_masterlist(m_masters.get_list());
			m_servers.request_info();
		}

		void servers_finished()
		{
			m_refreshing = false;
			std::cout << "finished " << m_servers.get_infos().size() << " srvers" << std::endl;
			m_mainsite.srvbrowser_site()->add_infos(m_servers.get_infos());
		}
	};
}


#endif // TWSMW_SERVERBROWSER_HPP

