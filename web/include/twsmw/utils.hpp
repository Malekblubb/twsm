//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef TWSMW_UTILS_HPP
#define TWSMW_UTILS_HPP


#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WEvent>
#include <Wt/WObject>

#include <functional>


namespace twsmw
{
	// lambda signal connect shortcut
	template<typename Signal_Type, typename Func_To>
	void connect(Wt::EventSignal<Signal_Type>& signal, Func_To&& to)
	{signal.connect(std::bind(to));}

	// memfn
	template<typename Signal_Type, typename Some_Type, typename Memfn_Ret, typename Memfn_Class>
	void connect(Wt::EventSignal<Signal_Type>& signal, Some_Type* t, Memfn_Ret(Memfn_Class::*ptr)())
	{signal.connect(t, ptr);}


	// create n braks
	template<std::size_t current, std::size_t max>
	typename std::enable_if<current == max, void>::type create_breaks_impl(Wt::WContainerWidget&)
	{ }

	template<std::size_t current, std::size_t max>
	typename std::enable_if<current < max, void>::type create_breaks_impl(Wt::WContainerWidget& widget)
	{
		widget.addWidget(new Wt::WBreak);
		create_breaks_impl<current + 1, max>(widget);
	}

	template<std::size_t num>
	void create_breaks(Wt::WContainerWidget& widget)
	{create_breaks_impl<0, num>(widget);}
}


#endif // TWSMW_UTILS_HPP
