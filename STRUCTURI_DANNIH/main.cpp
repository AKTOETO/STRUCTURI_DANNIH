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

// если нужна печать по каждому действию
#define NEED_PRINT_DEBUG 0

// вывод в консоль сообщения
#define INFO(str) if(NEED_PRINT_DEBUG)cout<<"\t"<<str<<"\n";


// элемент дерева
template<class T>
struct Node
{
	T m_data;			// ключ
	Node* m_left;		// указатель на левого потомка
	Node* m_right;		// указатель на правого потомка

	// конструкторы
	Node() :m_left(nullptr), m_right(nullptr), m_data(NULL)
	{
		INFO("NODE: элемент создан");
	};

	Node(T _data)
		:m_left(nullptr), m_right(nullptr), m_data(_data)
	{
		INFO("NODE: элемент создан");
	};

	Node(T _data, Node* _left, Node* _right)
		:m_left(_left), m_right(_right), m_data(_data)
	{
		INFO("NODE: элемент создан");
	};

	// деструктор
	~Node()
	{
		m_data = T(NULL);
		m_left = nullptr;
		m_right = nullptr;
		INFO("NODE: элемент удален");
	}
};

// вставка элемента в дерево
template<class T>
Node<T>* tree_insert_node(Node<T>* _root, T _key)
{
	// если текущий элемент пуст
	if (!_root)
	{
		// создаем новый элемент
		Node<T>* new_node = new Node<T>(_key);

		INFO("NODE_INSERT: элемент вставлен");

		// возвращаем его
		return new_node;
	}
	// если ключ меньше ключа текущего элемента
	if (_key < _root->m_data)
	{
		// идем влево
		_root->m_left = tree_insert_node(_root->m_left, _key);
	}
	// если ключ больше ключа текущего элемента
	else if (_key > _root->m_data)
	{
		// идем вправо
		_root->m_right = tree_insert_node(_root->m_right, _key);
	}

	// возвращаем корень дерева
	return _root;
}

// создание дерева из массива
template<class T>
Node<T>* tree_create(T* arr, int size)
{
	// объявление переменной списка
	Node<T>* root = nullptr;

	// заполнение дерева значениями
	for (int i = 0; i < size; i++)
	{
		root = tree_insert_node(root, arr[i]);
	}

	INFO("CREATE_TREE: дерево создано");

	// возвращение созданного списка
	return root;
}

// печать элемента дерева
template<class T>
void node_print(Node<T>* _node)
{
	cout << _node->m_data << " ";
}

// удаление элемента дерева
template<class T>
void node_delete(Node<T>* _node)
{
	delete _node;
}

// обход дерева симметричный
template<class T>
void inorder(Node<T>* _root, void (*_func)(Node<T>*)) {
	if (_root)
	{
		// идем налево
		inorder(_root->m_left, _func);

		// обрабатываем элемент
		_func(_root);

		// идем направо
		inorder(_root->m_right, _func);
	}
}

// обход дерева прямой
template<class T>
void preorder(Node<T>* _root, void (*_func)(Node<T>*))
{
	if (_root)
	{
		// обрабатываем элемент
		_func(_root);

		// идем налево
		preorder(_root->m_left, _func);

		// идем направо
		preorder(_root->m_right, _func);
	}
}

// обход дерева обратный
template<class T>
void postorder(Node<T>* _root, void (*_func)(Node<T>*))
{
	if (_root)
	{
		// идем налево
		postorder(_root->m_left, _func);

		// идем направо
		postorder(_root->m_right, _func);

		// обрабатываем элемент
		_func(_root);
	}
}

// удаление дерева
template<class T>
void tree_delete(Node<T>*& _root)
{
	// если дерево существует
	if (_root)
	{
		// удаляем его
		postorder(_root, node_delete/*[](Node<T>* _node) { delete _node; }*/);
		_root = nullptr;
		INFO("TREE_DELETE: дерево удалено");
	}
	else
	{
		INFO("TREE_DELETE: дерево не существует");
	}
}

// поиск узла
template<class T>
Node<T>* tree_find_node(Node<T>* _root, T _key)
{
	// если мы не нашли элемент 
	// возвращаем nullptr
	if (!_root)
	{
		return nullptr;
	}
	// если мы нашли элемент, возвращаем _root
	else if (_key == _root->m_data)
	{
		return _root;
	}
	// если элемент для поиска меньше текущего
	else if (_key < _root->m_data)
	{
		// переходим к левой ветви
		return tree_find_node(_root->m_left, _key);
	}
	// если элемент для поиска больше текущего
	else
	{
		// переходим к правой ветви
		return tree_find_node(_root->m_right, _key);
	}
}

