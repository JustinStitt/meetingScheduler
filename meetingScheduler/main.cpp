#include <iostream>
#include <vector>
#include <string>
#include "schedule.hpp"

using namespace std;

int main()
{
	cout << "I am Schedule 1!" << endl;
	Schedule p1({ {"9:30","12:30"},{"13:30","15:45"} }, { "8:00","18:00" });
	cout << "I am Schedule 2!" << endl;
	Schedule p2({ {"9:30","12:30"},{"13:30","15:45"} }, { "8:00","18:00" });
	
	cout << "Start the meeting during these time windows to be on common availability! : " << endl;
	p1.print_sched(p1.compare_schedules(p2, 30));

	cin.get();
	return 0;
}