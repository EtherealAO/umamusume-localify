/*
	Source code from https://github.com/CNA-Bld/EXNOA-CarrotJuicer by CNA-Bld
	Released under MIT license. Copyright belongs to CNA-Bld(https://github.com/CNA-Bld)
*/
#pragma once
#include <string>

namespace notifier
{
	void init();

	void ping();

	void notify_response(const std::string& data);

	void notify_request(const std::string& data);
}
