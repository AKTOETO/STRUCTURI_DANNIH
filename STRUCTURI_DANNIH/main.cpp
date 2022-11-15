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
#include <iomanip>

using namespace std;

// если нужна печать по каждому действию
#define NEED_PRINT_DEBUG 1

// вывод в консоль сообщения
#define INFO(str) if(NEED_PRINT_DEBUG)cout<<"\t"<<str<<"\n";

// множитель ширины вывода дерева
#define WIDTH_MULT_PRINT 4

// коды для взаимодействия пользователья с программой
enum class input_codes
{
	exit = 0,
	template_program,
	tree_insert,
	tree_delete_node,
	tree_print,
	tree_delete,
	tree_find_max,
	tree_find_min,
	tree_count_height,
	tree_count_nodes,
	tree_count_nodes_in_left_subtree,
	tree_count_nodes_in_right_subtree,
	tree_find_node,
	clear_console
};

// строка с коммандами
const char* command_str =
"\nВведите номер комманды:\n\
\t1. Выйти из программы.\n\
\t2. Запустить пример готового алгоритма.\n\
\t3. Добавить элемент в дерево.\n\
\t4. Удалить элемент дерева.\n\
\t5. Распечатать дерево.\n\
\t6. Удалить дерево.\n\
\t7.Найти элемент с максимальным значением.\n\
\t8.Найти элемент с минимальным значением.\n\
\t9.Найти высоту дерева.\n\
\t10.Найти количество элементов дерева.\n\
\t11.Найти количество элементов в левом поддереве.\n\
\t12.Найти количество элементов в правом поддереве.\n\
\t13.Найти количество элементов дерева.\n\
\t14. Очистить консоль.";

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

		INFO("INSERT: элемент вставлен");

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
		postorder(_root, node_delete);
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
	// Возвращаем, если дерево пустое
	if (!_root) return _root;

	// Ищем узел, который хотим удалить
	if (_key < _root->m_data)
	{
		// идем на левую ветвь
		_root->m_left = tree_node_delete(_root->m_left, _key);
	}
	else if (_key > _root->m_data)
	{
		// идем на правую ветвь
		_root->m_right = tree_node_delete(_root->m_right, _key);
	}
	// если был найден нужный элемент
	else {
		// Если у узла один дочерний элемент или их нет
		// если нет левой ветви
		if (!_root->m_left) {
			Node<T>* temp = _root->m_right;
			delete _root;
			return temp;
		}
		// если нет правой ветви
		else if (!_root->m_right) {
			Node<T>* temp = _root->m_left;
			delete _root;
			return temp;
		}

		// Если у узла два дочерних элемента
		// находим минимальный элемент в правой ветви
		Node<T>* temp = tree_find_min(_root->m_right);

		// помещаем найденный элемент в тот, который хотим удалить
		_root->m_data = temp->m_data;

		// удаляем скопированный элемент из правой ветви дерева
		_root->m_right = tree_node_delete(_root->m_right, temp->m_data);
	}
	return _root;
}

// печать дерева
template<class T>
void tree_print(Node<T>* _root, int _lvl = 0)
{
	if (_root)
	{
		// вывод правого поддерева
		tree_print(_root->m_right, _lvl + 1);

		// вывод количества сдвигов
		cout << fixed << setw(_lvl * WIDTH_MULT_PRINT) << setfill(' ') << ' ';

		// вывод корня
		cout << _root->m_data << endl;

		// вывод левого поддерева
		tree_print(_root->m_left, _lvl + 1);
	}
}

// ввод и проверка значений
template<typename T>
T input_and_check(T _min, T _max,
	const char* welcome_str, const char* err_str
	= "Было введено некорректное значение")
{
	// размер массива
	T num;

	// вывод сообщения
	cout << welcome_str << "\n";
	cin >> num;

	// если было введено некорректное значение
	if (num > _max || num < _min) {
		// если была введена не цифра
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(INT_MAX, '\n');
		}

		// отчистка консоли
		//system("cls");
		cout << err_str << "\n";

		// рекурсивное обращение
		num = input_and_check(_min, _max, welcome_str, err_str);
	}
	return num;
}

