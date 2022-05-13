#include "ed_sampling.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ed_sampling_::ed_sampling_(const _exchange_data& _ed) : data(ed.size())
{
	for (int i = 0; i < _ed.size(); i++) data[i].index = i;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ed_sampling_ fltr_demand(const ed_sampling_& eds)
{
	ed_sampling_ result;
	result.data.reserve(eds.data.size());
	for (auto i : eds.data)
	{
		i.supply_filter = 0;
		if (i.demand_filter) result.data.push_back(i);
	}
	return result;
}

ed_sampling_ fltr_supply(const ed_sampling_& eds)
{
	ed_sampling_ result;
	result.data.reserve(eds.data.size());
	for (auto i : eds.data)
	{
		i.demand_filter = 0;
		if (i.supply_filter) result.data.push_back(i);
	}
	return result;
}
