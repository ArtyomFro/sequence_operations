#include "HT.h"
#include <iostream>
#include <iomanip>
#include <iostream>
#include <clocale>
#include <vector>
#include <list>
#include <stack>
#include <iterator>
#include <algorithm>
#include <unordered_set>
#include <chrono>
#include <ctime>
using std::cout;
using namespace std;
using MySet = std::unordered_set<int>;
using MyIt = std::unordered_set<int>::iterator;
using MySeq = std::vector<MyIt>;
const int lim = 100;
class HT {	//Хеш-таблица
	static  size_t tags;
	char tag;
	int power;
	MySet A;
	MySeq sA;
	Node** bucket;
	size_t count = 0;
	static myiter myiter0;
public: //Необходимые элементы контейнера
	static const size_t Buckets = 26;
	using key_type = int;
	using value_type = int;
	using key_compare = equal_to<int>;
	void swap(HT& rgt)
	{
		std::swap(tag, rgt.tag);
		std::swap(bucket, rgt.bucket);
		std::swap(count, rgt.count);
	}
	int hash(int k)const { return (k * (Buckets - 1) + 7) % Buckets; }
	size_t bucket_count() { return Buckets; }
	myiter Insert(const int& k,myiter where) {
		return insert(k, where).first;
	}
	void Display();
	myiter begin()const;
	myiter end()const { return myiter(nullptr); }
	//	const myiter cbegin() const { return begin(); }
	//	const myiter cend()const { return cend(); }
	pair<myiter, bool> insert(int, myiter = myiter(nullptr));
	pair<myiter, bool> Remove(int);
	HT() : tag('A' + tags++), bucket(new Node* [Buckets]) {
		for (int i = 0; i < Buckets; ++i) bucket[i] = nullptr;
	}
	//	HT(size_t Buckets);
	//	void resize(size_t Buckets);
	int size() { return count; }
	template<typename MyIt>
	HT(MyIt, MyIt);
	~HT();
	myiter find(int)const;
	HT(const HT& rgt) : HT() {
		for (auto x = rgt.begin(); x != rgt.end(); ++x) insert(*x);
	}
	HT(HT&& rgt) : HT() {
		swap(rgt);
	}
	HT& operator=(const HT& rgt)
	{
		HT temp;
		for (auto x = rgt.begin(); x != rgt.end(); ++x) insert(*x);
		swap(temp);
		return *this;
	}
	HT& operator=(HT&& rgt)
	{
		swap(rgt);
		return *this;
	}
	HT& operator |= (const HT&);
	HT operator | (const HT& rgt) const
	{
		HT result(*this); return (result |= rgt);
	}
	HT& operator &= (const HT&);
	HT operator & (const HT& rgt) const
	{
		HT result(*this); return (result &= rgt);
	}
	HT& operator -= (const HT&);
	HT operator - (const HT& rgt) const
	{
		HT result(*this); return (result -= rgt);
	}
	HT& operator / (const HT& m1);
	
	void convert(const HT& rgt);
	void Merge(const HT& ,const HT&);
	void Erase(int, int);
	void Excl(const HT&);
	void Show() const;
	size_t Power() const { return sA.size(); }
	void PrepareExcl(const HT&); //Подготовка excl
	//подготовка and и sub
	friend HT XOR(const HT& m1, const HT& m2)
	{
		return ((m1 | m2) - (m1 & m2));
	}
};

myiter HT::begin()const {
	myiter p(nullptr);
	p.bct = bucket;
	for (; p.pos < Buckets; ++p.pos) {
		p.Ptr = bucket[p.pos];
		if (p.Ptr) break;
	}
	return p;
}
myiter myiter::operator++()
{
	if (!Ptr) { //Первое обращение?
		return *this; //Не работает без предварительной установки на ХТ
	}
	else
	{ //Текущий уже выдан
		if (Ptr->down) { //Есть следующий, вниз
			Ptr = Ptr->down;
			return (*this);
		}
		while (++pos < HT::Buckets) {	//Поиск очередного элемента
			if (Ptr = bct[pos])
				return *this;
		}
		Ptr = nullptr;
		return (*this);
	}
}


HT :: ~HT()
{
	for (auto t = 0; t < Buckets; ++t)
		delete bucket[t];
	delete[]bucket;
}

