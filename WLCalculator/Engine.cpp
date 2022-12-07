#include "Engine.h"
#include <iterator>


void Engine::OnQuote(int stock_id, double price)
{
	auto map_iterator = _quotesMap.find(stock_id);

	// new quote
	if (map_iterator == _quotesMap.end())
	{

		auto* pQuote = new Quote(stock_id, price);

		// insert into set
		auto set_iterator = _sortedChangesSet.insert(pQuote);

		// insert into map
		_quotesMap.insert(std::make_pair(stock_id,
			QuoteMapEntry(
				pQuote,
				set_iterator)
		));


	}
	// we have this quote
	else
	{
		// update last price
		map_iterator->second.pQuote->Last = price;

		// update set 
		_sortedChangesSet.erase(map_iterator->second.Iterator);
		map_iterator->second.Iterator = _sortedChangesSet.insert(map_iterator->second.pQuote);

		// update Gainers & Losers
		auto gainers = GetGainers();
		auto losers = GetLosers();
		
		// update cache and send notification if needed
		UpdateCache(_gainers, gainers, GainersCallbackPtr);
		UpdateCache(_losers, losers, LosersCallbackPtr);
	}
}

void Engine::Clean()
{
	for (auto& kvp : _quotesMap)
	{
		delete kvp.second.pQuote;
	}

	_quotesMap.clear();
	_sortedChangesSet.clear();
}

Engine::~Engine()
{
	Clean();
}

QuoteVector Engine::GetLosers()
{
	QuoteVector losers;

	auto rit = _sortedChangesSet.rbegin();
	int counter = 0;

	while (counter < MaxGainers && rit != _sortedChangesSet.rend() && (*rit)->Change() != 0)
	{
		losers.push_back(*rit);
		counter++;
		rit++;
	}

	return losers;
}


QuoteVector Engine::GetGainers()
{
	QuoteVector gainers;

	auto it = _sortedChangesSet.begin();
	int counter = 0;

	while (counter < MaxLosers && it != _sortedChangesSet.end() && (*it)->Change() != 0)
	{
		gainers.push_back(*it);
		counter++;
		it++;
	}

	return gainers;
}

// check if there is change
// send notification
void Engine::UpdateCache(QuoteCacheVector& qcv, const QuoteVector& qv, ICallback *ptr)
{
	bool ok = 
		qcv.size() == qv.size()
		&&
		std::equal(std::begin(qcv), std::end(qcv),
		begin(qv),
		[](const QuoteCache& qc, const Quote* q)
		{return qc.Id == q->Id && qc.Change == q->Change();});

	// update needed
	if (!ok)
	{
		qcv.clear();

		std::transform(std::begin(qv), std::end(qv),
			std::back_inserter(qcv), 
			[](const Quote* pq) {return QuoteCache(pq->Id, pq->Change());});

		// send notification
		if (ptr != nullptr)
			ptr->OnChanged(qcv);
	}
}

