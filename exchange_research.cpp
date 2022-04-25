#include "exchange_research.h"
#include "g_exchange_graph.h"
#include "exchange_data.h"
#include "g_terminal.h"
#include "mediator.h"
#include "reading_sad.h"

#include <set>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

_delta_offers::_delta_offers(const _offers& a, const _offers& b)
{
	i64 end_price;
	if (a[1].price > a[0].price)
	{
		delta = 1;
		start = std::min(a.front().price, b.front().price);
		end_price = std::min(a.back().price, b.back().price);
	}
	else
	{
		delta = -1;
		start = std::max(a.front().price, b.front().price);
		end_price = std::max(a.back().price, b.back().price);
	}
	number.resize((end_price - start) * delta + 1, { 0, 0 });
	for (auto i : a)
	{
		i64 n = (i.price - start) * delta;
		if ((n >= 0) && (n < (i64)number.size())) number[n].old_number = i.number;
	}
	for (auto i : b)
	{
		i64 n = (i.price - start) * delta;
		if ((n >= 0) && (n < (i64)number.size())) number[n].new_number = i.number;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

_delta_supply_and_demand::_delta_supply_and_demand(const _supply_and_demand& a, const _supply_and_demand& b):
	demand(a.demand, b.demand), supply(a.supply, b.supply)
{
}

_delta_supply_and_demand operator-(const _supply_and_demand& a, const _supply_and_demand& b)
{
	return _delta_supply_and_demand(b, a);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void test_font(_g_terminal* t)
{
/*	constexpr uint zebra2[9] = {0xffeaeaf6, 0xffe8e8f4, 0xffe6e6f2, 0xffe4e4f0, 0xffe2e2ee, 0xffdfdfeb,
		0xffdddde9, 0xffdbdbe7, 0xffd9d9e5 };
	_bitmap a(20, 20);
	a.clear(0xffffffff);
	a.line({ 0,0 }, { 20,0 }, zebra2[0]);
	a.line({ 0,1 }, { 20,1 }, zebra2[1]);
	a.line({ 0,2 }, { 20,2 }, zebra2[2]);
	a.line({ 0,3 }, { 20,3 }, zebra2[3]);
	a.line({ 0,4 }, { 20,4 }, zebra2[4]);
	a.line({ 0,5 }, { 20,5 }, zebra2[5]);
	a.line({ 0,6 }, { 20,6 }, zebra2[6]);
	a.line({ 0,7 }, { 20,7 }, zebra2[7]);
	a.line({ 0,8 }, { 20,8 }, zebra2[8]);
	a.set_font(L"Gadugi", false);
	a.text({ 0, -4 }, L"012", 16, 0xffff0000);
	a.save_to_file(L"e:\\aa.bmp");*/

	read_sad_from_screen();
/*	_bitmap a(20, 20);
	a.set_font(L"Gadugi", false);
	u64 ggg[20][9][6];
	for (i64 i = 0; i < 10; i++)
	{
		a.clear(0xffffffff);
		a.text({ 0, -4 }, std::to_wstring(i), 16, 0xff0000ff);
		for (i64 y = 0; y < 9; y++)
			for (i64 x = 0; x < 6; x++)
				ggg[i][y][x] = a.scan_line(y)[x] & 0xffffff;
	}
	for (i64 i = 0; i < 10; i++)
	{
		a.clear(0xffffffff);
		a.text({ 0, -4 }, std::to_wstring(i), 16, 0xffff0000);
		for (i64 y = 0; y < 9; y++)
			for (i64 x = 0; x < 6; x++)
				ggg[10+i][y][x] = a.scan_line(y)[x] & 0xffffff;
	}
	for (i64 y = 0; y < 9; y++)
		for (i64 x = 1; x < 5; x++)
			for (i64 y2 = 0; y2 < 9; y2++)
				for (i64 x2 = 1; x2 < 5; x2++)
				{
					if (y == y2 && x == x2) continue;
					std::set<u64> f;
					for (i64 i = 0; i < 20; i++)
					{
						if (ggg[i][y][x] == 0xffffff && ggg[i][y2][x2] == 0xffffff) continue;
						f.insert((ggg[i][y][x] << 32) + ggg[i][y2][x2]);
					}
					if (f.size() == 20)
						t->print(L"y: " + std::to_wstring(y) + L" x: " + std::to_wstring(x)+ L" y2: " + std::to_wstring(y2) + L" x2: " + std::to_wstring(x2));
		}*/

/*	a.set_font(L"Gadugi", false);
	a.resize({ 100, 100 });
	a.clear(0xff000000);
	a.fill_rectangle({ {0, 100}, {50, 100} }, 0xffffffff);
	a.text({ 0, 0 }, L"0123456789", 16, 0xffff0000);
	a.text({ 0, 15 }, L"0123456789", 16, 0xff00ff00);
	a.text({ 0, 30 }, L"0123456789", 16, 0xff0000ff);
	a.text({ 0, 50 }, L"0123456789", 16, 0xffff0000);
	a.text({ 0, 60 }, L"0123456789", 16, 0xff00ff00);
	a.text({ 0, 70 }, L"0123456789", 16, 0xff0000ff);
	a.text({ 0, 80 }, L"0123456789", 16, 0xff000000);*/
//	bb.save_to_file(L"e:\\aa.bmp");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void exchange_fun1(_g_terminal* t)
{ // общая информация
	start_stock();
	t->print(L"количество цен: " + std::to_wstring(ed.size()));
	t->print(L"размер сжатой записи: " + double_to_wstring(double(ed.info_compressed_size) / ed.size(), 1)); // 20.2
	const _supply_and_demand* prev = nullptr;
	for (auto& i : ed)
	{
		if (prev)
			if (i.time - prev->time == 1)
			{
				auto ee = i - *prev;
			}
		prev = &i;
	}
	if (prev) *t << *prev;
}

void exchange_fun2(_g_terminal* t, std::vector<std::wstring>& parameters)
{ // вывод конкретных цен
	if (parameters.empty()) return;
	start_stock();
	auto n = std::stoll(parameters[0]);
	if ((n < 0) || (n >= (i64)ed.size())) return;
	*t << ed[n];
}

void exchange_fun3(_g_terminal* t, std::vector<std::wstring>& parameters)
{ // вывод сравнения цен
	if (parameters.size() != 2) return;
	start_stock();
	auto n1 = std::stoll(parameters[0]);
	if ((n1 < 0) || (n1 >= (i64)ed.size())) return;
	auto n2 = std::stoll(parameters[1]);
	if ((n2 < 0) || (n2 >= (i64)ed.size())) return;
	auto delta = ed[n2] - ed[n1];
	*t << delta;
}

_g_terminal& operator << (_g_terminal& t, const _delta_supply_and_demand& delta)
{
	for (i64 i = delta.supply.number.size() - 1; i >= 0; i--)
	{
		auto num1 = delta.supply.number[i].old_number;
		auto num2 = delta.supply.number[i].new_number;
		if (num1 == 0 && num2 == 0) continue;
		t.print(double_to_wstring((delta.supply.start + i * delta.supply.delta) * c_unpak, 2) +	L": " +
			std::to_wstring(num1) + ((num1 == num2)?L" == ":((num1 > num2) ? L" -- " : L" ++ ")) +
			std::to_wstring(num2));
	}
	t.print(L"-------");
	for (i64 i = 0; i < (i64)delta.demand.number.size(); i++)
	{
		auto num1 = delta.demand.number[i].old_number;
		auto num2 = delta.demand.number[i].new_number;
		if (num1 == 0 && num2 == 0) continue;
		t.print(double_to_wstring((delta.demand.start + i * delta.demand.delta) * c_unpak, 2) + L": " +
			std::to_wstring(num1) + ((num1 == num2) ? L" == " : ((num1 > num2) ? L" -- " : L" ++ ")) +
			std::to_wstring(num2));
	}
	return t;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