void HT::Display()	//Выдача хеш-таблицы на экран
{
	Node** P = new Node * [Buckets];
	for (auto t = 0; t < Buckets; ++t) P[t] = bucket[t];
	bool prod = true;
	cout << "\n" << tag << ":";
	while (prod) {
		prod = false;
		for (auto t = 0; t < Buckets; ++t) {
			if (P[t]) {
				cout << setw(4) << P[t]->key;
				P[t] = P[t]->down;
				prod = true;
			}
			else cout << "  - ";
		}
		cout << "\n  ";
	}
}

myiter HT::find(int k)const
{
	auto t(hash(k));
	Node* p(bucket[t]);
	while (p) {
		if (p->key == k)
			return myiter(p);
		else p = p->down;
	}
	return end();
}
pair<myiter, bool> HT::insert(int k, myiter)	//Вставка нового значения в хеш-таблицу
{
	auto t(hash(k));
	Node* p(bucket[t]);
	while (p) {
		if (p->key == k)
			return make_pair(myiter(p), false);
		else p = p->down;
	}
	bucket[t] = new Node(k, bucket[t]);
	++count;
	return make_pair(myiter(bucket[t]), true);
}

pair<myiter, bool> HT::Remove(int k)	//Удаление (единственного) элемента из хеш-таблицы
{
	auto t(hash(k));
	auto p(bucket[t]), q(p);
	if (p) {
		if (p->key == k) { //Удаление первого или единственого
			bucket[t] = p->down;
			p->down = nullptr;
			delete p;
			return make_pair(myiter(q), true);
		}
		p = p->down;
		while (p) {
			if (p->key == k) {
				q->down = p->down;
				p->down = nullptr;
				delete p;
				--count;
				return make_pair(myiter(q), true);
			}
			else q = p, p = p->down;
		}
	}
	return make_pair(myiter(q), false);
}

template<class MyIt> //Вставка из интервала
HT::HT(MyIt first, MyIt last) : HT() {
	for (; first != last; ++first) insert(*first);
}

HT& HT::operator |= (const HT& rgt) {
	for (myiter x = rgt.begin(); x != rgt.end(); ++x)
		insert(*x, nullptr);
	return *this;
}

HT& HT::operator &= (const HT& rgt) {
	HT temp;
	for (auto x = begin(); x != end(); ++x)
		if (rgt.find(*x) != rgt.end())
			temp.insert(*x);
	swap(temp);
	return *this;
}

HT& HT::operator / (const HT& m1) {
	HT temp;
	for (auto x = begin(); x != end(); ++x)
	{
		bool l = true;
		for (myiter y = m1.begin(); y != m1.end(); ++y)
		{
			if (*x == *y)
			{
				l = false;
			}
		}
		if (l)
		{
			temp.insert(*x);
		}
	}
	swap(temp);
	return *this;
}

HT& HT::operator -= (const HT& rgt) {
	HT temp;
	for (auto x = begin(); x != end(); ++x)
		if (rgt.find(*x) == rgt.end()) temp.insert(*x);
	swap(temp);
	return *this;
}
size_t HT::tags = 0;


void HT::convert(const HT& rgt)
{
	HT temp;
	auto x = rgt.begin();
	for (; x != end(); ++x)
	{
		temp.sA.push_back(temp.A.insert(*x).first);
	}

	for (int x=0; x < temp.sA.size(); ++x)
	{
		int y = *(temp.sA[x]);
		sA.push_back(A.insert(y).first);
	}
	
}

void HT::Erase(int k1, int k2) { //Исключение фр-та от p до q
	HT temp;
	auto x = begin();
	for (; x.pos < k1; ++x)
	{
		temp.insert(*x);
	}
	for (; x.pos < k2; ++x)
	{

	}
	for (;  x != end(); ++x)
	{
		temp.insert(*x);
	}
	swap(temp);
	}



void HT::Merge(const HT& lft,const HT& rgt) { //Слияние
	HT fin;
	for (myiter x =lft.begin(); x != lft.end(); ++x)
	{
		fin.insert(*x);
	}
	for (myiter y = rgt.begin(); y != rgt.end(); ++y)
	{
		fin.insert(*y);
	}
	
	swap(fin);
}

