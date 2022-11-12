/***********************************************************************\
*                    кафедра № 304 2 курс 3 семестр информатика			*
*-----------------------------------------------------------------------*
*	Project type : solution												*
*	Project name : LW4													*
*	File name    : main.cpp												*
*	Language     : c/c++												*
*	Programmers  : Плоцкий Б.А. Раужев Ю. М.							*
*	Created      :  12/11/22											*
*	Last revision:  --/11/22											*
*	Comment(s)   : 														*
*																		*
*	Лабораторная работа «Бинарные деревья поиска»						*
*	Задание																*
*	1.	Реализовать функции вставки, поиска, удаления узла,				*
*	обхода дерева, вывода дерева на экран, нахождения высоты			*
*	дерева и количества узлов.											*
*	2.	Реализовать дополнительно функцию в соответствии с вариантом:	*
*	T – тип ключей, D – диапазон изменения значений ключей.				*
\***********************************************************************/
#include <iostream>

using namespace std;

// элемент дерева
template<class T>
struct Node
{
	T m_data;			// ключ
	Node* m_left;		// указатель на левого потомка
	Node* m_right;		// указатель на правого потомка

	// конструкторы
	Node() :m_left(nullptr), m_right(nullptr) {};

	Node(T _data)
		:m_left(nullptr), m_right(nullptr), m_data(_data) {};

	Node(T _data, Node* _left, Node* _right)
		:m_left(_left), m_right(_right), m_data(_data) {};
};

// вставка элемента в дерево
template<class T>
Node<T>* tree_insert(Node<T>* _root, T _key)
{
	if (!_root)
	{
		return new Node<T>(_key);
	}
	if (_key < _root->m_data)
	{
		_root->m_left = tree_insert(_root->m_left, _key);
	}
	else if (_key > _root->m_data)
	{
		_root->m_right = tree_insert(_root->m_right, _key);
	}
	return _root;
}

int main()
{
	setlocale(LC_ALL, "ru");

	Node<int>* root = nullptr;

	root = tree_insert(root, 20);
	root = tree_insert(root, 10);
	root = tree_insert(root, 35);
	root = tree_insert(root, 15);
	root = tree_insert(root, 17);
	root = tree_insert(root, 27);
	root = tree_insert(root, 8);
	root = tree_insert(root, 30);

	return 0;
}