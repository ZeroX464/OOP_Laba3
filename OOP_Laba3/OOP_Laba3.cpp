#include <stdio.h>
#include <memory>
using namespace std;

class Interface {
public:
	virtual void printInf() = 0;
	virtual ~Interface() {}
};

class Number : public Interface {
private:
	int value;
public:
	Number() : value(0) {}
	Number(int value) : value(value) {}
	Number(const Number& s) : value(s.value) {}
	~Number() {}
	void printInf() override {
		printf("Number object with value: %d\n", value);
	}
};

class Point : public Interface {
private:
    int x, y, z;
public:
    Point() : x(0), y(0), z(0) {}
    Point(int x, int y, int z) : x(x), y(y), z(z) {}
    Point(const Point& p) : x(p.x), y(p.y), z(p.z) {}
    ~Point() {}
	void printInf() override {
		printf("Point object with coordinates: %d, %d, %d\n", x, y, z);
	}
};

class Container {
private:
	Interface** array;
	int capacity;
	int size;
public:
	Container() : capacity(0), size(0), array(new Interface*[0]) {} //???
	Container(int capacity) : capacity(capacity), size(0), array(new Interface* [capacity]) {}
	Container(const Container& c) : capacity(c.capacity), size(c.size), array(new Interface*[c.capacity]) {}
	~Container() {
		clear();
	}

	void clear() {
		for (int i = 0; i < size; i++) {
			delete array[i];
		}
		size = 0;
	}

	void push_back(Interface* element) {
		if (size >= capacity) {
			capacity *= 2;
			Interface** newArr = new Interface* [capacity];
			for (int i = 0; i < size; i++) {
				newArr[i] = array[i];
			}
			delete[] array;
			array = newArr;
		}
		array[size++] = element; //unique_ptr
	}
	void push_front(Interface* element) {
		if (size >= capacity) { capacity *= 2; }
		Interface** newArr = new Interface * [capacity];
		newArr[0] = element;
		size++;
		for (int i = 0; i < size; i++) {
			newArr[i + 1] = array[i]; //unique_ptr
		}
		delete[] array;
		array = newArr;
	}
	void push_middle(Interface* element) {
		if (size >= capacity) { capacity *= 2; }
		Interface** newArr = new Interface * [capacity];
		newArr[size / 2] = element;
		size++;
		for (int i = 0; i < size; i++) {
			if (i >= size / 2) { newArr[i + 1] = array[i]; }
			else { newArr[i] = array[i]; }
		}
		delete[] array;
		array = newArr;
	}
	void kickObj() {

	}
	void deleteObj() {

	}
	void currentObj() {

	}
	void nextObj() {

	}
	void prevObj() {

	}
	void isExist() {

	}
	Interface* operator[](int index) { //Обращение по индексу
		if (index >= 0 && index < size) {
			return array[index];
		}
		return nullptr;
	}

	int getSize() {
		return size;
	}
	int getCapacity() {
		return capacity;
	}
};

int main() {
	Container c(2);
	for (int i = 0; i < 10; i++) {
		if (i % 2) {
			c.push_back(new Point(i, i, i));
		}
		else {
			c.push_back(new Number(i));
		}
	}
	c.push_front(new Point(4, 3, 2));
	//c.push_middle(new Number(7));
	for (int i = 0; i < c.getSize(); i++) {
		c[i]->printInf();
	}
	printf("End");
	return 0;
}