#include <iostream>     
#include <vector>  
#include <iterator>   

template<typename TValue>
class Elem {
	const unsigned int row;
	const unsigned int column;
public:
	TValue Value;
	Elem* Next;

	Elem(TValue const& _value,
		unsigned int const&_row, unsigned int const& _column, Elem* _next = nullptr)
		: Value(_value)
		, row(_row)
		, column(_column)
		, Next(_next)
	{}

	template<typename AnotherTValue>
	bool operator == (Elem<AnotherTValue> aTv) const;

	bool operator == (TValue value) const;

	unsigned int GetRow();

	unsigned int GetColumn();

};

template<class Type>
class spdok_iterator
{
	Elem<Type>* iterator;

public:

	spdok_iterator(Elem<Type>* it); 
	spdok_iterator(spdok_iterator const& it); 
	spdok_iterator();

	spdok_iterator& operator++ ();
	spdok_iterator operator++ (int);

	template<class AnotherType>
	bool operator == (spdok_iterator<AnotherType> const& aIt) const;
	template<class AnotherType>
	bool operator != (spdok_iterator<AnotherType> const& aIt) const;

	Elem<Type>& operator* ();
	Elem<Type>& operator-> ();

	Elem<Type>* GetElem() {
		return iterator;
	}
};

template<typename TValue>
class sparse_matrix_dok {

	Elem<TValue>* first;
	int count;
	const int maxRow;
	const int maxColumn;

public:

	sparse_matrix_dok(int const& _nSize, int const& _mSize);
	sparse_matrix_dok(std::pair<int const&, int const&> size);
	~sparse_matrix_dok() {};

	TValue operator() (int, int);

	std::pair<int, int> Size();
	unsigned int Count();

	spdok_iterator<TValue> begin();
	spdok_iterator<TValue> end();

	spdok_iterator<const TValue> cbegin();
	spdok_iterator<const TValue> cend();

	void Insert(TValue const& value,
		unsigned int const& row, unsigned int const& column);
	bool Remove(TValue const& value);
	bool Remove(int const& row, int const& column);

	spdok_iterator<TValue> At(TValue const& value) const;
};

template<typename T>
std::ostream& operator<< (std::ostream& s, sparse_matrix_dok<T> m);