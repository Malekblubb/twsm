//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef TWSMW_UTILS_HPP
#define TWSMW_UTILS_HPP


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
}


#endif // TWSMW_UTILS_HPP
