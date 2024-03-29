#include "sparse_matrix_dok.h"

template<typename T>
std::ostream& operator<< (std::ostream& s, sparse_matrix_dok<T> m) {
	for (spdok_iterator<T> i = m.begin(); i != m.end(); ++i) {
		s << "[ " << (*i).GetRow() << ", " << (*i).GetColumn() << " ] -> ( " << (*i).Value << " )" << std::endl;
	}
	return s;
}

#pragma region Iterator

template<typename Type>
spdok_iterator<Type>::spdok_iterator(Elem<Type>* it)
	: iterator(it)
{	}

template<typename Type>
spdok_iterator<Type>::spdok_iterator(spdok_iterator const& it)
	: iterator(it.iterator)
{	}

template<typename Type>
spdok_iterator<Type>::spdok_iterator()
	: iterator(nullptr)
{	}

template<typename Type>
spdok_iterator<Type>& spdok_iterator<Type>::operator++ () {
	if (iterator == nullptr) {
		throw std::invalid_argument("");
	}

	iterator = iterator->Next;
	return *this;
}

template<typename Type>
spdok_iterator<Type> spdok_iterator<Type>::operator++ (int) {
	if (iterator == nullptr) {
		throw std::invalid_argument("");
	}

	spdok_iterator temp(*this);
	iterator = iterator->Next;
	return temp;
}

template<class Type>
template<class AnotherType>
bool spdok_iterator<Type>::operator == (spdok_iterator<AnotherType> const& aIt) const {
	return iterator == aIt.iterator;
}

template<class Type>
template<class AnotherType>
bool spdok_iterator<Type>::operator != (spdok_iterator<AnotherType> const& aIt) const {
	return iterator != aIt.iterator;
}

template<class Type>
Elem<Type>& spdok_iterator<Type>::operator* () {
	if (iterator == nullptr) {
		throw std::invalid_argument("");
	}
	return *iterator;
}

template<class Type>
Elem<Type>& spdok_iterator<Type>::operator-> () {
	if (iterator == nullptr) {
		throw std::invalid_argument("");
	}
	return iterator;
}

#pragma endregion

