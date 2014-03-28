//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#include <twsmw/main_site.hpp>


int main(int argc, char* argv[])
{
	return Wt::WRun(argc, argv, [](const Wt::WEnvironment& e){return new twsmw::main_site{e};});
}
