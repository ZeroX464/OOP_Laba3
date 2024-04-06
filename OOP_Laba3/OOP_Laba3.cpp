#include <stdio.h>
#include <memory>
#include <ctime>
#include <random>
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
	Container() : capacity(1), size(0), array(new Interface*[1]) {}
	Container(int capacity) : capacity(capacity), size(0), array(new Interface* [capacity]) {}
	Container(const Container& c) : capacity(c.capacity), size(c.size), array(new Interface*[c.capacity]) {}
	~Container() {
		delete[] array; // не clear т.к. объекты удалять не нужно
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
		array[size++] = element;
	}
	void push_front(Interface* element) {
		if (size >= capacity) { capacity *= 2; }
		Interface** newArr = new Interface * [capacity];
		newArr[0] = element;
		size++;
		for (int i = 0; i < size; i++) {
			newArr[i + 1] = array[i];
		}
		delete[] array;
		array = newArr;
	}
	void push_middle(Interface* element) {
		if (size >= capacity) { capacity *= 2; }
		Interface** newArr = new Interface * [capacity];
		size++;
		int middle = 0;
		if (size % 2 == 0) { middle = size / 2 - 1; }
		else { middle = size / 2; }
		for (int i = 0; i < middle; i++) { newArr[i] = array[i]; }
		newArr[middle] = element;
		for (int i = middle + 1; i < size; i++) { newArr[i] = array[i - 1]; }
		delete[] array;
		array = newArr;
	}
	void kickObj(Interface* element) {
		Interface** newArr = new Interface * [capacity];
		bool elemFind = false;
		size--;
		for (int i = 0; i < size; i++) {
			if (array[i] == element) {
				elemFind = true;
			}
			if (elemFind) { newArr[i] = array[i + 1]; }
			else { newArr[i] = array[i]; }
		}
		if (array[size] == element) { elemFind = true; } //Для последнего элемента
		delete[] array;
		array = newArr;
		if (!elemFind) {
			printf("Element below not found:\n");
			if (element != nullptr) { element->printInf(); }
			else { printf("Element doesn't exist\n"); }
		}

	}
	void deleteObj(Interface* element) {
		kickObj(element);
		delete element;
	}
	Interface* nextObj(Interface* element) {
		for (int i = 0; i < size; i++) {
			if (array[i] == element) {
				if (i + 1 == size) { throw "Next object doesn't exist\n"; }
				else { return array[i + 1]; }
			}
		}
		throw "This array doesn't contain this element\n";
		
	}
	Interface* prevObj(Interface* element) {
		for (int i = 0; i < size; i++) {
			if (array[i] == element) {
				if (i == 0) { throw "Previous object doesn't exist\n"; }
				else { return array[i - 1]; }
			}
		}
		throw "This array doesn't contain this element\n";
	}
	bool isExist(Interface* element) {
		for (int i = 0; i < size; i++) {
			if (array[i] == element) { return true; }
		}
		return false;
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
	void putInArray(int index, Interface* element) // Вставка элемента в массив по индексу
	{
		if (index >= 0 && index < size + 1) {
			if (size + 1 >= capacity) { capacity *= 2; }
			Interface** newArr = new Interface * [size + 1];
			for (int i = 0; i < index; i++) { newArr[i] = array[i]; }
			newArr[index] = element;
			for (int i = index + 1; i < size + 1; i++) { newArr[i] = array[i - 1]; }
			size++;
			delete[] array;
			array = newArr;
		}
		else { throw "Index out of massive range\n"; }
	}
};

int randomNumber(int start, int end) { return rand() % (end - start + 1) + start; }

int lastElem(int elemIndex) {
	if (elemIndex > 0) { return elemIndex - 1; }
	else { return 0; }
}

int main() {
	srand(time(0));

	Container c3;
	for (int i = 0; i < 2; i++) { c3.push_back(new Number(i)); }
	c3.push_middle(new Point);
	for (int i = 0; i < 3; i++) { c3[i]->printInf(); }
	printf("Size = %d\nCapacity = %d\n", c3.getSize(), c3.getCapacity());
	c3.clear();
	printf("--------------------------------------------\n");

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
	c.push_middle(new Number(7));
	Interface* obj = c[10];
	c.kickObj(c[10]);
	obj->printInf();
	obj = c[10];
	c.deleteObj(c[10]);
	//obj->printInf();
	try { c.nextObj(c[9])->printInf(); }
	catch (const char* error_message) { printf(error_message); }
	try { c.nextObj(c[10])->printInf(); }
	catch (const char* error_message) { printf(error_message); }
	try { c.prevObj(c[9])->printInf(); }
	catch (const char* error_message) { printf(error_message); }
	try { c.prevObj(c[0])->printInf(); }
	catch (const char* error_message) { printf(error_message); }
	Interface* n = new Number();
	//c.push_back(n);
	if (c.isExist(n)) { n->printInf(); }
	else { printf("n not in container\n"); }
	printf("--------------------------------------------\n");
	printf("Size = %d\nCapacity = %d\n", c.getSize(), c.getCapacity());
	for (int i = 0; i < c.getSize(); i++) { c[i]->printInf(); }
	c.clear();
	printf("--------------------------------------------\n");

	Container c2(1);
	int start_time = clock();
	for (int i = 0; i < 10000; i++) {
		switch (randomNumber(0,2)) {
		case 0: // Создание и вставка объекта
			switch (randomNumber(0,1)) {
			case 0: //Point
				c2.putInArray(randomNumber(0, lastElem(c2.getSize())), new Point(i,i,i));
				break;
			case 1://Number
				c2.putInArray(randomNumber(0, lastElem(c2.getSize())), new Number(i));
				break;
			}
			break;
		case 1: // Удаление и уничтожение случайного объекта
			switch (randomNumber(0, 1)) {
			case 0: // Удаление без уничтожения
				if (c2.getSize() > 0) { c2.kickObj(c2[randomNumber(0, lastElem(c2.getSize()))]); }
				break;
			case 1: // Удаление с уничтожением
				if (c2.getSize() > 0) { c2.deleteObj(c2[randomNumber(0, lastElem(c2.getSize()))]); }
				break;
			break;
			}
		case 2: // Вызов метода printinf у случайного объекта
			if (c2.getSize() > 0) { c2[randomNumber(0, lastElem(c2.getSize()))]->printInf(); }
			break;
		}
	}
	int end_time = clock();
	printf("Work time: %d mls\n", end_time - start_time);
	printf("Size = %d\nCapacity = %d\n", c2.getSize(), c2.getCapacity());
	c2.clear();
	printf("End");
	return 0;
}