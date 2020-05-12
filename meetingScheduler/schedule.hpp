#pragma once
#include <iostream>
#include <vector>
#include <string>

using namespace std;

template<typename T>
using matrix = vector<vector<T>>;

class Schedule
{
private:
	int time_string_to_int(string time);
	string time_int_to_string(int time_in_minutes);
	matrix<int> matrix_string_to_int(matrix<string>& _sched);
	matrix<string> matrix_int_to_string(matrix<int>& _sched);
	matrix<int> calculate_availability(const matrix<int>& _sched);
public:
	matrix<string> sched;
	vector<string> busy;
	Schedule(matrix<string>, vector<string>);
	template<typename T>
	void print_sched(matrix<T> to_print);
	matrix<string> compare_schedules(Schedule& s2, int duration);
};

Schedule::Schedule(matrix<string> _sched, vector<string> _busy)
{
	this->sched = _sched;
	this->busy = _busy;

	//we want to add (busy) as a meeting (taken / non-available) time slot
	vector<string> early = { "00:00",this->busy[0]};
	vector<string> late = { this->busy[1], "24:00" };
	this->sched.insert(this->sched.cbegin(),early);
	this->sched.push_back(late);

	matrix<int> conv_to_int = matrix_string_to_int(this->sched);
	print_sched(this->sched);
	cout << "My Availability! : " << endl;
	matrix<int> avail = calculate_availability(conv_to_int);
	matrix<string> avail_string = matrix_int_to_string(avail);
	print_sched(avail_string);
	cout << string(50, '=') << endl;

}

int Schedule::time_string_to_int(string time)
{
	//find colon to use as delimiter and separate into lhs and rhs
	string lhs, rhs;//hours:minutes
	char delim = ':';
	for (int x{}; x < time.size(); ++x)
		if (time[x] == delim)
		{
			lhs = string(time.begin(), time.begin() + x);
			rhs = string(time.begin() + x + 1, time.end());
		}
	int hours, minutes;
	hours = stoi(lhs);
	minutes = stoi(rhs) + (hours * 60);
	return minutes;
}

string Schedule::time_int_to_string(int time)
{
	int hours, minutes;
	hours = (time / 60);
	minutes = (time - (hours * 60));
	string res = (to_string(hours) + ":" + to_string(minutes));
	if (!minutes)
		res.push_back('0');
	return res;
}

matrix<int> Schedule::matrix_string_to_int(matrix<string>& s)
{
	matrix<int> ret;
	for (int x{}; x < s.size(); ++x)
	{
		ret.push_back({});
		for (int y{}; y < s[x].size(); ++y)
		{
			ret[x].push_back(time_string_to_int(s[x][y]));
		}
	}
	return ret;
}

matrix<string> Schedule::matrix_int_to_string(matrix<int>& s)
{
	matrix<string> ret;
	for (int x{}; x < s.size(); ++x)
	{
		ret.push_back({});
		for (int y{}; y < s[x].size(); ++y)
		{
			ret[x].push_back(time_int_to_string(s[x][y]));
		}
	}
	return ret;
}


matrix<int> Schedule::calculate_availability(const matrix<int>& _sched)
{
	matrix<int> available;
	if (!_sched.size())
		available.push_back({0,1440});
	for (int x{}; x < _sched.size() - 1; ++x)
	{
		vector<int> slot;
		slot.push_back(_sched[x][1]);
		slot.push_back(_sched[x + 1][0]);
		available.push_back(slot);
	}
	if (_sched[_sched.size() - 1][1] < 1440)//if the end of our day is uncapped, then add availability till midnight (1440)
		available.push_back({ _sched[_sched.size() - 1][1] ,1440});

	return available;
}


bool in_range(const int& val, const vector<int>& slot)
{
	return (val >= slot[0] && val <= slot[1]) ? true : false;
}

matrix<string> Schedule::compare_schedules(Schedule& s2, int duration)
{
	/*
		compare our schedule to s2 and see compatible availability times
	*/
	matrix<string> common_avail;
	matrix<int> lhs, rhs, common;
	lhs = calculate_availability(matrix_string_to_int(this->sched));
	rhs = calculate_availability(matrix_string_to_int(s2.sched));

	for (int x{}; x < lhs.size(); ++x)
	{
		int lhs_s = lhs[x][0];
		int lhs_e = lhs[x][1];
		for (int y{}; y < rhs.size(); ++y)
		{
			int rhs_s = rhs[y][0];
			int rhs_e = rhs[y][1];
			if (in_range(lhs_s, rhs[y]) && in_range(lhs_e, rhs[y]) )
			{
				common.push_back({lhs_s,lhs_e - duration});
			}
			else if (in_range(lhs_s, rhs[y]))
			{//if the left hand cursor is in the range of s2's available time slots
				if (lhs_s + duration <= rhs_e)
					common.push_back({ lhs_s,rhs_e - duration });
			}
			else if (in_range(lhs_e, rhs[y]))
			{
				if (rhs_s + duration <= lhs_e)
					common.push_back({ rhs_s,lhs_e - duration });
			}
		}
	}
	common_avail = matrix_int_to_string(common);
	return common_avail;
}



template<typename T>
void Schedule::print_sched(matrix<T> s)
{
	for (const vector<T>& row : s)
	{
		for (const T& col : row)
		{
			cout << " " << col;
		}
		cout << endl;
	}
}

/*
example schedule
sched = [["9:00","10:30"],["12:00","13:30"], ["15:00","16:30"] ]
*/