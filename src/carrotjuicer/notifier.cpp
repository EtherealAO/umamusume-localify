/*
	Source code from https://github.com/CNA-Bld/EXNOA-CarrotJuicer by CNA-Bld
	Released under MIT license. Copyright belongs to CNA-Bld(https://github.com/CNA-Bld)
*/
#include <string>
#include "httplib.h"
#include "stdinclude.hpp"

namespace notifier
{
	httplib::Client* client = nullptr;

	void init()
	{
		client = new httplib::Client(g_notifier_host);
		client->set_connection_timeout(0, 50000);
	}

	void ping() {
		if (client == nullptr) {
			init();
		}

		auto res = client->Get("/notify/ping");
		auto error = res.error();
		int mb_select = 0;
		if (error != httplib::Error::Success) {
			std::ostringstream oss;
			oss << "无法连接到URA: " << httplib::to_string(error);
			mb_select = MessageBox(NULL, oss.str().c_str(), "umamusume-localify", MB_ABORTRETRYIGNORE | MB_ICONERROR);
		}
		else if (res.value().body != "pong") {
			std::ostringstream oss;
			oss << "URA返回值异常: " << res.value().body;
			mb_select = MessageBox(NULL, oss.str().c_str(), "umamusume-localify", MB_ABORTRETRYIGNORE | MB_ICONERROR);
		}
		switch (mb_select)
		{
		case IDABORT:
			exit(0);
			break;
		case IDRETRY:
			ping();
			break;
		default:
			break;
		}
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
			std::cout << "Unexpected error from notifier: " << httplib::to_string(error) << "\n";
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
			std::cout << "Unexpected error from notifier: " << httplib::to_string(error) << "\n";
		}
	}
}
