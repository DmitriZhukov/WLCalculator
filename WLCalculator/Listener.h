#pragma once

class Listener : public ICallback
{

	void PrintQuotes(const QuoteCacheVector& qcv)
	{
		for (auto& q : qcv)
		{
			std::cout << "Id:" << q.Id << ": " << q.Change << ";";
		}

		std::cout << std::endl;
	}

public:
	void OnChanged(const QuoteCacheVector& qcv) override
	{
		PrintQuotes(qcv);
	}
};

