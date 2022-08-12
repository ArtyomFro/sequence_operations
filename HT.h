#pragma once
#pragma once
#include <iterator>
using namespace std;

struct Node {	//������� ��
	int key;	//���
	Node* down;	//������ ����
	Node() : down(nullptr) {}
	Node(int k, Node* d = nullptr) : key(k), down(d) {}
	~Node() { delete down; } //- ���������� ���� - ������ �� ���������!!!
};
//�������� ������
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
//�������� �������
template <typename Container, typename Iter = myiter>
class outiter : public std::iterator<std::output_iterator_tag, typename Container::value_type>
{
protected:
	Container& container;    // ��������� ��� ������� ���������
	Iter iter;	// ������� �������� ���������
public:
	// �����������
	explicit outiter(Container& c, Iter it) : container(c), iter(it) { }

	// ������������ - ������� ����� � ���������
	const outiter<Container>&
		operator = (const typename Container::value_type& value) {
		iter = container.insert(value, iter).first;
		return *this;
	}
	const outiter<Container>& //������������ ����� - ���������
		operator = (const outiter<Container>&) { return *this; }
	// ������������� - ������ ��������
	outiter<Container>& operator* () { return *this; }
	// ��������� - ������ ��������
	outiter<Container>& operator++ () { return *this; }
	outiter<Container>& operator++ (int) { return *this; }
};

// ������� ��� �������� ��������� ������� - �������� ��� ������ ���������
template <typename Container, typename Iter>
inline outiter<Container, Iter> outinserter(Container& c, Iter it)
{
	return outiter<Container, Iter>(c, it);
}
//myiter myiter0(nullptr);


