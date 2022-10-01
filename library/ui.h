#pragma once

#include "graphics.h"
#include <memory>
#include <optional>

struct _ui_element
{
private:
	_size local_size;
	_trans trans;
	std::optional<_area> area;
	std::vector<std::shared_ptr<_ui_element>> subelements;
};

struct _ui
{
private:
	std::shared_ptr<_ui_element> root;
	_bitmap canvas;
	_area changed_area;
};