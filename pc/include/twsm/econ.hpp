//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef TWSM_ECON_HPP
#define TWSM_ECON_HPP


#include "ui_main_window.h"

#include <twl/network/network.hpp>

#include <QObject>


namespace Ui
{class main_window;}

namespace twsm
{
	class econ : public QObject
	{
		Q_OBJECT

		Ui::main_window& m_ui;

		std::map<std::size_t, twl::econ_client> m_clients;
		mlk::uint m_current_id{0};

	public:
		econ(Ui::main_window& ui) :
			m_ui{ui}
		{ }

		void update()
		{
			for(auto& a : m_clients)
				a.second.update();
		}

		void init()
		{
			QObject::connect(m_ui.m_cb_ec_servers, SIGNAL(currentIndexChanged(int)), this, SLOT(switch_server(int)));

			QObject::connect(m_ui.m_pb_ec_connect, SIGNAL(clicked()), this, SLOT(connect()));
			QObject::connect(m_ui.m_pb_ec_change_map, SIGNAL(clicked()), this, SLOT(send_map_change()));
			QObject::connect(m_ui.m_pb_ec_reload_map, SIGNAL(clicked()), this, SLOT(send_reload_map()));
			QObject::connect(m_ui.m_pb_ec_start_recording, SIGNAL(clicked()), this, SLOT(send_start_recording()));
			QObject::connect(m_ui.m_pb_ec_stop_recording, SIGNAL(clicked()), this, SLOT(send_stop_recording()));
			QObject::connect(m_ui.m_pb_ec_set_broadcast, SIGNAL(clicked()), this, SLOT(send_broadcast()));
			QObject::connect(m_ui.m_pb_ec_restart_round, SIGNAL(clicked()), this, SLOT(send_restart()));
			QObject::connect(m_ui.m_pb_ec_send_chatmsg, SIGNAL(clicked()), this, SLOT(send_chat()));
			QObject::connect(m_ui.m_pb_ec_send_custom_cmd, SIGNAL(clicked()), this, SLOT(send_custom_cmd()));
			QObject::connect(m_ui.m_pb_ec_shutdown, SIGNAL(clicked()), this, SLOT(send_shutdown()));
		}

		void connect(const mlk::ntw::ip_address& addr, const std::string& passwd)
		{
			auto iter(mlk::cnt::find_in_if([&addr](const decltype(m_clients)::value_type& c){return c.second.address() == addr;}, m_clients));

			// connected, client needs login
			if(iter != std::end(m_clients))
			{
				if(!iter->second.connected())
					iter->second.connect(addr);

				if(!iter->second.logged_in())
					iter->second.login(passwd);
			}

			// not connected, client needs connection and login
			else
			{
				// create new client
				m_clients.emplace(m_current_id, addr);
				auto& cl(m_clients[m_current_id]);
				cl.on_log_added =
				[this, id = m_current_id](const std::string& str)
				{
					if((m_ui.m_cb_ec_servers->currentIndex() == 0) ||
					   (m_ui.m_cb_ec_servers->currentData().toInt() != static_cast<int>(id)))
						return;

					m_ui.m_te_ec_log->moveCursor(QTextCursor::End);
					m_ui.m_te_ec_log->insertPlainText(str.c_str());
					m_ui.m_te_ec_log->moveCursor(QTextCursor::End);
				};
				cl.on_connection = [this, id = m_current_id]{this->_on_connection(id);};
				cl.on_login = [this, id = m_current_id]{this->_on_login(id);};
				cl.on_connection_timeout = []{std::cout << "timeout" << std::endl;};
				cl.login(passwd);
				++m_current_id;

			}
		}

		void send_command(const twl::econ_command& cmd)
		{
			auto index(m_ui.m_cb_ec_servers->currentIndex());
			if(index == 0)
			{
				// apply on all connected servers
				for(auto& a : m_clients)
					a.second.send_command(cmd);
			}
			else if(index > 0)
			{
				// get id (index != id)
				auto id(m_ui.m_cb_ec_servers->itemData(index).toInt());
				m_clients[id].send_command(cmd);
			}
		}

	private:
		void _on_connection(mlk::uint id)
		{
			for(auto i(1); i < m_ui.m_cb_ec_servers->count(); ++i)
				if(m_ui.m_cb_ec_servers->itemData(i).toInt() == static_cast<int>(id))
				{
					// we were already connected, choose right item
					m_ui.m_cb_ec_servers->setCurrentIndex(i);
					return;
				}


			// new connection
			m_ui.m_cb_ec_servers->addItem(m_clients[id].address().to_string().c_str(), id);
			m_ui.m_cb_ec_servers->setCurrentIndex(m_ui.m_cb_ec_servers->count() - 1);
		}

		void _on_login(mlk::uint id)
		{

		}

	private slots:
		void switch_server(int index)
		{
			if(index == 0)
				return;
			auto id(m_ui.m_cb_ec_servers->itemData(index).toInt());

			// switch server, reset ui
			m_ui.m_te_ec_log->setText(m_clients[id].log().c_str());
		}

		void connect()
		{this->connect({QString{m_ui.m_le_ec_host->text() + ":" + m_ui.m_sb_ec_port->text()}.toStdString()}, m_ui.m_le_ec_passwd->text().toStdString());}

		void send_map_change()
		{this->send_command("change_map " + m_ui.m_le_ec_mapname->text().toStdString());}

		void send_reload_map()
		{this->send_command("reload");}

		void send_start_recording()
		{this->send_command("record " + m_ui.m_le_ec_recording_filename->text().toStdString());}

		void send_stop_recording()
		{this->send_command("stoprecord");}

		void send_broadcast()
		{this->send_command("broadcast " + m_ui.m_le_ec_broadcast->text().toStdString());}

		void send_restart()
		{this->send_command("restart " + m_ui.m_le_ec_restart_time->text().toStdString());}

		void send_chat()
		{this->send_command("say " + m_ui.m_le_ec_chatmsg->text().toStdString());}

		void send_custom_cmd()
		{this->send_command(m_ui.m_le_ec_custom_cmd->text().toStdString());}

		void send_shutdown()
		{this->send_command("shutdown");}
	};
}


#endif // TWSM_ECON_HPP

