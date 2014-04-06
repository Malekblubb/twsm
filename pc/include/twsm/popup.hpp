//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef TWSM_POPUP_HPP
#define TWSM_POPUP_HPP


#include <mlk/time/simple_timer.h>

#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <iostream>


namespace twsm
{
	class popup
	{
		mlk::tm::simple_timer m_timer;
		bool m_need_destroy{false};

		int m_current_alpha;
		const QColor m_color;
		const QColor m_textcolor{255, 255, 255, 255};

		QWidget m_display_widget;
		QLabel m_text;
		QVBoxLayout m_layout{&m_display_widget};

	public:
		popup(const std::string& text, mlk::ullong lifetime, const QColor& color, QWidget& parent) :
			m_timer{lifetime},
			m_current_alpha{color.alpha()},
			m_color{color},
			m_display_widget{&parent},
			m_text{text.c_str(), &m_display_widget}
		{
			this->set_bgcolor(color);
			this->set_text_color(m_textcolor);

			m_layout.setMargin(10);

			m_text.adjustSize();
			m_layout.addWidget(&m_text);

			m_display_widget.adjustSize();
			m_display_widget.move(parent.geometry().width() / 2 - m_display_widget.geometry().width() / 2, parent.geometry().height() - m_display_widget.geometry().height() * 2);
			m_display_widget.show();

			m_timer.run();
		}

		void update() noexcept
		{
			if(m_timer.timed_out())
			{
				if(m_current_alpha <= 0)
				{
					m_need_destroy = true;
					return;
				}

				if(mlk::tm::time_stmp() % 4 == 0)
				{
					// fade-out effect
					this->set_bgcolor({m_color.red(), m_color.green(), m_color.blue(), m_current_alpha});
					this->set_text_color({m_textcolor.red(), m_textcolor.green(), m_textcolor.blue(), m_current_alpha});
					--m_current_alpha;
				}
			}
		}

		bool need_destroy() const noexcept
		{return m_need_destroy;}

	private:
		void set_bgcolor(const QColor& color)
		{
			m_display_widget.setStyleSheet(
						QString{"border-radius: 3px; background-color: rgb(%1, %2, %3, %4);"}.arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()));
		}

		void set_text_color(const QColor& color)
		{
			m_text.setStyleSheet(
						QString{"background-color: rgb(0, 0, 0, 0); color: rgb(%1, %2, %3, %4);"}.arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()));
		}
	};
}


#endif // TWSM_POPUP_HPP

