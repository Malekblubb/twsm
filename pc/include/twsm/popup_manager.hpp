//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef TWSM_POPUP_MANAGER_HPP
#define TWSM_POPUP_MANAGER_HPP


#include "popup.hpp"

#include <mlk/containers/container_utl.h>


namespace twsm
{
	enum class popup_type : char
	{info, error, warning};

	class popup_manager
	{
		std::vector<mlk::sptr<popup>> m_popups;

		QWidget& m_parent;

	public:
		popup_manager(QWidget& parent) :
			m_parent{parent}
		{ }

		void update()
		{
			for(auto& a : m_popups)
				a->update();

			// delete dead popups
			mlk::cnt::remove_all_if([](const mlk::sptr<popup>& p){return p->need_destroy();}, m_popups);
		}

		template<popup_type type>
		mlk::sptr<popup>& create_popup(const std::string& text, mlk::ullong lifetime = 3000)
		{
			m_popups.emplace_back(std::make_shared<popup>(text, lifetime, QColor{80, 174, 250}, m_parent));
			return m_popups.back();
		}
	};

	template<>
	inline mlk::sptr<popup>& popup_manager::create_popup<popup_type::error>(const std::string& text, mlk::ullong lifetime)
	{
		m_popups.emplace_back(std::make_shared<popup>(text, lifetime, QColor{250, 36, 36}, m_parent));
		return m_popups.back();
	}

	template<>
	inline mlk::sptr<popup>& popup_manager::create_popup<popup_type::warning>(const std::string& text, mlk::ullong lifetime)
	{
		m_popups.emplace_back(std::make_shared<popup>(text, lifetime, QColor{235, 184, 29}, m_parent));
		return m_popups.back();
	}
}


#endif // TWSM_POPUP_MANAGER_HPP

