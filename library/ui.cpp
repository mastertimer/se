#include "ui.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

_ui_element::_ui_element(_ui& ui_): ui(ui_)
{
}

_ui_element::~_ui_element()
{
}

void _ui_element::ris2(_trans tr)
{
}

void _ui_element::run()
{
	auto copy_subelements = subelements;
	for (auto element : copy_subelements) element->run();
}

void _ui_element::key_down(ushort key)
{
}

void _ui_element::key_press(ushort key)
{
}

bool _ui_element::mouse_wheel2(_xy r)
{
	return false;
}

bool _ui_element::mouse_down_left2(_xy r)
{
	return false;
}

void _ui_element::mouse_move_left2(_xy r)
{
}

void _ui_element::cha_area(std::optional<_area> a)
{
	if (!a) a = local_area;
	if (!parent)
	{
		if (ui.n_ko.get() == this) ui.add_changed_area(trans(*a));
		return;
	}
	parent->cha_area(trans(*a));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void _ui::add_changed_area(_area a)
{
	if (a.empty()) return;
	if ((a.x.max < 0) || (a.x.min > canvas.size.x) || (a.y.max < 0) || (a.y.min > canvas.size.y)) return;
	changed_area |= a;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