// нахождение высоты дерева
template<class T>
int tree_count_height(Node<T>* _root)
{
	// если элемент не существует
	if (!_root)
		return 0;

	// если это самый низкий элемент в данной ветке
	if (!_root->m_left && !_root->m_right)
		return 1;

	// возвращаем максимальное значение из двух ветвей + 1
	return max(
		tree_count_height(_root->m_left),
		tree_count_height(_root->m_right)
	) + 1;
}

// нахождение количества узлов
template<class T>
int tree_count_nodes(Node<T>* _root)
{
	// если элемент не существует
	if (!_root)
		return 0;

	// если это самый низкий элемент в данной ветке
	if (!_root->m_left && !_root->m_right)
		return 1;

	// возвращаем сумму значений из двух ветвей + 1
	return (
		tree_count_nodes(_root->m_left) +
		tree_count_nodes(_root->m_right)
		) + 1;
}

// нахождение минимального элемента в дереве
template<class T>
Node<T>* tree_find_min(Node<T>* _root)
{
	Node<T>* current = _root;

	// идем по левой ветке, пока не дойдем до конца
	while (current && current->m_left)
	{
		current = current->m_left;
	}

	return current;
}

// нахождение максимального элемента в дереве
template<class T>
Node<T>* tree_find_max(Node<T>* _root)
{
	Node<T>* current = _root;

	// идем по правой ветке, пока не дойдем до конца
	while (current && current->m_right)
	{
		current = current->m_right;
	}

	return current;
}

// удаление элемента дерева
template<class T>
Node<T>* tree_node_delete(Node<T>* _root, T _key)
{
	// ищем узел 
	Node<T>* to_find = tree_find_node(_root, _key);
	Node<T>* temp;

	// если элемент не был найден
	if (!to_find) return to_find;

	// если у удаляемого элемента нет дочерних
	// элементов или один дочерний элемент
	if (!to_find->m_left)
	{
		// создание временного элемента
		temp = to_find->m_right;
		delete to_find;
		//return temp;
	}
	else if (!to_find->m_right)
	{
		// создание временного элемента
		temp = to_find->m_left;
		delete to_find;
		//return temp;
	}
	else
	{
		// если у удаляемого элемента есть
		// оба дочерних элемента
		temp = tree_find_min(_root->m_right);
	}

	// помещаем найденный элемент temp в корень дерева
	_root->m_data = temp->m_data;

	// удаляем из дерева элемент
	_root->m_right = tree_node_delete(_root->m_right, temp->m_data);

	return _root;
}

int main()
{
	setlocale(LC_ALL, "ru");

	// ДЛЯ ТЕСТОВ

	//char* mass = new char [5] { 'a', 'c', 'B', 'd', 'A' };
	//int* mass = new int [5] { 50, 30, 20, 40, 60 };
	//int* mass = new int [10] { 50, 30, 20, 40, 60, 45, 55, 32, 12, 98 };
	int* mass = new int [6] { 80,52,48,71,63,67 };
	//int* mass = new int [9] { 20, 10, 35, 15, 17, 27, 24, 8, 30 };
	//int* mass = new int [5] { 50, 30, 50, 40, 600 };

	Node<int>* root = tree_create(mass, 6);

	cout << "inorder: ";
	inorder(root, node_print);
	cout << endl;
	cout << endl;

	cout << "preorder: ";
	preorder(root, node_print);
	cout << endl;
	cout << endl;

	cout << "postorder: ";
	postorder(root, node_print);
	cout << endl;
	cout << endl;

	Node<int>* to_found = tree_find_node(root, 35);
	if (to_found)
	{
		cout << "найденный элемент: " << to_found->m_data << endl;
	}

	cout << "количество узлов дерева: " << tree_count_nodes(root) << endl;

	cout << "высота дерева: " << tree_count_height(root) << endl;

	cout << "число узлов в левом поддереве: " << tree_count_nodes(root->m_left) << endl;
	cout << "число узлов в правом поддереве: " << tree_count_nodes(root->m_right) << endl;

	cout << "минимальный элемент: " << tree_find_min(root)->m_data << endl;
	cout << "максимальный элемент: " << tree_find_max(root)->m_data << endl;

	root = tree_node_delete(root, 71);

	cout << "inorder: ";
	inorder(root, node_print);
	cout << endl;
	cout << endl;

	tree_delete(root);

	return 0;
}