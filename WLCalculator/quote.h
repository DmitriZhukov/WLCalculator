#pragma once


struct Quote
{
	int Id;
	double First;
	double Last;

	double Change() const
	{
		return 1 - Last/First;
	}

	Quote(int id, double price)
	{
		Id = id;
		First = Last = price;
	}
};