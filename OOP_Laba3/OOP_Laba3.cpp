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
	Container() : capacity(1), size(0), array(new Interface*[capacity]) {} //???
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
		size++;
		int middle = 0;
		if (size % 2 == 0) {
			middle = size / 2 - 1;
			newArr[middle] = element;
		}
		else {
			middle = size / 2;
			newArr[middle] = element;
		}
		for (int i = 0; i < size; i++) {
			if (i >= middle) {
				newArr[i + 1] = array[i];
			}
			else {
				newArr[i] = array[i];
			}
			
		}
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
			element->printInf();
		}

	}
	void deleteObj(Interface* element) {
		kickObj(element);
		delete element;
	}
	void currentObj() { // ???

	}
	Interface* nextObj(Interface* element) {
		for (int i = 0; i < size; i++) {
			if (array[i] == element) {
				if (i + 1 == size) { throw "Next object isn't exist\n"; }
				else { return array[i + 1]; }
			}
		}
		throw "This array doesn't contain this element\n";
		
	}
	Interface* prevObj(Interface* element) {
		for (int i = 0; i < size; i++) {
			if (array[i] == element) {
				if (i == 0) { throw "Previous object isn't exist\n"; }
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
		Interface** newArr = new Interface * [size + 1];
		for (int i = 0; i < index; i++) { newArr[i] = array[i]; }
		newArr[index] = element;
		for (int i = index + 1; i < size + 1; i++) { newArr[i] = array[i - 1]; }
		size++;
		delete[] array;
		array = newArr;
	}
};

int randomNumber(int start, int end) { return rand() % (end - start + 1) + start; }

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
	c.push_middle(new Number(7));
	c.kickObj(c[11]);
	Interface* obj = c[10];
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
	for (int i = 0; i < c.getSize(); i++) {
		c[i]->printInf();
	}

	Container c2(100);
	int start_time = clock();
	for (int i = 0; i < 100; i++) {
		c2.putInArray(randomNumber(0, c.getSize() - 1), new Point());
	}
	/*for (int i = 0; i < 10; i++) {
		switch (randomNumber(0,2)) {
		case 0: // Создание и вставка объекта
			switch (randomNumber(0,1)) {
			case 0: //Point
				if (c2.getSize() > 0) { c2.putInArray(randomNumber(0, c.getSize() - 1), new Point()); }
				else { c2.push_back(new Point()); }
				break;
			case 1://Number
				//if (c2.getSize() > 0) { c2.putInArray(randomNumber(0, c.getSize() - 1), new Number()); }
				//else { c2.push_back(new Point()); }
				break;
			}
			break;
		case 1: // Удаление и уничтожение случайного объекта
			switch (randomNumber(0, 1)) {
			case 0: // Удаление без уничтожения
				//if (c2.getSize() > 0) { c.kickObj(c[randomNumber(0, c.getSize() - 1)]); }
				break;
			case 1: // Удаление с уничтожением
				//if (c2.getSize() > 0) { c.deleteObj(c[randomNumber(0, c.getSize() - 1)]); }
				break;
			break;
			}
		case 2: // Вызов метода printinf у случайного объекта
			//c2[randomNumber(0, c.getSize() - 1)]->printInf();
			break;
		}
	}
	*/
	int end_time = clock();
	printf("Work time: %d mls\n", end_time - start_time);
	printf("Size = %d\nCapacity = %d\n", c2.getSize(), c2.getCapacity());
	printf("End");
	return 0;
}