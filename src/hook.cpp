#include <stdinclude.hpp>
#include <./carrotjuicer/notifier.hpp>

using namespace std;

namespace
{
	void path_game_assembly();

	bool mh_inited = false;

	void dump_bytes(void* pos)
	{
		printf("Hex dump of %p\n", pos);

		char* memory = reinterpret_cast<char*>(pos);

		for (int i = 0; i < 0x20; i++)
		{
			if (i > 0 && i % 16 == 0)
				printf("\n");

			char byte = *(memory++);

			printf("%02hhX ", byte);
		}

		printf("\n\n");
	}
	// CarrotJuice
	/*
	Source code from https://github.com/CNA-Bld/EXNOA-CarrotJuicer by CNA-Bld
	Released under MIT license. Copyright belongs to CNA-Bld(https://github.com/CNA-Bld)
	*/
	std::string current_time()
	{
		const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch());
		return std::to_string(ms.count());
	}

	void write_file(const std::string& file_name, const char* buffer, const int len)
	{
		FILE* fp;
		fopen_s(&fp, file_name.c_str(), "wb");
		if (fp != nullptr)
		{
			fwrite(buffer, 1, len, fp);
			fclose(fp);
		}
	}


	void* LZ4_decompress_safe_ext_orig = nullptr;

	int LZ4_decompress_safe_ext_hook(
		char* src,
		char* dst,
		int compressedSize,
		int dstCapacity)
	{
		const int ret = reinterpret_cast<decltype(LZ4_decompress_safe_ext_hook)*>(LZ4_decompress_safe_ext_orig)(
			src, dst, compressedSize, dstCapacity);

		const std::string data(dst, ret);

		auto notifier_thread = std::thread([&]
			{
				notifier::notify_response(data);
			});

		notifier_thread.join();

		return ret;
	}

	void* LZ4_compress_default_ext_orig = nullptr;

	int LZ4_compress_default_ext_hook(
		char* src,
		char* dst,
		int srcSize,
		int dstCapacity)
	{
		const int ret = reinterpret_cast<decltype(LZ4_compress_default_ext_hook)*>(LZ4_compress_default_ext_orig)(
			src, dst, srcSize, dstCapacity);

		const std::string data(src, srcSize);

		auto notifier_thread = std::thread([&]
			{
				notifier::notify_request(data);
			});

		notifier_thread.join();

		return ret;
	}
	void bootstrap_carrot_juicer()
	{
		const auto libnative_module = GetModuleHandle("libnative.dll");
		printf("libnative.dll at %p\n", libnative_module);
		if (libnative_module == nullptr)
		{
			return;
		}

		const auto LZ4_decompress_safe_ext_ptr = GetProcAddress(libnative_module, "LZ4_decompress_safe_ext");
		printf("LZ4_decompress_safe_ext at %p\n", LZ4_decompress_safe_ext_ptr);
		if (LZ4_decompress_safe_ext_ptr == nullptr)
		{
			return;
		}
		MH_CreateHook(LZ4_decompress_safe_ext_ptr, LZ4_decompress_safe_ext_hook, &LZ4_decompress_safe_ext_orig);
		MH_EnableHook(LZ4_decompress_safe_ext_ptr);

		const auto LZ4_compress_default_ext_ptr = GetProcAddress(libnative_module, "LZ4_compress_default_ext");
		printf("LZ4_compress_default_ext at %p\n", LZ4_compress_default_ext_ptr);
		if (LZ4_compress_default_ext_ptr == nullptr)
		{
			return;
		}
		MH_CreateHook(LZ4_compress_default_ext_ptr, LZ4_compress_default_ext_hook, &LZ4_compress_default_ext_orig);
		MH_EnableHook(LZ4_compress_default_ext_ptr);
	}
	//Carrot Juice End
	void* load_library_w_orig = nullptr;
	HMODULE __stdcall load_library_w_hook(const wchar_t* path)
	{
		// GameAssembly.dll code must be loaded and decrypted while loading criware library
		if (path == L"cri_ware_unity.dll"s)
		{
			path_game_assembly();
			bootstrap_carrot_juicer();

			MH_DisableHook(LoadLibraryW);
			MH_RemoveHook(LoadLibraryW);

			// use original function beacuse we have unhooked that
			return LoadLibraryW(path);
		}

		return reinterpret_cast<decltype(LoadLibraryW)*>(load_library_w_orig)(path);
	}

	void* populate_with_errors_orig = nullptr;
	bool populate_with_errors_hook(void* _this, Il2CppString* str, TextGenerationSettings_t* settings, void* context)
	{
		return reinterpret_cast<decltype(populate_with_errors_hook)*>(populate_with_errors_orig) (
			_this, local::get_localized_string(str), settings, context
			);
	}

	void* localize_get_orig = nullptr;
	Il2CppString* localize_get_hook(int id)
	{
		auto orig_result = reinterpret_cast<decltype(localize_get_hook)*>(localize_get_orig)(id);
		auto result = local::get_localized_string(id);

		return result ? result : orig_result;
	}

	std::unordered_map<void*, bool> text_queries;

	void* query_ctor_orig = nullptr;
	void* query_ctor_hook(void* _this, void* conn, Il2CppString* sql)
	{
		auto ssql = std::wstring(sql->start_char);

		if (ssql.find(L"text_data") != std::string::npos ||
			ssql.find(L"character_system_text") != std::string::npos ||
			ssql.find(L"race_jikkyo_comment") != std::string::npos ||
			ssql.find(L"race_jikkyo_message") != std::string::npos)
		{
			text_queries.emplace(_this, true);
		}

		return reinterpret_cast<decltype(query_ctor_hook)*>(query_ctor_orig)(_this, conn, sql);
	}

	void* query_dispose_orig = nullptr;
	void query_dispose_hook(void* _this)
	{
		if (text_queries.contains(_this))
			text_queries.erase(_this);

		return reinterpret_cast<decltype(query_dispose_hook)*>(query_dispose_orig)(_this);
	}

	void* query_getstr_orig = nullptr;
	Il2CppString* query_getstr_hook(void* _this, int idx)
	{
		auto result = reinterpret_cast<decltype(query_getstr_hook)*>(query_getstr_orig)(_this, idx);

		if (text_queries.contains(_this))
			return local::get_localized_string(result);

		return result;
	}

	void* set_fps_orig = nullptr;
	void set_fps_hook(int value)
	{
		return reinterpret_cast<decltype(set_fps_hook)*>(set_fps_orig)(g_max_fps);
	}

	bool (*is_virt)() = nullptr;
	int last_height = 0, last_width = 0;

	void* wndproc_orig = nullptr;
	LRESULT wndproc_hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (uMsg == WM_SIZING)
		{
			RECT* rect = reinterpret_cast<RECT*>(lParam);

			float ratio = is_virt() ? 1.f / g_aspect_ratio : g_aspect_ratio;
			float height = rect->bottom - rect->top;
			float width = rect->right - rect->left;

			float new_ratio = width / height;

			if (new_ratio > ratio && height >= last_height || width < last_width)
				height = width / ratio;
			else if (new_ratio < ratio && width >= last_width || height < last_height)
				width = height * ratio;

			switch (wParam)
			{
			case WMSZ_TOP:
			case WMSZ_TOPLEFT:
			case WMSZ_TOPRIGHT:
				rect->top = rect->bottom - height;
				break;
			default:
				rect->bottom = rect->top + height;
				break;
			}

			switch (wParam)
			{
			case WMSZ_LEFT:
			case WMSZ_TOPLEFT:
			case WMSZ_BOTTOMLEFT:
				rect->left = rect->right - width;
				break;
			default:
				rect->right = rect->left + width;
				break;
			}

			last_height = height;
			last_width = width;

			return TRUE;
		}

		return reinterpret_cast<decltype(wndproc_hook)*>(wndproc_orig)(hWnd, uMsg, wParam, lParam);
	}

	void* get_virt_size_orig = nullptr;
	Vector3_t* get_virt_size_hook(Vector3_t* pVec3, int width, int height)
	{
		auto size = reinterpret_cast<decltype(get_virt_size_hook)*>(get_virt_size_orig)(pVec3, width, height);

		height = width * g_aspect_ratio;

		size->x = width;
		size->y = height;
		size->z = g_aspect_ratio;

		return size;
	}

	void* get_hori_size_orig = nullptr;
	Vector3_t* get_hori_size_hook(Vector3_t* pVec3, int width, int height)
	{
		auto size = reinterpret_cast<decltype(get_hori_size_hook)*>(get_hori_size_orig)(pVec3, width, height);

		width = height * g_aspect_ratio;

		size->x = width;
		size->y = height;
		size->z = g_aspect_ratio;

		return size;
	}

	Resolution_t* (*get_resolution)(Resolution_t* buffer);

	void get_resolution_stub(Resolution_t* r)
	{
		*r = *get_resolution(r);

		int width = min(r->height, r->width) * g_aspect_ratio;
		if (r->width > r->height)
			r->width = width;
		else
			r->height = width;
	}

	void* gallop_get_screenheight_orig;
	int gallop_get_screenheight_hook()
	{
		Resolution_t res;
		get_resolution_stub(&res);

		int w = max(res.width, res.height), h = min(res.width, res.height);

		return is_virt() ? w : h;
	}

	void* gallop_get_screenwidth_orig;
	int gallop_get_screenwidth_hook()
	{
		Resolution_t res;
		get_resolution_stub(&res);

		int w = max(res.width, res.height), h = min(res.width, res.height);

		return is_virt() ? h : w;
	}

	void (*set_scale_factor)(void*, float);

	void* canvas_scaler_setres_orig;
	void canvas_scaler_setres_hook(void* _this, Vector2_t res)
	{
		Resolution_t r;
		get_resolution_stub(&r);

		// set scale factor to make ui bigger on hi-res screen
		set_scale_factor(_this, max(1.0f, r.width / 1920.f) * g_ui_scale);

		return reinterpret_cast<decltype(canvas_scaler_setres_hook)*>(canvas_scaler_setres_orig)(_this, res);
	}

	void (*text_assign_font)(void*);
	int (*text_get_size)(void*);
	void (*text_set_size)(void*, int);
	float (*text_get_linespacing)(void*);
	void (*text_set_style)(void*, int);
	void (*text_set_linespacing)(void*, float);

	void* on_populate_orig = nullptr;
	void on_populate_hook(void* _this, void* toFill)
	{
		if (text_get_linespacing(_this) != 1.05f)
		{
			text_assign_font(_this);
			text_set_style(_this, 1);
			text_set_size(_this, text_get_size(_this) - 4);
			text_set_linespacing(_this, 1.05f);
		}

		return reinterpret_cast<decltype(on_populate_hook)*>(on_populate_orig)(_this, toFill);
	}

	void* set_resolution_orig;
	void set_resolution_hook(int width, int height, bool fullscreen)
	{
		Resolution_t r;
		r = *get_resolution(&r);

		bool need_fullscreen = false;

		if (is_virt() && r.width / static_cast<double>(r.height) == (9.0 / 16.0))
			need_fullscreen = true;
		else if (!is_virt() && r.width / static_cast<double>(r.height) == (16.0 / 9.0))
			need_fullscreen = true;

		return reinterpret_cast<decltype(set_resolution_hook)*>(set_resolution_orig)(
			need_fullscreen ? r.width : width, need_fullscreen ? r.height : height, need_fullscreen
			);
	}

	void adjust_size()
	{
		thread([]() {
			auto tr = il2cpp_thread_attach(il2cpp_domain_get());

			Resolution_t r;
			get_resolution_stub(&r);

			auto target_height = r.height - 100;

			set_resolution_hook(target_height * 0.5625f, target_height, false);

			il2cpp_thread_detach(tr);
			}).detach();
	}

	void* load_scene_internal_orig = nullptr;
	void* load_scene_internal_hook(Il2CppString* sceneName, int sceneBuildIndex, void* parameters, bool mustCompleteNextFrame)
	{
		wprintf(L"%s\n", sceneName->start_char);
		return reinterpret_cast<decltype(load_scene_internal_hook)*>(load_scene_internal_orig)(sceneName, sceneBuildIndex, parameters, mustCompleteNextFrame);
	}

	void dump_all_entries()
	{
		// 0 is None
		for (int i = 1;; i++)
		{
			auto* str = reinterpret_cast<decltype(localize_get_hook)*>(localize_get_orig)(i);

			if (str && *str->start_char)
			{
				logger::write_entry(i, str->start_char);
			}
			else
			{
				// check next string, if it's still empty, then we are done!
				auto* nextStr = reinterpret_cast<decltype(localize_get_hook)*>(localize_get_orig)(i + 1);
				if (!(nextStr && *nextStr->start_char))
					break;
			}
		}
	}

	void path_game_assembly()
	{
		if (!mh_inited)
			return;

		printf("Trying to patch GameAssembly.dll...\n");

		auto il2cpp_module = GetModuleHandle("GameAssembly.dll");

		// load il2cpp exported functions
		il2cpp_symbols::init(il2cpp_module);

#pragma region HOOK_MACRO
#define ADD_HOOK(_name_, _fmt_) \
	auto _name_##_offset = reinterpret_cast<void*>(_name_##_addr); \
	\
	printf(_fmt_, _name_##_offset); \
	dump_bytes(_name_##_offset); \
	\
	MH_CreateHook(_name_##_offset, _name_##_hook, &_name_##_orig); \
	MH_EnableHook(_name_##_offset); 
#pragma endregion
#pragma region HOOK_ADDRESSES
		auto populate_with_errors_addr = il2cpp_symbols::get_method_pointer(
			"UnityEngine.TextRenderingModule.dll",
			"UnityEngine", "TextGenerator",
			"PopulateWithErrors", 3
		);

		// have to do this way because there's Get(TextId id) and Get(string id)
		// the string one looks like will not be called by elsewhere
		auto localize_get_addr = il2cpp_symbols::find_method("umamusume.dll", "Gallop", "Localize", [](const MethodInfo* method) {
			return method->name == "Get"s &&
				method->parameters->parameter_type->type == IL2CPP_TYPE_VALUETYPE;
			});

		auto query_ctor_addr = il2cpp_symbols::get_method_pointer(
			"LibNative.Runtime.dll", "LibNative.Sqlite3",
			"Query", ".ctor", 2
		);

		auto query_getstr_addr = il2cpp_symbols::get_method_pointer(
			"LibNative.Runtime.dll", "LibNative.Sqlite3",
			"Query", "GetText", 1
		);

		auto query_dispose_addr = il2cpp_symbols::get_method_pointer(
			"LibNative.Runtime.dll", "LibNative.Sqlite3",
			"Query", "Dispose", 0
		);

		auto set_fps_addr = il2cpp_symbols::get_method_pointer(
			"UnityEngine.CoreModule.dll", "UnityEngine",
			"Application", "set_targetFrameRate", 1
		);

		auto wndproc_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"StandaloneWindowResize", "WndProc", 4
		);

		auto get_virt_size_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"StandaloneWindowResize", "getOptimizedWindowSizeVirt", 2
		);

		auto get_hori_size_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"StandaloneWindowResize", "getOptimizedWindowSizeHori", 2
		);

		is_virt = reinterpret_cast<bool(*)()>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"StandaloneWindowResize", "get_IsVirt", 0
			));

		get_resolution = reinterpret_cast<Resolution_t * (*)(Resolution_t*)>(
			il2cpp_symbols::get_method_pointer(
				"UnityEngine.CoreModule.dll", "UnityEngine",
				"Screen", "get_currentResolution", 0
			)
			);

		auto gallop_get_screenheight_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"Screen", "get_Height", 0
		);

		auto gallop_get_screenwidth_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"Screen", "get_Width", 0
		);

		auto change_resize_ui_for_pc_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"UIManager", "ChangeResizeUIForPC", 2
		);

		auto canvas_scaler_setres_addr = il2cpp_symbols::get_method_pointer(
			"UnityEngine.UI.dll", "UnityEngine.UI",
			"CanvasScaler", "set_referenceResolution", 1
		);

		set_scale_factor = reinterpret_cast<void(*)(void*, float)>(
			il2cpp_symbols::get_method_pointer(
				"UnityEngine.UI.dll", "UnityEngine.UI",
				"CanvasScaler", "set_scaleFactor", 1
			)
			);

		auto on_populate_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"TextCommon", "OnPopulateMesh", 1
		);

		text_assign_font = reinterpret_cast<void(*)(void*)>(
			il2cpp_symbols::get_method_pointer(
				"UnityEngine.UI.dll", "UnityEngine.UI",
				"Text", "AssignDefaultFont", 0
			)
			);

		text_get_size = reinterpret_cast<int(*)(void*)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"TextCommon", "get_FontSize", 0
			)
			);

		text_set_size = reinterpret_cast<void(*)(void*, int)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"TextCommon", "set_FontSize", 1
			)
			);

		text_get_linespacing = reinterpret_cast<float(*)(void*)>(
			il2cpp_symbols::get_method_pointer(
				"UnityEngine.UI.dll", "UnityEngine.UI",
				"Text", "get_lineSpacing", 0
			)
			);

		text_set_style = reinterpret_cast<void(*)(void*, int)>(
			il2cpp_symbols::get_method_pointer(
				"UnityEngine.UI.dll", "UnityEngine.UI",
				"Text", "set_fontStyle", 1
			)
			);

		text_set_linespacing = reinterpret_cast<void(*)(void*, float)>(
			il2cpp_symbols::get_method_pointer(
				"UnityEngine.UI.dll", "UnityEngine.UI",
				"Text", "set_lineSpacing", 1
			)
			);

		auto set_resolution_addr = il2cpp_symbols::get_method_pointer(
			"UnityEngine.CoreModule.dll", "UnityEngine",
			"Screen", "SetResolution", 3
		);

		auto load_scene_internal_addr = il2cpp_resolve_icall("UnityEngine.SceneManagement.SceneManager::LoadSceneAsyncNameIndexInternal_Injected(System.String,System.Int32,UnityEngine.SceneManagement.LoadSceneParameters&,System.Boolean)");
