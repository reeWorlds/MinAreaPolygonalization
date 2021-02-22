#include "SegmentLinkedList.h"

SegmentLinkedList::SegmentLinkedList()
{
	last = -1;
}

void SegmentLinkedList::add(Segment s)
{
	if (last != -1)
	{
		m[last].second.second = m.size();
	}

	m.push_back({ s, {last, -1} });
	last = m.size() - 1;
}

void SegmentLinkedList::erase(int i)
{
	if (last == i)
	{
		last = m[i].second.first;
	}

	if (m[i].second.first != -1)
	{
		m[m[i].second.first].second.second = m[i].second.second;
	}
	if (m[i].second.second != -1)
	{
		m[m[i].second.second].second.first = m[i].second.first;
	}

	m[i].second = { -1, -1 };
}

void SegmentLinkedList::clearInvalidSegments()
{
	vector <Segment> newM;

	for (int i = last; i != -1;)
	{
		newM.push_back(m[i].first);

		i = m[i].second.first;
	}

	m.clear();
	
	if (!newM.empty())
	{
		m.push_back({ newM[0], {-1, -1} });
		for (int i = 1; i < newM.size(); i++)
		{
			m[i - 1].second.second = i;
			m.push_back({ newM[i], {i - 1, -1} });
		}
		last = newM.size() - 1;
	}
	else
	{
		last = -1;
	}
}
