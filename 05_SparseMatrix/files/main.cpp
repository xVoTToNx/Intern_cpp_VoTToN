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
		Elem<TValue>* elem = new Elem<TValue>(value, row, column);
		(*elem).Next = first;
		first = elem;
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
		auto current_elem = begin();
		auto last_elem = nullptr;
		while (current_elem != end()) {
			if ((*current_elem).GetRow() == row && (*current_elem).GetColumn() == column) {
				if (last_elem == nullptr) {
					delete first;
					first = (*current_elem).next;
					return true;
				}
				else {
					(*current_elem).next = (*current_elem).next;
					delete current_elem;
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
	lol.Insert(55, 1, 2);
	lol.Insert(33, 2, 7);
	lol.Insert(9, 4, 3);
	std::cout << lol << std::endl;
	lol.Remove(9);
	std::cout << lol << std::endl;
	auto jesus = std::find(lol.begin(), lol.end(), Elem<int>(33, 2, 7));
	std::cout << (*jesus).Value << std::endl;
	return 0;
}