#include "reading_sad.h"
#include "graphics.h"

#include <chrono>
#include <map>

// шрифт цен: "Gadugi" 16

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace
{
	constexpr bool debug = true;
	_bitmap image;

	struct FindWnd
	{
		std::wstring class_name_; // имя класса
		std::wstring window_name_; // имя окна
		HWND hwnd_{}; // указатель окна
	};

	BOOL CALLBACK PoiskOkna(HWND hwnd, LPARAM lParam)
	{
		FindWnd* aa = (FindWnd*)lParam;
		wchar_t str[255];
		GetWindowText(hwnd, str, 255);
		if (aa->window_name_ != str) return TRUE;
		GetClassName(hwnd, str, 255);
		if (aa->class_name_ != str) return TRUE;
		aa->hwnd_ = hwnd;
		return FALSE;


		//FindWnd* aa = (FindWnd*)lParam;
		//wchar_t str[255];
		//wstring s;
		//GetWindowText(hwnd, str, 255);
		//if (aa->window_name_ != str) return TRUE;
		//s = str;
		//GetClassName(hwnd, str, 255);
		//s = s + L" : " + str;
		//MessageBox(0, s.c_str(), L"упс", MB_OK | MB_TASKMODAL);

		//if (aa->class_name_ != str) return TRUE;
		//aa->hwnd_ = hwnd;
		//return FALSE;

	}

	HWND FindSubWindow(HWND w, const wchar_t* classname, const wchar_t* windowname)
	{
		//		HWND w2 = FindWindowEx(w, 0, L"HostWindow", 0);
		FindWnd aa;
		aa.class_name_ = classname;
		aa.window_name_ = windowname;
		aa.hwnd_ = 0;
		//	EnumWindows(fnEnumWindowProc, 0);
		EnumChildWindows(w, PoiskOkna, (LPARAM)&aa);
		return aa.hwnd_;
	}

	void find_separator(i64& sep1, i64& sep2)
	{
		if (image.size.empty()) return;
		auto sl = image.scan_line2(0);
		for (i64 i = 0; i < image.size.x; i++)
			if (sl[i] != 0xffffffff)
			{
				sep1 = i;
				break;
			}
		for (i64 i = sep1 + 1; i < image.size.x; i++)
			if (sl[i] != 0xffffffff)
			{
				sep2 = i;
				break;
			}
	}

	const std::map<u64, wchar_t> gadugi_16{ {12, L'1'}, {13, L'2'} };

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::optional<_supply_and_demand> read_sad_from_screen()
{
	if (!debug)
	{
		HWND w = FindWindow(0, mmm3().c_str());
		if (!w) return std::nullopt;
		HWND w2 = FindSubWindow(w, L"InfoPriceTable", L"Сбербанк [МБ ФР: Т+ Акции и ДР] Котировки"); // InfoPriceTable HostWindow
		if (!w2) return std::nullopt;
		image.grab_ecran_oo2(w2);
	}
	else
	{
		static bool first = true;
		if (first)
		{
			first = false;
			image.load_from_file(L"e:\\scan\\ee1.bmp");
		}
	}
	i64 sep1 = 0, sep2 = 0;
	find_separator(sep1, sep2);
	_supply_and_demand pr;
	constexpr i64 step_height_line = 17;
	constexpr i64 font_height = 11;
	constexpr i64 font_width = 6;
	constexpr i64 comma_width = 3;
	constexpr i64 discharge_indent = 3;
	constexpr i64 indent_from_above = 19;
	constexpr i64 indent_from_separator = 5;
	const _ixy point1(3, 3); // первая опорная точки
	const _ixy point2(4, 4); // первая опорная точки
	if (sep1 < 40 || sep2-sep1 < 60) goto err; // 10 000 000 должно поместиться
	pr.time = time(0);

	if (image.size.y < indent_from_above + step_height_line * size_offer * 2 - (step_height_line - font_height))
		goto err;

	for (i64 j = 0; j < size_offer * 2; j++)
	{
		auto sl1 = image.scan_line2(indent_from_above + j * step_height_line + point1.y);
		auto sl2 = image.scan_line2(indent_from_above + j * step_height_line + point2.y);
		i64 x0 = sep1 - indent_from_separator - font_width;

		auto read_c = [&](i64 x) -> std::optional<int> 
			{
				u64 key = (u64(sl1[x + point1.x]) << 32) + sl2[x + point2.x];
				auto symb = gadugi_16.find(key);
				if (symb == gadugi_16.end()) return std::nullopt;
				wchar_t c = symb->second;
				if (c < L'0' || c > L'9') return std::nullopt;
				return c - L'0';
			};
		std::optional<i64> c;
		if (c = read_c(x0); !c) goto err;
		_offer off{ 0,0 };
		off.price = *c;
		if (c = read_c(x0 - font_width); !c) goto err;
		off.price += *c * 10;
		if (c = read_c(x0 - font_width * 2 - comma_width); !c) goto err;
		off.price += *c * 100;
		if (c = read_c(x0 - font_width * 3 - comma_width); !c) goto err;
		off.price += *c * 1000;
		if (c = read_c(x0 - font_width * 4 - comma_width); !c) goto err;
		off.price += *c * 10000;
		x0 = sep2 - indent_from_separator - font_width;
		if (c = read_c(x0); !c) goto err;
		off.number = *c;
		int k = 1;
		for (i64 i = 1; i < 9; i++) // обязательно стратит
		{
			k *= 10;
			x0 -= font_width;
			if (i % 3 == 0) x0 -= comma_width;
			if (c = read_c(x0); !c) break;
			off.number += *c * k;
		}

		// проверка что ничего не пропущено
		x0 += font_width - 2;
		auto sl = image.scan_line2(indent_from_above + j * step_height_line + font_height / 2);
		_color ccc = sl[sep2 + 2];
		for (i64 i = sep2 + 2; i <= x0; i++ )
			if (sl[i] != ccc) goto err; // должна быть линия сплошного цвета
		// --------- проверка что ничего не пропущено

		if (j < size_offer)
			pr.supply[size_offer - 1 - j] = off;
		else
			pr.demand[j - size_offer] = off;
	}
	if (pr.valid()) return pr;
err:
	if (!debug)
	{

	}
	return std::nullopt;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::optional<RECT> find_window_prices()
{
	HWND w = FindWindow(0, mmm3().c_str());
	if (!w) return std::nullopt;
	HWND w2 = FindSubWindow(w, L"InfoPriceTable", L"Сбербанк [МБ ФР: Т+ Акции и ДР] Котировки"); // InfoPriceTable HostWindow
	if (!w2) return std::nullopt;
	RECT rr;
	GetWindowRect(w2, &rr);
	return rr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
