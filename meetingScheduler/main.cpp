#include <iostream>
#include <vector>
#include <string>
#include "schedule.hpp"

using namespace std;

int main()
{
	cout << "I am Schedule 1!" << endl;
	Schedule p1({ {"9:30","12:30"},{"13:45","14:15"},{"15:00","15:20"} }, { "8:00","18:00" });
	cout << "I am Schedule 2!" << endl;
	Schedule p2({ {"11:15","12:45"},{"13:00","13:30"},{"16:30","17:00"} }, { "7:00","19:00" });
	
	cout << "Start the meeting during these time windows to be on common availability! : " << endl;
	p1.print_sched(p1.compare_schedules(p2, 30));

	cin.get();
	return 0;
}