// функция ведения диалога с пользователем
template<typename T>
void dialog()
{
	/* коды комманд
	exit,								0
	template_program,					1
	tree_create,						2
	tree_insert,						3
	tree_delete_node,					4
	tree_print,							5
	tree_delete,						6
	tree_find_max,						7
	tree_find_min,						8
	tree_count_height,					9
	tree_count_nodes,					10
	tree_count_nodes_in_left_subtree,	11
	tree_count_nodes_in_right_subtree,	12
	tree_find_node,						13
	clear_console						14
	*/

	// переменная содержащая коды действий
	input_codes in_code;

	// элемент для вставки
	T elem;

	//позиция вставки
	int pos_to_insert;

	// переменная списка
	list<T>* lst = NULL;

	// максимальный элемент
	node<T>* max_elem = NULL;

	do
	{
		// запрос у пользователя следующих действий
		in_code = input_codes(input_and_check(0, 14, command_str));

		// запуск соответствующих функций
		switch (in_code)
		{
		case input_codes::exit:
			INFO("Произведен выход");
			break;

		case input_codes::template_program:
			INFO("Запуск примера кода");
			example_program();
			break;

		case input_codes::tree_create:
			if (!list_exists(lst))
				lst = list_create<T>();
			else
				INFO("DIALOG: Список уже существует");
			break;

		case input_codes::tree_insert:
			elem = input_and_check(MIN_VALUE, MAX_VALUE,
				"Введите элемент для вставки в конец");
			list_push(lst, elem);
			break;

		case input_codes::tree_delete_node:
			list_pop(lst);
			break;

		case input_codes::list_insert:
			elem = input_and_check(MIN_VALUE, MAX_VALUE,
				"Введите элемент для вставки");

			pos_to_insert = input_and_check(MIN_VALUE_INSERT, MAX_VALUE_INSERT,
				"Введите позицию для вставки");

			// вставка элемента 
			list_insert(lst, pos_to_insert, elem);
			break;

		case input_codes::tree_print:
			// печать списка
			list_print(lst);
			break;

		case input_codes::tree_delete:
			// удаление списка
			list_delete(lst);
			break;

		case input_codes::clear_console:
			system("cls");
			break;

		case input_codes::find_max:
			list_print_max_elem(lst);
			break;

		default:
			INFO("Неизвестный код")
				break;
		}

	} while (
		// пока пользователь не захотел выйти из программы
		// или пока не запустил пример программыЫ
		in_code != input_codes::exit &&
		in_code != input_codes::template_program
		);
}

int main()
{
	setlocale(LC_ALL, "ru");

	// ДЛЯ ТЕСТОВ

	////char* mass = new char [5] { 'a', 'c', 'B', 'd', 'A' };
	////int* mass = new int [5] { 50, 30, 20, 40, 60 };
	////int* mass = new int [10] { 50, 30, 20, 40, 60, 45, 55, 32, 12, 98 };
	//int* mass = new int [20] { 50, 30, 20, 40, 60, 45, 55, 32, 12, 98, 51, 31, 21, 41, 61, 46, 56, 33, 13, 99 };
	////int* mass = new int [6] { 80,52,48,71,63,67 };
	////int* mass = new int [9] { 20, 10, 35, 15, 17, 27, 24, 8, 30 };
	////int* mass = new int [5] { 50, 30, 50, 40, 600 };

	//Node<int>* root = tree_create(mass, 20);
	////Node<int>* root = nullptr;
	////root = tree_insert_node(root, 2);

	//cout << "inorder: ";
	//inorder(root, node_print);
	//cout << endl;
	//cout << endl;

	//cout << "preorder: ";
	//preorder(root, node_print);
	//cout << endl;
	//cout << endl;

	//cout << "postorder: ";
	//postorder(root, node_print);
	//cout << endl;
	//cout << endl;

	//Node<int>* to_found = tree_find_node(root, 12);
	//if (to_found)
	//{
	//	cout << "найденный элемент: " << to_found->m_data << endl;
	//}

	//cout << "количество узлов дерева: " << tree_count_nodes(root) << endl;

	//cout << "высота дерева: " << tree_count_height(root) << endl;

	//cout << "число узлов в левом поддереве: " << tree_count_nodes(root->m_left) << endl;
	//cout << "число узлов в правом поддереве: " << tree_count_nodes(root->m_right) << endl;

	//cout << "минимальный элемент: " << tree_find_min(root)->m_data << endl;
	//cout << "максимальный элемент: " << tree_find_max(root)->m_data << endl;

	//tree_print(root);
	//cout << endl;
	//root = tree_node_delete(root, 60);
	//root = tree_node_delete(root, 40);
	//root = tree_node_delete(root, 12);
	//root = tree_insert_node(root, 500);

	//cout << "количество узлов дерева: " << tree_count_nodes(root) << endl;

	//cout << "высота дерева: " << tree_count_height(root) << endl;

	//cout << "число узлов в левом поддереве: " << tree_count_nodes(root->m_left) << endl;
	//cout << "число узлов в правом поддереве: " << tree_count_nodes(root->m_right) << endl;

	//cout << "минимальный элемент: " << tree_find_min(root)->m_data << endl;
	//cout << "максимальный элемент: " << tree_find_max(root)->m_data << endl;

	//tree_print(root);
	//cout << endl;

	//cout << "inorder: ";
	//inorder(root, node_print);
	//cout << endl;
	//cout << endl;

	//tree_delete(root);

	return 0;
}