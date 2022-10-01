#pragma once

#include "graphics.h"
#include <memory>
#include <optional>
#include <set>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _ui;

struct _ui_element : public std::enable_shared_from_this<_ui_element>
{

	_ui_element(_ui& ui_);
	virtual ~_ui_element();

protected:
	_ui& ui;
	_area local_area;
	bool key_fokus = false;
	_color c{ 0xFF208040 };
	_color c2{ 0 };

	virtual void ris2(_trans tr);
	virtual void run();
	virtual void key_down(ushort key);
	virtual void key_press(ushort key);
	virtual bool mouse_wheel2(_xy r);
	virtual bool mouse_down_left2(_xy r);
	virtual void mouse_move_left2(_xy r);

	void cha_area(std::optional<_area> a = std::nullopt);

private:
	_trans trans;
	std::optional<_area> area;
	std::set<std::shared_ptr<_ui_element>> subelements;
	std::shared_ptr<_ui_element> parent;

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _ui
{
	_bitmap canvas;
	_area changed_area;

	std::shared_ptr<_ui_element> n_ko;
	std::shared_ptr<_ui_element> n_act_key;
	std::set<std::shared_ptr<_ui_element>> n_timer1000;
	bool n_s_ctrl = false;
	i64 n_wheel = 0;

	_trans master_trans_go;

	const _color c00{ 0 };          // прозрачный цвет
	const _color cc0{ 0xFF000000 }; // цвет фона
	const _color cc1{ 0xFF208040 }; // цвет 1
	const _color cc2{ 0xFF40FF80 }; // цвет 2
	const _color cc3{ 0xFFA0FFC0 }; // цвет 3
	const _color cc4{ 0xFF104020 }; // цвет 4

	void add_changed_area(_area a);

private:
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