void HT::PrepareExcl(const HT& rgt) {
	//Подготовка объекта исключения в пустом контейнере...
	int a = 1+rand() % 13, b = rand() % 14+12;
	//... из случайного [a, b] отрезка rgt
	HT temp;
	auto x = rgt.begin();
	for (; x.pos < a; ++x)
	{

	}
	for (; x.pos < b; ++x) {

		temp.sA.push_back(temp.A.insert(*x).first);
	}
	for (int x = 0; x < temp.sA.size(); ++x) {
		int y =*(temp.sA[x]); 
		sA.push_back(A.insert(y).first); 
	}
	
}

void HT::Excl(const HT& rgt)
{ //Исключение подпоследовательности
	size_t n(Power()), m(rgt.Power());
	if (m) for (size_t p = 0; p < n; ++p) { //Поиск первого элемента 
		bool f(true);
		//		int a(*sA[p]), b(*rgt.sA[0]); //ОТЛАДКА
		if (*sA[p] == *rgt.sA[0]) { //Проверка всей цепочки
			size_t q(p), r(0);
			if (m > 1) do {
				++q, ++r;
				size_t c(*sA[q]), d(*rgt.sA[r]);
				f &= c == d;
			} while ((r < m - 1) && f);
			if (f) {//Цепочки совпали, удаляем
				MySet temp;
				MySeq stemp;
				for (size_t i = 0; i < p; ++i)
					stemp.push_back(temp.insert(*sA[i]).first);
				for (size_t i = p + m; i < Power(); ++i)
					stemp.push_back(temp.insert(*sA[i]).first);
				A.swap(temp);
				sA.swap(stemp);
				break;
			}
		}
	}
}

void HT::Show() const {
	using std::cout;
	cout << "\n" << tag << ": ";
	/*	unsigned n = A.bucket_count( ); //Вариант: выдача для ХТ
		for (auto i = 0; i < n; ++i)
			if (A.bucket_size(i))
			{
					cout << "\n" <<  i << "("<< A.bucket_size(i) << "):" ;
		//	        auto it0 = A.begin(i), it1 = A.end(i);
					for (auto it = A.begin(i); it != A.end(i); ++it) cout << " " << *it;
		} */
	for (auto x : A) cout << x << " "; //Выдача множества
	cout << "\n < ";
	for (auto x : sA) cout << *x << " "; //Выдача последовательности
	cout << ">";
}


int main()
{
	srand((unsigned int)time(nullptr));
	int k = 0, l = 0, N = 0;
	int K1,K2,K3,K4,K5;
	//HT home(K.begin(), K.begin() + 10), second; //ХТ ДЛЯ УПРАЖНЕНИЙ
	HT home, second, third, fourth, fith;
	for (auto i = 0; i < 26; ++i)
	{
		K1 = 1 + rand() % 100;
		home.insert(K1);

		K2 = 1 + rand() % 100;
		second.insert(K2);

		K3 = 1 + rand() % 100;
		third.insert(K3);

		K4 = 1 + rand() % 100;
		fourth.insert(K4);

		K5 = 1 + rand() % 100;
		fith.insert(K5);
	}
	Node* current = 0;
	cout << "\n=== DATA ===";
	home.Display();
	second.Display();
	third.Display();
	fourth.Display();
	fith.Display();

	std::cin.get();
	using std::cout;
	using namespace std::chrono;
	setlocale(LC_ALL, "Russian");

	auto t1 = std::chrono::high_resolution_clock::now();
	cout << "\n=== Calculations ===";
	HT F(second & third);
	F.Display();
	HT G(F | fourth);
	G.Display();
	HT H(XOR(G, fith));
	H.Display();
	HT K(home / H);
	K.Display();
	int a = 1+rand()%13, b = rand()%13+13;
    cout << "\n=== R.Erase (" << a << "," << b << ")===";
	home.Display();
	if (a > b) cout << "(skipped!)";
	home.Erase(a, b);  
	home.Display();

	cout << "\n=== R.Merge(E) ===";
	home.Display();
	second.Display();
	home.Merge(home,second); 
	home.Display();

    cout << "\n=== R.Excl(F) ===";
	home.convert(home);
	third.PrepareExcl(home);
	third.Show();
	cout << "\n";
	home.Show();
	home.Excl(third);
	cout << "\n";
	home.Show();

	cin.get();
	return 0;
	}