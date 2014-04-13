//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef TWSM_SERVERS_FILE_HPP
#define TWSM_SERVERS_FILE_HPP


#include <mlk/filesystem/filesystem.h>
#include <mlk/network/network.h>


namespace twsm
{
	class servers_file
	{
		mlk::fs::file_handle m_file;
		std::vector<std::string> m_read_ips;
		std::vector<std::string> m_write_ips;

	public:
		servers_file(const std::string& path) :
			m_file{path}
		{this->init();}

		~servers_file()
		{
			m_file.reopen(std::ios::out | std::ios::trunc);
			for(const auto& a : m_write_ips)
				m_file.write_line(a);
		}


		auto server_ips()
		-> const decltype(m_read_ips)&
		{return m_read_ips;}

		void add_ip(const std::string& ip)
		{m_write_ips.emplace_back(ip);}

	private:
		void init()
		{
			if(!m_file.exists())
			{
				m_file.create();
				return;
			}

			this->read_file();
		}

		void read_file()
		{
			std::string line;
			while(m_file.read_line(line))
			{
				if(line.empty())
					continue;
				m_read_ips.emplace_back(line);
			}
		}
	};
}


#endif // TWSM_SERVERS_FILE_HPP
