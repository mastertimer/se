#pragma once

#include "g_exchange_graph.h"
#include "ui.h"

struct _e_exchange_graph : public _ui_element
{
	std::vector<_basic_curve*> curve; // кривая, 2я версия
	i64 size_el = 6; // размер элемента

	_e_exchange_graph(_ui* ui_);
	~_e_exchange_graph();
	void ris2(_trans tr) override;
	void update() override;
	bool mouse_down_left2(_xy r) override; // начало перетаскивания
	void mouse_move_left2(_xy r) override; // процесс перетаскивания

private:
	i64 x_tani = 0; // предыдущая координата x при перетаскивании
	int v_vib = 0; // диапазон полосы прокрутки
};

inline _e_exchange_graph* egraph = nullptr; // график
