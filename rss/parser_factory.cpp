#include <rsspp_internal.h>

namespace rsspp {

rss_parser * rss_parser_factory::get_object(feed& f) {
	switch (f.rss_version) {
		case UNKNOWN:
			return NULL;
		case RSS_0_91:
		case RSS_0_92:
			return new rss_09x_parser();
			/*
		case RSS_1_0:
			return new rss_10_parser();
		case RSS_2_0:
			return new rss_20_parser();
		case ATOM_0_3:
		case ATOM_1_0:
			return new atom_parser();
			*/
		default:
			return NULL;
	}
}

}
