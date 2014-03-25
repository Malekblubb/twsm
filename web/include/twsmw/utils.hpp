//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef TWSMW_UTILS_HPP
#define TWSMW_UTILS_HPP


#include <Wt/WEvent>


namespace twsmw
{
	// lambda signal connect shortcut
	template<typename Signal_Type, typename Func_To>
	void connect(Wt::EventSignal<Signal_Type>& signal, Func_To&& to)
	{signal.connect(std::bind(to));}
}


#endif // TWSMW_UTILS_HPP
