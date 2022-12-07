#pragma once



struct QuoteCache
{
	int Id;
	double Change;

	QuoteCache(int id, double change) { Id = id; Change = change; }
};

using QuoteCacheVector = std::vector<QuoteCache>;

class ICallback
{
public:

	virtual void OnChanged(const QuoteCacheVector &qcv) = 0;
};

class IEngine
{
public:

	virtual void OnQuote(int stock_id, double price) = 0;
	ICallback* GainersCallbackPtr = nullptr;
	ICallback* LosersCallbackPtr = nullptr;
}; 

