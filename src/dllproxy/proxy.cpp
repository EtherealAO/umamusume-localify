#include <stdinclude.hpp>

extern "C"
{
	//version
	void* GetFileVersionInfoA_Original = NULL;
	void* GetFileVersionInfoByHandle_Original = NULL;
	void* GetFileVersionInfoExA_Original = NULL;
	void* GetFileVersionInfoExW_Original = NULL;
	void* GetFileVersionInfoSizeA_Original = NULL;
	void* GetFileVersionInfoSizeExA_Original = NULL;
	void* GetFileVersionInfoSizeExW_Original = NULL;
	void* GetFileVersionInfoSizeW_Original = NULL;
	void* GetFileVersionInfoW_Original = NULL;
	void* VerFindFileA_Original = NULL;
	void* VerFindFileW_Original = NULL;
	void* VerInstallFileA_Original = NULL;
	void* VerInstallFileW_Original = NULL;
	void* VerLanguageNameA_Original = NULL;
	void* VerLanguageNameW_Original = NULL;
	void* VerQueryValueA_Original = NULL;
	void* VerQueryValueW_Original = NULL;

	//psapi
	void* EmptyWorkingSet_Original = NULL;
	void* EnumDeviceDrivers_Original = NULL;
	void* EnumPageFilesA_Original = NULL;
	void* EnumPageFilesW_Original = NULL;
	void* EnumProcessModules_Original = NULL;
	void* EnumProcessModulesEx_Original = NULL;
	void* EnumProcesses_Original = NULL;
	void* GetDeviceDriverBaseNameA_Original = NULL;
	void* GetDeviceDriverBaseNameW_Original = NULL;
	void* GetDeviceDriverFileNameA_Original = NULL;
	void* GetDeviceDriverFileNameW_Original = NULL;
	void* GetMappedFileNameA_Original = NULL;
	void* GetMappedFileNameW_Original = NULL;
	void* GetModuleBaseNameA_Original = NULL;
	void* GetModuleBaseNameW_Original = NULL;
	void* GetModuleFileNameExA_Original = NULL;
	void* GetModuleFileNameExW_Original = NULL;
	void* GetModuleInformation_Original = NULL;
	void* GetPerformanceInfo_Original = NULL;
	void* GetProcessImageFileNameA_Original = NULL;
	void* GetProcessImageFileNameW_Original = NULL;
	void* GetProcessMemoryInfo_Original = NULL;
	void* GetWsChanges_Original = NULL;
	void* GetWsChangesEx_Original = NULL;
	void* InitializeProcessForWsWatch_Original = NULL;
	void* QueryWorkingSet_Original = NULL;
	void* QueryWorkingSetEx_Original = NULL;

	//winhttp
	void* Private1_Original = NULL;
	void* SvchostPushServiceGlobals_Original = NULL;
	void* WinHttpAddRequestHeaders_Original = NULL;
	void* WinHttpAutoProxySvcMain_Original = NULL;
	void* WinHttpCheckPlatform_Original = NULL;
	void* WinHttpCloseHandle_Original = NULL;
	void* WinHttpConnect_Original = NULL;
	void* WinHttpConnectionDeletePolicyEntries_Original = NULL;
	void* WinHttpConnectionDeleteProxyInfo_Original = NULL;
	void* WinHttpConnectionFreeNameList_Original = NULL;
	void* WinHttpConnectionFreeProxyInfo_Original = NULL;
	void* WinHttpConnectionFreeProxyList_Original = NULL;
	void* WinHttpConnectionGetNameList_Original = NULL;
	void* WinHttpConnectionGetProxyInfo_Original = NULL;
	void* WinHttpConnectionGetProxyList_Original = NULL;
	void* WinHttpConnectionSetPolicyEntries_Original = NULL;
	void* WinHttpConnectionSetProxyInfo_Original = NULL;
	void* WinHttpConnectionUpdateIfIndexTable_Original = NULL;
	void* WinHttpCrackUrl_Original = NULL;
	void* WinHttpCreateProxyResolver_Original = NULL;
	void* WinHttpCreateUrl_Original = NULL;
	void* WinHttpDetectAutoProxyConfigUrl_Original = NULL;
	void* WinHttpFreeProxyResult_Original = NULL;
	void* WinHttpFreeProxyResultEx_Original = NULL;
	void* WinHttpFreeProxySettings_Original = NULL;
	void* WinHttpGetDefaultProxyConfiguration_Original = NULL;
	void* WinHttpGetIEProxyConfigForCurrentUser_Original = NULL;
	void* WinHttpGetProxyForUrl_Original = NULL;
	void* WinHttpGetProxyForUrlEx_Original = NULL;
	void* WinHttpGetProxyForUrlEx2_Original = NULL;
	void* WinHttpGetProxyForUrlHvsi_Original = NULL;
	void* WinHttpGetProxyResult_Original = NULL;
	void* WinHttpGetProxyResultEx_Original = NULL;
	void* WinHttpGetProxySettingsVersion_Original = NULL;
	void* WinHttpGetTunnelSocket_Original = NULL;
	void* WinHttpOpen_Original = NULL;
	void* WinHttpOpenRequest_Original = NULL;
	void* WinHttpPacJsWorkerMain_Original = NULL;
	void* WinHttpProbeConnectivity_Original = NULL;
	void* WinHttpQueryAuthSchemes_Original = NULL;
	void* WinHttpQueryDataAvailable_Original = NULL;
	void* WinHttpQueryHeaders_Original = NULL;
	void* WinHttpQueryOption_Original = NULL;
	void* WinHttpReadData_Original = NULL;
	void* WinHttpReadProxySettings_Original = NULL;
	void* WinHttpReadProxySettingsHvsi_Original = NULL;
	void* WinHttpReceiveResponse_Original = NULL;
	void* WinHttpResetAutoProxy_Original = NULL;
	void* WinHttpSaveProxyCredentials_Original = NULL;
	void* WinHttpSendRequest_Original = NULL;
	void* WinHttpSetCredentials_Original = NULL;
	void* WinHttpSetDefaultProxyConfiguration_Original = NULL;
	void* WinHttpSetOption_Original = NULL;
	void* WinHttpSetStatusCallback_Original = NULL;
	void* WinHttpSetTimeouts_Original = NULL;
	void* WinHttpTimeFromSystemTime_Original = NULL;
	void* WinHttpTimeToSystemTime_Original = NULL;
	void* WinHttpWebSocketClose_Original = NULL;
	void* WinHttpWebSocketCompleteUpgrade_Original = NULL;
	void* WinHttpWebSocketQueryCloseStatus_Original = NULL;
	void* WinHttpWebSocketReceive_Original = NULL;
	void* WinHttpWebSocketSend_Original = NULL;
	void* WinHttpWebSocketShutdown_Original = NULL;
	void* WinHttpWriteData_Original = NULL;
	void* WinHttpWriteProxySettings_Original = NULL;
}

