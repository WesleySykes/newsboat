#include <configcontainer.h>
#include <configparser.h>
#include <logger.h>
#include <sstream>
#include <iostream>

namespace newsbeuter
{

// TODO: refactor this. having the configuration command literals scattered around the place several times is unacceptable

configcontainer::configcontainer()
{
	// configure default values
	config_data["show-read-feeds"] = "yes";
	config_data["browser"] = "lynx";
	config_data["use-proxy"] = "no";
	config_data["auto-reload"] = "no";
	config_data["reload-time"] = "30";
	config_data["max-items"] = "0";
	config_data["save-path"] = "~/";
	config_data["download-path"] = "~/";
	config_data["max-downloads"] = "1";
	config_data["podcast-auto-enqueue"] = "no";
	config_data["player"] = "";
	config_data["cleanup-on-quit"] = "yes";
	config_data["user-agent"] = "";
	config_data["refresh-on-startup"] = "no";
}

configcontainer::~configcontainer()
{
}

void configcontainer::register_commands(configparser& cfgparser)
{
	// register this as handler for the supported configuration commands
	cfgparser.register_handler("show-read-feeds", this);
	cfgparser.register_handler("browser", this);
	cfgparser.register_handler("max-items", this);
	cfgparser.register_handler("use-proxy", this);
	cfgparser.register_handler("proxy", this);
	cfgparser.register_handler("proxy-auth", this);
	cfgparser.register_handler("auto-reload", this);
	cfgparser.register_handler("reload-time", this);
	cfgparser.register_handler("save-path", this);
	cfgparser.register_handler("download-path", this);
	cfgparser.register_handler("max-downloads", this);
	cfgparser.register_handler("podcast-auto-enqueue", this);
	cfgparser.register_handler("player", this);
	cfgparser.register_handler("cleanup-on-quit", this);
	cfgparser.register_handler("user-agent", this);
	cfgparser.register_handler("refresh-on-startup", this);
}

action_handler_status configcontainer::handle_action(const std::string& action, const std::vector<std::string>& params) {
	// handle the action when a configuration command has been encountered
	// TODO: refactor this
	GetLogger().log(LOG_DEBUG, "configcontainer::handle_action(%s,...) was called",action.c_str());
	if (action == "show-read-feeds" || action == "auto-reload" || action == "podcast-auto-enqueue" || action == "cleanup-on-quit" || action == "refresh-on-startup") {
		if (params.size() < 1) {
			return AHS_TOO_FEW_PARAMS;
		}
		if (!is_bool(params[0])) {
			return AHS_INVALID_PARAMS;
		}
		config_data[action] = params[0];
		// std::cerr << "setting " << action << " to `" << params[0] << "'" << std::endl;
		return AHS_OK; 
	} else if (action == "browser") {
		if (params.size() < 1) {
			return AHS_TOO_FEW_PARAMS;
		}
		config_data[action] = params[0];
		return AHS_OK;	
	} else if (action == "max-items" || action == "reload-time" || action == "max-downloads") {
		if (params.size() < 1) {
			return AHS_TOO_FEW_PARAMS;
		}
		config_data[action] = params[0];
		return AHS_OK;	
	} else if (action == "use-proxy") {
		if (params.size() < 1) {
			return AHS_TOO_FEW_PARAMS;
		}
		if (!is_bool(params[0])) {
			return AHS_INVALID_PARAMS;
		}
		config_data[action] = params[0];
		return AHS_OK;
	} else if (action == "proxy") {
		if (params.size() < 1) {
			return AHS_TOO_FEW_PARAMS;
		}
		config_data[action] = params[0];
		return AHS_OK;
	} else if (action == "proxy-auth") {
		if (params.size() < 1) {
			return AHS_TOO_FEW_PARAMS;
		}
		config_data[action] = params[0];
		return AHS_OK;
	} else if (action == "save-path") {
		if (params.size() < 1) {
			return AHS_TOO_FEW_PARAMS;
		}
		config_data[action] = params[0];
		return AHS_OK;
	} else if (action == "download-path") {
		if (params.size() < 1) {
			return AHS_TOO_FEW_PARAMS;
		}
		config_data[action] = params[0];
		return AHS_OK;
	} else if (action == "player") {
		if (params.size() < 1) {
			return AHS_TOO_FEW_PARAMS;
		}
		config_data[action] = params[0];
		return AHS_OK;
	} else if (action == "user-agent") {
		if (params.size() < 1) {
			return AHS_TOO_FEW_PARAMS;
		}
		config_data[action] = params[0];
		return AHS_OK;
	}
	return AHS_INVALID_COMMAND;	
}

bool configcontainer::is_bool(const std::string& s) {
	char * bool_values[] = { "yes", "no", "true", "false", 0 };
	for (int i=0; bool_values[i] ; ++i) {
		if (s == bool_values[i])
			return true;
	}
	return false;
}

std::string configcontainer::get_configvalue(const std::string& key) {
	return config_data[key];
}

int configcontainer::get_configvalue_as_int(const std::string& key) {
	std::istringstream is(config_data[key]);
	int i;
	is >> i;
	return i;	
}

bool configcontainer::get_configvalue_as_bool(const std::string& key) {
	if (config_data[key] == "true" || config_data[key] == "yes")
		return true;
	return false;	
}

void configcontainer::set_configvalue(const std::string& key, const std::string& value) {
	GetLogger().log(LOG_DEBUG,"configcontainer::set_configvalue(%s,%s) called", key.c_str(), value.c_str());
	config_data[key] = value;
}

}