#pragma region Matrix

	template<typename TValue>
	sparse_matrix_dok<TValue>::sparse_matrix_dok(int const& _nSize, int const& _mSize)
		: maxRow(_nSize)
		, maxColumn(_mSize)
		, first(nullptr)
		, count(0)
	{}

	template<typename TValue>
	sparse_matrix_dok<TValue>::sparse_matrix_dok(std::pair<int const&, int const&> size)
		: maxRow(size.first)
		, maxColumn(size.second)
		, first(nullptr)
		, count(0)
	{}

	template<typename TValue>
	TValue sparse_matrix_dok<TValue>::operator()(int row, int column) {

		if (row >= maxRow || column >= maxColumn)
			throw std::out_of_range("");
		spdok_iterator<TValue> current_elem = begin();
		while (current_elem != spdok_iterator<TValue>(nullptr)) {
			if ((*current_elem).GetRow() == row &&
				(*current_elem).GetColumn() == column) {
				return (*current_elem).Value;
			}
			current_elem++;
		}
		TValue var = TValue();
		return var;
	}

	template<typename TValue>
	std::pair<int, int> sparse_matrix_dok<TValue>::Size() {

		return std::pair<int, int>(maxRow, maxColumn);
	}

	template<typename TValue>
	unsigned int sparse_matrix_dok<TValue>::Count() {

		return count;
	}

	template<typename TValue>
	spdok_iterator<TValue> sparse_matrix_dok<TValue>::begin() { return spdok_iterator<TValue>(first); }

	template<typename TValue>
	spdok_iterator<TValue> sparse_matrix_dok<TValue>::end() { return spdok_iterator<TValue>(nullptr); }

	template<typename TValue>
	spdok_iterator<const TValue> sparse_matrix_dok<TValue>::cbegin() { return spdok_iterator<const TValue>(first); }

	template<typename TValue>
	spdok_iterator<const TValue> sparse_matrix_dok<TValue>::cend() { return spdok_iterator<const TValue>(nullptr); }

	template<typename TValue>
	void sparse_matrix_dok<TValue>::Insert(TValue const& value,
		unsigned int const& row, unsigned int const& column) {

		if (row >= maxRow || column >= maxColumn) {
			throw std::out_of_range("");
		}

		if (first == nullptr) {
			first = new Elem<TValue>(value, row, column);
			return;
		}
		else {
			spdok_iterator<TValue> current_elem = begin();
			spdok_iterator<TValue> last_elem = end();
			while (true) {
				if (current_elem == end()) {

					(*last_elem).Next = new Elem<TValue>(value,
						row, column);
					return;
				}
				else if ((*current_elem).GetRow() > row) {

						if (last_elem == end()) {
							first = new Elem<TValue>(value, row, column,
								current_elem.GetElem());
							return;
						}
						(*last_elem).Next = new Elem<TValue>(value, 
							row, column, current_elem.GetElem());
						return;
				}
				else if ((*current_elem).GetRow() == row) {
						if (current_elem == end()) {

							(*last_elem).Next = new Elem<TValue>(value,
								row, column);
							return;
						}
						else if ((*current_elem).GetRow() > row) {
							(*last_elem).Next = new Elem<TValue>(value,
								row, column, current_elem.GetElem());
							return;
						}
						else if ((*current_elem).GetColumn() == column) {
							(*current_elem).Value = value;
							return;
						}
						else if ((*current_elem).GetColumn() > column) {
							if (last_elem == end()) {
								first = new Elem<TValue>(value, row, column,
									current_elem.GetElem());
								return;
							}
							(*last_elem).Next = new Elem<TValue>(value,
								row, column, current_elem.GetElem());
							return;
						}

					
				}

				last_elem = current_elem;
				current_elem++;
			}
		}
	}

	template<typename TValue>
	bool sparse_matrix_dok<TValue>::Remove(TValue const& value) {

		spdok_iterator<TValue> current_elem = begin();
		spdok_iterator<TValue> last_elem = end();
		while (current_elem != end()) {
			if ((*current_elem).Value == value) {
				if (last_elem == end()) {
					first = (*current_elem).Next;
					delete  &(*current_elem);
					return true;
				}
				else {
					(*last_elem).Next = (*current_elem).Next;
					delete &(*current_elem);
					return true;
				}
			}

			last_elem = current_elem;
			current_elem++;
		}
		return false;
	}

	template<typename TValue>
	bool sparse_matrix_dok<TValue>::Remove(int const& row, int const& column) {

		spdok_iterator<TValue> current_elem = begin();
		spdok_iterator<TValue> last_elem = end();
		while (current_elem != end()) {
			if ((*current_elem).GetRow() == row && (*current_elem).GetColumn() == column) {
				if (last_elem == end()) {
					first = (*current_elem).Next;
					delete  &(*current_elem);
					return true;
				}
				else {
					(*last_elem).Next = (*current_elem).Next;
					delete &(*current_elem);
					return true;
				}
			}

			last_elem = current_elem;
			current_elem++;
		}
		return false;
	}

	template<typename TValue>
	spdok_iterator<TValue> sparse_matrix_dok<TValue>::At(TValue const& value) const {

		spdok_iterator<TValue> current_elem(first);
		while (current_elem != spdok_iterator<TValue>(nullptr)) {
			if ((*current_elem).Value == value) {
				return current_elem;
			}
			current_elem++;
		}
		return nullptr;
	}

#pragma endregion

#pragma region Elem

	template<typename TValue>
	template<typename AnotherTValue>
	bool Elem<TValue>::operator == (Elem<AnotherTValue> aTv) const {
		return row == aTv.GetRow() && column == aTv.GetColumn() && Value == aTv.Value;
	}

	template<typename TValue>
	bool Elem<TValue>::operator == (TValue value) const {
		return Value == value;
	}

	template<typename TValue>
	unsigned int Elem<TValue>::GetRow() {
		return row;
	}

	template<typename TValue>
	unsigned int Elem<TValue>::GetColumn() {
		return column;
	}

#pragma endregion

int main()
{
	sparse_matrix_dok<int> lol(10, 10);
	std::cout << lol << std::endl;
	lol.Insert(55, 1, 2); //1
	std::cout << "1" << std::endl;
	lol.Insert(33, 2, 7);
	std::cout << "2" << std::endl;
	lol.Insert(44, 2, 7); //3
	std::cout << "3" << std::endl;
	lol.Insert(33, 2, 9); //4
	std::cout << "4" << std::endl;
	lol.Insert(33, 2, 5); //2
	lol.Insert(33, 3, 7); //5
	lol.Insert(33, 4, 7); //6
	lol.Insert(54, 2, 2); //3
	lol.Insert(44, 0, 2); //3
	lol.Insert(44, 0, 0); //3
	std::cout << lol << std::endl;
	return 0;
}