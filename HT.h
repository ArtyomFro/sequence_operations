#pragma once
#pragma once
#include <iterator>
using namespace std;

struct Node {	//Элемент ХТ
	int key;	//Вес
	Node* down;	//Ссылка вниз
	Node() : down(nullptr) {}
	Node(int k, Node* d = nullptr) : key(k), down(d) {}
	~Node() { delete down; } //- Деструктор узла - только по умолчанию!!!
};
//ИТЕРАТОР ЧТЕНИЯ
//template<class Container = HT>
struct myiter : public std::iterator<std::forward_iterator_tag, int>
{
	myiter(Node* p) : bct(nullptr), pos(0), Ptr(p) {}
	bool operator == (const myiter& Other) const { return Ptr == Other.Ptr; }
	bool operator != (const myiter& Other) const { return Ptr != Other.Ptr; }
	myiter operator++();
	myiter operator++(int) { myiter temp(*this); ++* this; return temp; }
	pointer operator->() { return &Ptr->key; }
	reference operator*() { return Ptr->key; }
	//protected:
//	Container& c; 
	Node** bct;
	size_t pos;
	Node* Ptr;
};
//ИТЕРАТОР ВСТАВКИ
template <typename Container, typename Iter = myiter>
class outiter : public std::iterator<std::output_iterator_tag, typename Container::value_type>
{
protected:
	Container& container;    // Контейнер для вставки элементов
	Iter iter;	// Текущее значение итератора
public:
	// Конструктор
	explicit outiter(Container& c, Iter it) : container(c), iter(it) { }

	// Присваивание - вставка ключа в контейнер
	const outiter<Container>&
		operator = (const typename Container::value_type& value) {
		iter = container.insert(value, iter).first;
		return *this;
	}
	const outiter<Container>& //Присваивание копии - фиктивное
		operator = (const outiter<Container>&) { return *this; }
	// Разыменование - пустая операция
	outiter<Container>& operator* () { return *this; }
	// Инкремент - пустая операция
	outiter<Container>& operator++ () { return *this; }
	outiter<Container>& operator++ (int) { return *this; }
};

// Функция для создания итератора вставки - аргумент при вызове алгоритма
template <typename Container, typename Iter>
inline outiter<Container, Iter> outinserter(Container& c, Iter it)
{
	return outiter<Container, Iter>(c, it);
}
//myiter myiter0(nullptr);


