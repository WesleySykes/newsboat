#include <remote_api.h>
#include <ttrss_api.h>
#include <json.h>

namespace newsbeuter {

ttrss_api::ttrss_api(configcontainer * c) : remote_api(c) {
}

ttrss_api::~ttrss_api() {
}

bool ttrss_api::authenticate() {
	sid = retrieve_sid();

	return sid != "";
}

std::string ttrss_api::retrieve_sid() {
	CURL * handle = curl_easy_init();
	const std::string location = cfg->get_configvalue("ttrss-url");
	char * user = curl_easy_escape(handle, cfg->get_configvalue("ttrss-login").c_str(), 0);
	char * pass = curl_easy_escape(handle, cfg->get_configvalue("ttrss-password").c_str(), 0);

	std::string login_url = utils::strprintf("%s/api/?op=login&user=%s&password=%s", location.c_str(), user, pass);

	curl_free(user);
	curl_free(pass);

	std::string result = utils::retrieve_url(login_url, cfg);

	std::string sid;

	struct json_object * reply = json_tokener_parse(result.c_str());

	struct json_object * status = json_object_object_get(reply, "status");
	if (json_object_get_int(status) == 0) {
		struct json_object * content = json_object_object_get(reply, "content");
		struct json_object * session_id = json_object_object_get(content, "session_id");
		sid = json_object_get_string(session_id);
	}

	json_object_put(reply);

	LOG(LOG_DEBUG, "ttrss_api::retrieve_sid: result = '%s' sid = '%s'", result.c_str(), sid.c_str());

	return sid;
}

std::vector<tagged_feedurl> ttrss_api::get_subscribed_urls() {
	std::string cat_url = utils::strprintf("%s/api/?op=getCategories&sid=%s", cfg->get_configvalue("ttrss-url").c_str(), sid.c_str());
	std::string result = utils::retrieve_url(cat_url, cfg);

	LOG(LOG_DEBUG, "ttrss_api::get_subscribed_urls: reply = %s", result.c_str());

	std::vector<tagged_feedurl> feeds;

	struct json_object * reply = json_tokener_parse(result.c_str());

	struct json_object * status = json_object_object_get(reply, "status");
	if (json_object_get_int(status) != 0)
		return feeds;

	struct json_object * content = json_object_object_get(reply, "content");

	if (json_object_get_type(content) != json_type_array)
		return feeds;

	struct array_list * categories = json_object_get_array(content);

	int catsize = array_list_length(categories);

	for (int i=0;i<catsize;i++) {
		struct json_object * cat = (struct json_object *)array_list_get_idx(categories, i);

		struct json_object * cat_id_obj = json_object_object_get(cat, "id");
		int cat_id = json_object_get_int(cat_id_obj);

		struct json_object * cat_title_obj = json_object_object_get(cat, "title");
		const char * cat_name = json_object_get_string(cat_title_obj);

		LOG(LOG_DEBUG, "ttrss_api::get_subscribed_urls: id = %d title = %s", cat_id, cat_name);

		std::string feeds_url = utils::strprintf("%s/api/?op=getFeeds&cat_id=%d&sid=%s", cfg->get_configvalue("ttrss-url").c_str(), cat_id, sid.c_str());

		std::string feeds_data = utils::retrieve_url(feeds_url, cfg);

		LOG(LOG_DEBUG, "ttrss_api::get_subscribed_urls: feeds_data = %s", feeds_data.c_str());

		struct json_object * feeds_reply = json_tokener_parse(feeds_data.c_str());

		struct json_object * feed_list_obj = json_object_object_get(feeds_reply, "content");
		struct array_list * feed_list = json_object_get_array(feed_list_obj);

		int feed_list_size = array_list_length(feed_list);

		for (int j=0;j<feed_list_size;j++) {
			struct json_object * feed = (struct json_object *)array_list_get_idx(feed_list, j);

			int feed_id = json_object_get_int(json_object_object_get(feed, "id"));
			const char * feed_title = json_object_get_string(json_object_object_get(feed, "title"));

			std::vector<std::string> tags;
			tags.push_back(std::string("~") + feed_title);
			tags.push_back(cat_name);
			feeds.push_back(tagged_feedurl(utils::strprintf("ttrss:%d", feed_id), tags));

			// TODO: cache feed_id -> feed_url (or feed_url -> feed_id ?)
		}

		json_object_put(feeds_reply);

	}

	json_object_put(reply);

	return feeds;
}

void ttrss_api::configure_handle(CURL * handle) {
	// TODO: implement
}

bool ttrss_api::mark_all_read(const std::string& feedurl) {

}

bool ttrss_api::mark_article_read(const std::string& guid, bool read) {

}

bool ttrss_api::update_article_flags(const std::string& oldflags, const std::string& newflags, const std::string& guid) {

}


}