using namespace std;

namespace proxy
{
	void init_proxy(const std::string& proxyName) {
		std::string dll_path;
		dll_path.resize(MAX_PATH);
		dll_path.resize(GetSystemDirectoryA(dll_path.data(), MAX_PATH));

		dll_path += "\\" + proxyName;
		auto original_dll = LoadLibraryA(dll_path.data());
		printf("%s", proxyName);

		if (proxyName == "version.dll") {
			GetFileVersionInfoA_Original = GetProcAddress(original_dll, "GetFileVersionInfoA");
			GetFileVersionInfoByHandle_Original = GetProcAddress(original_dll, "GetFileVersionInfoByHandle");
			GetFileVersionInfoExA_Original = GetProcAddress(original_dll, "GetFileVersionInfoExA");
			GetFileVersionInfoExW_Original = GetProcAddress(original_dll, "GetFileVersionInfoExW");
			GetFileVersionInfoSizeA_Original = GetProcAddress(original_dll, "GetFileVersionInfoSizeA");
			GetFileVersionInfoSizeExA_Original = GetProcAddress(original_dll, "GetFileVersionInfoSizeExA");
			GetFileVersionInfoSizeExW_Original = GetProcAddress(original_dll, "GetFileVersionInfoSizeExW");
			GetFileVersionInfoSizeW_Original = GetProcAddress(original_dll, "GetFileVersionInfoSizeW");
			GetFileVersionInfoW_Original = GetProcAddress(original_dll, "GetFileVersionInfoW");
			VerFindFileA_Original = GetProcAddress(original_dll, "VerFindFileA");
			VerFindFileW_Original = GetProcAddress(original_dll, "VerFindFileW");
			VerInstallFileA_Original = GetProcAddress(original_dll, "VerInstallFileA");
			VerInstallFileW_Original = GetProcAddress(original_dll, "VerInstallFileW");
			VerLanguageNameA_Original = GetProcAddress(original_dll, "VerLanguageNameA");
			VerLanguageNameW_Original = GetProcAddress(original_dll, "VerLanguageNameW");
			VerQueryValueA_Original = GetProcAddress(original_dll, "VerQueryValueA");
			VerQueryValueW_Original = GetProcAddress(original_dll, "VerQueryValueW");
		}
		else if (proxyName == "psapi.dll") {
			EmptyWorkingSet_Original = GetProcAddress(original_dll, "EmptyWorkingSet");
			EnumDeviceDrivers_Original = GetProcAddress(original_dll, "EnumDeviceDrivers");
			EnumPageFilesA_Original = GetProcAddress(original_dll, "EnumPageFilesA");
			EnumPageFilesW_Original = GetProcAddress(original_dll, "EnumPageFilesW");
			EnumProcessModules_Original = GetProcAddress(original_dll, "EnumProcessModules");
			EnumProcessModulesEx_Original = GetProcAddress(original_dll, "EnumProcessModulesEx");
			EnumProcesses_Original = GetProcAddress(original_dll, "EnumProcesses");
			GetDeviceDriverBaseNameA_Original = GetProcAddress(original_dll, "GetDeviceDriverBaseNameA");
			GetDeviceDriverBaseNameW_Original = GetProcAddress(original_dll, "GetDeviceDriverBaseNameW");
			GetDeviceDriverFileNameA_Original = GetProcAddress(original_dll, "GetDeviceDriverFileNameA");
			GetDeviceDriverFileNameW_Original = GetProcAddress(original_dll, "GetDeviceDriverFileNameW");
			GetMappedFileNameA_Original = GetProcAddress(original_dll, "GetMappedFileNameA");
			GetMappedFileNameW_Original = GetProcAddress(original_dll, "GetMappedFileNameW");
			GetModuleBaseNameA_Original = GetProcAddress(original_dll, "GetModuleBaseNameA");
			GetModuleBaseNameW_Original = GetProcAddress(original_dll, "GetModuleBaseNameW");
			GetModuleFileNameExA_Original = GetProcAddress(original_dll, "GetModuleFileNameExA");
			GetModuleFileNameExW_Original = GetProcAddress(original_dll, "GetModuleFileNameExW");
			GetModuleInformation_Original = GetProcAddress(original_dll, "GetModuleInformation");
			GetPerformanceInfo_Original = GetProcAddress(original_dll, "GetPerformanceInfo");
			GetProcessImageFileNameA_Original = GetProcAddress(original_dll, "GetProcessImageFileNameA");
			GetProcessImageFileNameW_Original = GetProcAddress(original_dll, "GetProcessImageFileNameW");
			GetProcessMemoryInfo_Original = GetProcAddress(original_dll, "GetProcessMemoryInfo");
			GetWsChanges_Original = GetProcAddress(original_dll, "GetWsChanges");
			GetWsChangesEx_Original = GetProcAddress(original_dll, "GetWsChangesEx");
			InitializeProcessForWsWatch_Original = GetProcAddress(original_dll, "InitializeProcessForWsWatch");
			QueryWorkingSet_Original = GetProcAddress(original_dll, "QueryWorkingSet");
			QueryWorkingSetEx_Original = GetProcAddress(original_dll, "QueryWorkingSetEx");
		}
		else if (proxyName == "winhttp.dll") {
			Private1_Original = GetProcAddress(original_dll, "Private1");
			SvchostPushServiceGlobals_Original = GetProcAddress(original_dll, "SvchostPushServiceGlobals");
			WinHttpAddRequestHeaders_Original = GetProcAddress(original_dll, "WinHttpAddRequestHeaders");
			WinHttpAutoProxySvcMain_Original = GetProcAddress(original_dll, "WinHttpAutoProxySvcMain");
			WinHttpCheckPlatform_Original = GetProcAddress(original_dll, "WinHttpCheckPlatform");
			WinHttpCloseHandle_Original = GetProcAddress(original_dll, "WinHttpCloseHandle");
			WinHttpConnect_Original = GetProcAddress(original_dll, "WinHttpConnect");
			WinHttpConnectionDeletePolicyEntries_Original = GetProcAddress(original_dll, "WinHttpConnectionDeletePolicyEntries");
			WinHttpConnectionDeleteProxyInfo_Original = GetProcAddress(original_dll, "WinHttpConnectionDeleteProxyInfo");
			WinHttpConnectionFreeNameList_Original = GetProcAddress(original_dll, "WinHttpConnectionFreeNameList");
			WinHttpConnectionFreeProxyInfo_Original = GetProcAddress(original_dll, "WinHttpConnectionFreeProxyInfo");
			WinHttpConnectionFreeProxyList_Original = GetProcAddress(original_dll, "WinHttpConnectionFreeProxyList");
			WinHttpConnectionGetNameList_Original = GetProcAddress(original_dll, "WinHttpConnectionGetNameList");
			WinHttpConnectionGetProxyInfo_Original = GetProcAddress(original_dll, "WinHttpConnectionGetProxyInfo");
			WinHttpConnectionGetProxyList_Original = GetProcAddress(original_dll, "WinHttpConnectionGetProxyList");
			WinHttpConnectionSetPolicyEntries_Original = GetProcAddress(original_dll, "WinHttpConnectionSetPolicyEntries");
			WinHttpConnectionSetProxyInfo_Original = GetProcAddress(original_dll, "WinHttpConnectionSetProxyInfo");
			WinHttpConnectionUpdateIfIndexTable_Original = GetProcAddress(original_dll, "WinHttpConnectionUpdateIfIndexTable");
			WinHttpCrackUrl_Original = GetProcAddress(original_dll, "WinHttpCrackUrl");
			WinHttpCreateProxyResolver_Original = GetProcAddress(original_dll, "WinHttpCreateProxyResolver");
			WinHttpCreateUrl_Original = GetProcAddress(original_dll, "WinHttpCreateUrl");
			WinHttpDetectAutoProxyConfigUrl_Original = GetProcAddress(original_dll, "WinHttpDetectAutoProxyConfigUrl");
			WinHttpFreeProxyResult_Original = GetProcAddress(original_dll, "WinHttpFreeProxyResult");
			WinHttpFreeProxyResultEx_Original = GetProcAddress(original_dll, "WinHttpFreeProxyResultEx");
			WinHttpFreeProxySettings_Original = GetProcAddress(original_dll, "WinHttpFreeProxySettings");
			WinHttpGetDefaultProxyConfiguration_Original = GetProcAddress(original_dll, "WinHttpGetDefaultProxyConfiguration");
			WinHttpGetIEProxyConfigForCurrentUser_Original = GetProcAddress(original_dll, "WinHttpGetIEProxyConfigForCurrentUser");
			WinHttpGetProxyForUrl_Original = GetProcAddress(original_dll, "WinHttpGetProxyForUrl");
			WinHttpGetProxyForUrlEx_Original = GetProcAddress(original_dll, "WinHttpGetProxyForUrlEx");
			WinHttpGetProxyForUrlEx2_Original = GetProcAddress(original_dll, "WinHttpGetProxyForUrlEx2");
			WinHttpGetProxyForUrlHvsi_Original = GetProcAddress(original_dll, "WinHttpGetProxyForUrlHvsi");
			WinHttpGetProxyResult_Original = GetProcAddress(original_dll, "WinHttpGetProxyResult");
			WinHttpGetProxyResultEx_Original = GetProcAddress(original_dll, "WinHttpGetProxyResultEx");
			WinHttpGetProxySettingsVersion_Original = GetProcAddress(original_dll, "WinHttpGetProxySettingsVersion");
			WinHttpGetTunnelSocket_Original = GetProcAddress(original_dll, "WinHttpGetTunnelSocket");
			WinHttpOpen_Original = GetProcAddress(original_dll, "WinHttpOpen");
			WinHttpOpenRequest_Original = GetProcAddress(original_dll, "WinHttpOpenRequest");
			WinHttpPacJsWorkerMain_Original = GetProcAddress(original_dll, "WinHttpPacJsWorkerMain");
			WinHttpProbeConnectivity_Original = GetProcAddress(original_dll, "WinHttpProbeConnectivity");
			WinHttpQueryAuthSchemes_Original = GetProcAddress(original_dll, "WinHttpQueryAuthSchemes");
			WinHttpQueryDataAvailable_Original = GetProcAddress(original_dll, "WinHttpQueryDataAvailable");
			WinHttpQueryHeaders_Original = GetProcAddress(original_dll, "WinHttpQueryHeaders");
			WinHttpQueryOption_Original = GetProcAddress(original_dll, "WinHttpQueryOption");
			WinHttpReadData_Original = GetProcAddress(original_dll, "WinHttpReadData");
			WinHttpReadProxySettings_Original = GetProcAddress(original_dll, "WinHttpReadProxySettings");
			WinHttpReadProxySettingsHvsi_Original = GetProcAddress(original_dll, "WinHttpReadProxySettingsHvsi");
			WinHttpReceiveResponse_Original = GetProcAddress(original_dll, "WinHttpReceiveResponse");
			WinHttpResetAutoProxy_Original = GetProcAddress(original_dll, "WinHttpResetAutoProxy");
			WinHttpSaveProxyCredentials_Original = GetProcAddress(original_dll, "WinHttpSaveProxyCredentials");
			WinHttpSendRequest_Original = GetProcAddress(original_dll, "WinHttpSendRequest");
			WinHttpSetCredentials_Original = GetProcAddress(original_dll, "WinHttpSetCredentials");
			WinHttpSetDefaultProxyConfiguration_Original = GetProcAddress(original_dll, "WinHttpSetDefaultProxyConfiguration");
			WinHttpSetOption_Original = GetProcAddress(original_dll, "WinHttpSetOption");
			WinHttpSetStatusCallback_Original = GetProcAddress(original_dll, "WinHttpSetStatusCallback");
			WinHttpSetTimeouts_Original = GetProcAddress(original_dll, "WinHttpSetTimeouts");
			WinHttpTimeFromSystemTime_Original = GetProcAddress(original_dll, "WinHttpTimeFromSystemTime");
			WinHttpTimeToSystemTime_Original = GetProcAddress(original_dll, "WinHttpTimeToSystemTime");
			WinHttpWebSocketClose_Original = GetProcAddress(original_dll, "WinHttpWebSocketClose");
			WinHttpWebSocketCompleteUpgrade_Original = GetProcAddress(original_dll, "WinHttpWebSocketCompleteUpgrade");
			WinHttpWebSocketQueryCloseStatus_Original = GetProcAddress(original_dll, "WinHttpWebSocketQueryCloseStatus");
			WinHttpWebSocketReceive_Original = GetProcAddress(original_dll, "WinHttpWebSocketReceive");
			WinHttpWebSocketSend_Original = GetProcAddress(original_dll, "WinHttpWebSocketSend");
			WinHttpWebSocketShutdown_Original = GetProcAddress(original_dll, "WinHttpWebSocketShutdown");
			WinHttpWriteData_Original = GetProcAddress(original_dll, "WinHttpWriteData");
			WinHttpWriteProxySettings_Original = GetProcAddress(original_dll, "WinHttpWriteProxySettings");
		}
	}
}
