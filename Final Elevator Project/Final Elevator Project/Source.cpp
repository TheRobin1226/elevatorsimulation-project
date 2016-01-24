#include <iostream>           
#include <thread>             
#include <mutex>              
#include <condition_variable> 
#include <vector>
#include <ostream>
// add people
// generate elevator
using namespace std;
	mutex mtx; 
	condition_variable cv;
	int current_floor = 1;
	int index = 0;

/* Consumer */
	class Human {
		int id;
	public:
		int current_floor;
		int next_floor;
		bool get_Up;
		Human() {}
		Human(int i) { id = i; }

		int getID() { return id; }
		void setID(int i) { id = i; }
		void initialize(vector<Human> &v) {
			current_floor = rand() % 5;
			next_floor = rand() % 5;
			while (current_floor == next_floor)
				next_floor = rand() % 5;
			if (current_floor > next_floor) {
				get_Up = false;
			}
			else {
				get_Up = true;
			}
			v.push_back(*this);
		}
			
		friend ostream& operator<<(ostream& os, const Human& h);
	};

void makePeople(vector<Human> &v) {
	unique_lock<mutex> lck(mtx);
	for (int i = 0; i < 10; i++) {
		int random = rand() % 100 + 1;
		Human one(random);
		one.initialize(v);
	}
	for (Human s : v)
		cout << s << " ";
	cout << endl;
}

void movePeople(vector<Human> &v) {
	unique_lock<mutex> lck(mtx);
	while (index == v.size()) cv.wait(lck);
	current_floor = v[index].current_floor;
	cout << " Elevator is picking up person:  " << v[index].getID() << " at floor " << current_floor << endl;
	current_floor = v[index].next_floor;
	cout << " Elevator has dropped off person: " << v[index].getID() << " at floor " << current_floor << endl;
	index++;
	cv.notify_all();
}

ostream& operator<<(ostream& os, const Human& h)
{
	os << " Person " << h.id << " Current floor " << h.current_floor << " Next floor " << h.next_floor << " " << h.get_Up << endl;
	return os;
}



int main() {
	thread people[10];
	
	//thread elevator[10];
	
	vector<Human> going;
	makePeople(going);
	
	for (int i = 0; i < 10; i++) {
		people[i] = thread(movePeople, going);
		//elevator[i] = thread(movePeople, going);
	}
	for (int i = 0; i < 10; i++) {
		people[i].join();
		//elevator[i].join();
	}
	
	getchar();
	getchar();
	return 0;
}
