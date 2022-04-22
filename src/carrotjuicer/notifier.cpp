/*
	Source code from https://github.com/CNA-Bld/EXNOA-CarrotJuicer by CNA-Bld
	Released under MIT license. Copyright belongs to CNA-Bld(https://github.com/CNA-Bld)
*/
#include <string>
#include "httplib.h"

namespace notifier
{
	httplib::Client* client = nullptr;

	void init()
	{
		client = new httplib::Client("http://127.0.0.1:4693");
		client->set_connection_timeout(0, 50000);
	}

	void notify_response(const std::string& data)
	{
		if (client == nullptr) {
			init();
		}

		auto res = client->Post("/notify/response", data, "application/x-msgpack");
		httplib::Error error = res.error();
		if (error != httplib::Error::Success)
		{
			printf("Unexpected error from notifier: %s \n", httplib::to_string(error));
		}
	}
	void notify_request(const std::string& data)
	{
		if (client == nullptr) {
			init();
		}

		auto res = client->Post("/notify/request", data, "application/x-msgpack");
		httplib::Error error = res.error();
		if (error != httplib::Error::Success)
		{
			printf("Unexpected error from notifier: %s \n", httplib::to_string(error));
		}
	}
}