#pragma endregion

		if (!compatible_mode) {
			// hook UnityEngine.TextGenerator::PopulateWithErrors to modify text
			ADD_HOOK(populate_with_errors, "UnityEngine.TextGenerator::PopulateWithErrors at %p\n");

			// Looks like they store all localized texts that used by code in a dict
			ADD_HOOK(localize_get, "Gallop.Localize.Get(TextId) at %p\n");

			ADD_HOOK(query_ctor, "Query::ctor at %p\n");
			ADD_HOOK(query_getstr, "Query::GetString at %p\n");
			ADD_HOOK(query_dispose, "Query::Dispose at %p\n");

			// ADD_HOOK(load_scene_internal, "SceneManager::LoadSceneAsyncNameIndexInternal at %p\n");

			if (g_replace_font)
			{
				ADD_HOOK(on_populate, "Gallop.TextCommon::OnPopulateMesh at %p\n");
			}

			if (g_max_fps > -1)
			{
				// break 30-40fps limit
				ADD_HOOK(set_fps, "UnityEngine.Application.set_targetFrameRate at %p \n");
			}

			if (g_unlock_size)
			{
				// break 1080p size limit
				ADD_HOOK(get_virt_size, "Gallop.StandaloneWindowResize.getOptimizedWindowSizeVirt at %p \n");
				ADD_HOOK(get_hori_size, "Gallop.StandaloneWindowResize.getOptimizedWindowSizeHori at %p \n");
				ADD_HOOK(wndproc, "Gallop.StandaloneWindowResize.WndProc at %p \n");

				// remove fixed 1080p render resolution
				ADD_HOOK(gallop_get_screenheight, "Gallop.Screen::get_Height at %p\n");
				ADD_HOOK(gallop_get_screenwidth, "Gallop.Screen::get_Width at %p\n");

				ADD_HOOK(canvas_scaler_setres, "UnityEngine.UI.CanvasScaler::set_referenceResolution at %p\n");
			}

			if (g_auto_fullscreen)
			{
				ADD_HOOK(set_resolution, "UnityEngine.Screen.SetResolution(int, int, bool) at %p\n");
				adjust_size();
			}

			if (g_dump_entries)
				dump_all_entries();
		}
	}
}

bool init_hook()
{
	if (mh_inited)
		return false;

	if (MH_Initialize() != MH_OK)
		return false;

	mh_inited = true;

	if (compatible_mode)
		std::this_thread::sleep_for(std::chrono::seconds(10));
	MH_CreateHook(LoadLibraryW, load_library_w_hook, &load_library_w_orig);
	MH_EnableHook(LoadLibraryW);

	notifier::ping();

	return true;
}

void uninit_hook()
{
	if (!mh_inited)
		return;

	MH_DisableHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}
