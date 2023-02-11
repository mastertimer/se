﻿#pragma once

#include "exchange_basic.h"
#include "geometry.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _exchange_data: public std::vector<_supply_and_demand>
{
	i64 info_compressed_size = 0;

	void push_back(const _supply_and_demand& c);
	void save_to_file();
	void load_from_file();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _index           // разнообразные минутные коэффициенты
{
	_iinterval ncc;     // диапазон цен
	i64    time    = 0; // время (с обнуленной секундой (time%60 = 0))
	double min     = 0; // минимальная цена
	double max     = 0; // макимальная цена
	double first   = 0; // первая цена
	double last    = 0; // последняя цена
	double c3_buy  = 0; // цена покупки на 3-й секунде
	double c3_sale = 0; // цена продажи на 3-й секунде
	double minmin  = 0; // минимальная цена минимального спроса ([19])
	double maxmax  = 0; // максимальная цена максимального предложения ([19])
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool update_index_data();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline std::vector<_index> index_data; // поминутный вектор все коэффициенты
inline _exchange_data ed; // посекундные цены

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
