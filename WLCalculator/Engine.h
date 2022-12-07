#pragma once

#include <set>
#include <vector>
#include <map>
#include <algorithm>

#include "IEngine.h"
#include "quote.h"



struct compareMore {
	bool operator() (const Quote* lhs, const Quote* rhs) const {
		return (lhs->Change() > rhs->Change());
	}
};

using QuoteSet = std::multiset<Quote*, compareMore>;
using QuoteVector = std::vector<Quote*>;
using QuotesSetIterator = std::multiset<Quote*, compareMore>::iterator;


struct QuoteMapEntry
{
	Quote* pQuote;
	QuotesSetIterator Iterator;

	QuoteMapEntry(Quote *_pQuote, QuotesSetIterator _it)
	{
		pQuote = _pQuote;
		Iterator = _it;
	}
};

using QuoteMap = std::map<int, QuoteMapEntry>;

class Engine : public IEngine
{
private:

	const int MaxGainers = 10;
	const int MaxLosers = 10;

	// cached containers for updates no need to sort
	QuoteCacheVector _gainers;
	QuoteCacheVector _losers;

	// Sorted Multi Set stores price changes
	QuoteSet _sortedChangesSet;
	// Map for fast stock search
	std::map<int, QuoteMapEntry> _quotesMap;

	// methods

	QuoteVector GetLosers();
	QuoteVector GetGainers();

	void UpdateCache(QuoteCacheVector &qcv, const QuoteVector &qv, ICallback *ptr);


public:

	void OnQuote(int stock_id, double price);
	void Clean();
	~Engine();
};
