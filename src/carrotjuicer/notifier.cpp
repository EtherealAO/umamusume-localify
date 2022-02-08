/*
	Source code from https://github.com/CNA-Bld/EXNOA-CarrotJuicer by CNA-Bld
	Released under MIT license. Copyright belongs to CNA-Bld(https://github.com/CNA-Bld)
*/
#include <string>
#include <httplib.h>

namespace notifier
{
	httplib::Client* client = nullptr;

	void init()
	{
		client = new httplib::Client("http://127.0.0.1:4693");
		client->set_connection_timeout(0, 100 * 1000);
	}

	void notify_response(const std::string& data)
	{
		if (client == nullptr) {
			init();
		}

		if (auto res = client->Post("/notify/response", data, "application/x-msgpack"))
		{
			if (res->status != 200)
			{
				std::cout << "Unexpected response from listener: " << res->status << "\n";
			}
		}
	}
	void notify_request(const std::string& data)
	{
		if (client == nullptr) {
			init();
		}

		if (auto res = client->Post("/notify/request", data, "application/x-msgpack"))
		{
			if (res->status != 200)
			{
				std::cout << "Unexpected response from listener: " << res->status << "\n";
			}
		}
	}
}
