/***********************************************************************\
*                    кафедра № 304 2 курс 3 семестр информатика			*
*-----------------------------------------------------------------------*
*	Project type : solution												*
*	Project name : LW2_GRAPH											*
*	File name    : main.cpp												*
*	Language     : c/c++												*
*	Programmers  : Плоцкий Б.А. Раужев Ю. М.							*
*	Created      : 10/11/22												*
*	Last revision: 30/11/22												*
*	Comment(s)   : 														*
*		Для взвешенного ориентированного графа, состоящего как минимум	*
*	из 10 вершин, реализовать по вариантам:								*
*	1.	алгоритм поиска кратчайшего пути;								*
*	2.	сделав тот же самый граф неориентированным, построить его		*
*	остовное дерево минимальной стоимости.								*
*																		*
\***********************************************************************/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <Windows.h> // для считывания кириллицы
#include <string>

using namespace std;

/****************************************************************
*					   К О Н С Т А Н Т Ы						*
****************************************************************/

// если нужна печать по каждому действию
#define NEED_PRINT_DEBUG 1

// вывод в консоль сообщения
#define INFO(str) if(NEED_PRINT_DEBUG)cout<<"\t"<<str<<"\n";

// коды для взаимодействия пользователья с программой
enum class input_codes
{
	exit = 0,
	template_program,
	read_matrix,
	find_short_path_diykstra,
	create_min_spanning_tree_prim,
	delete_smej_matr,
	print_smej_matr,
	clear_console
};

// путь до графаа
const string graph_file_path = "graph.txt";

// строка с коммандами
const string command_str =
"\nВведите номер комманды:\n\
\t1. Выйти из программы.\n\
\t2. Запустить пример готового алгоритма.\n\
\t3. Считать матрицу из файла.\n\
\t4. Найти кратчайшее расстояние от точки (Дейкстра).\n\
\t5. Построить остовное дерево min стоимости (Прима).\n\
\t6. Удалить матрицу смежности.\n\
\t7. Распечатать матрицу смежности.\n\
\t8. Очистить консоль.";

/****************************************************************
*				М А Т Р И Ц А   С М Е Ж Н О С Т И				*
****************************************************************/
struct SmejMatr
{
	int** m_smej_matr;
	int m_numb_of_vertexes;

	// конструктор
	SmejMatr()
		:m_smej_matr(nullptr), m_numb_of_vertexes(0)
	{}
	// деструктор
	~SmejMatr()
	{
		for (int i = 0; i < m_numb_of_vertexes; i++)
		{
			delete[] m_smej_matr[i];
		}
		delete[] m_smej_matr;
	}
};

// чтение матрицы смежности из файла
SmejMatr* read_smej_matr_from_file(string _file_path = graph_file_path);

// печать матрицы смежности
void print_smej_matr(SmejMatr*);

// конвертация ориентированного графа в неориентированный
void ConverOrientedIntoDisoriened(SmejMatr*&);

/****************************************************************
*        В С П О М О Г А Т Е Л Ь Н Ы Е   Ф У Н К Ц И И          *
****************************************************************/

// печать массива
template<class T, class FUNC>
void PrintArr(T* _arr, int _size, FUNC _prt);

// обработка элемента из массива расстояний при печати
string ChangeDistElem(int _elem);

// обработка элемента из массива посещений при печати
string ChangeVisitElem(int _elem);

// установка одного значения во все ячейка матрицы
template<class T>
void SetMartixValue(T** _mat, int _sizex, int _sizey, T _value);

// проверка на номер команды
bool check_number(int _num);

// пример готовой программы
void example_program();

// выделение памяти под двумерный массив
template<class T>
T** mem_alloc(int _size_x, int _size_y);

// ввод и проверка значений
template<typename T, class FUNC>
T input_and_check(FUNC,
	string welcome_str, string err_str
	= "Было введено некорректное значение");

// функция ведения диалога с пользователем
void dialog();

// Поиск кратчайшего пути
void Diykstra(SmejMatr* _sm, int _start_vert);

// построение Min остового дерева
void Prima(SmejMatr* _sm, int _st);

/****************************************************************
*                Г Л А В Н А Я   Ф У Н К Ц И Я                  *
****************************************************************/

int main()
{
	// для считывания кириллицы
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	// запуск диалога
	dialog();

	return 0;
}

/****************************************************************
*              Р Е А Л И З А Ц И Я   Ф У Н К Ц И Й              *
****************************************************************/

SmejMatr* read_smej_matr_from_file(string _file_path)
{
	ifstream fin(_file_path);

	// если файл не открылся
	if (!fin.is_open())
	{
		INFO("ЧТЕНИЕ: файл не был открыт");
		return nullptr;
	}

	// создаем структуру смежной матрицы
	SmejMatr* sm = new SmejMatr;

	// количество вершин
	sm->m_numb_of_vertexes = 1;

	// выяснение количества строк
	int numb_of_string = 0;
	int cur_vert = 0;
	while (fin.peek() != EOF)
	{
		int temp;
		fin >> temp;
		if (temp != cur_vert)
		{
			cur_vert = temp;
			sm->m_numb_of_vertexes++;
		}
		fin.ignore(INT_MAX, '\n');
		numb_of_string++;
	}

	// перемещение указателя в начало файла
	fin.close();
	fin.open(_file_path);

	//выделение памяти под матрицу
	sm->m_smej_matr = mem_alloc<int>(sm->m_numb_of_vertexes, sm->m_numb_of_vertexes);
	SetMartixValue(sm->m_smej_matr, sm->m_numb_of_vertexes, sm->m_numb_of_vertexes, 0);

	// считываем матрицу смежности
	for (int i = 0; i < numb_of_string; i++)
	{
		int from, to;
		fin >> from >> to;
		fin >> sm->m_smej_matr[to][from];
	}

	fin.close();
	return sm;
}

void print_smej_matr(SmejMatr* _sm)
{
	INFO("Матрица смежности");
	cout << "Количество вершин = " << _sm->m_numb_of_vertexes << endl;
	for (int i = 0; i < _sm->m_numb_of_vertexes; i++)
	{
		for (int j = 0; j < _sm->m_numb_of_vertexes; j++)
		{
			cout << '\t' <<
				(_sm->m_smej_matr[i][j] == INT_MAX ?
					"inf" : to_string(_sm->m_smej_matr[i][j]));
		}
		cout << endl;
	}
}

void ConverOrientedIntoDisoriened(SmejMatr*& _sm)
{
	INFO("Конвертация ориентированного графа в неориентированный");
	for (int i = 0; i < _sm->m_numb_of_vertexes; i++)
	{
		for (int j = 0; j < _sm->m_numb_of_vertexes; j++)
		{
			if (_sm->m_smej_matr[i][j] != 0)
			{
				_sm->m_smej_matr[j][i] =
					_sm->m_smej_matr[i][j];
			}
			else
			{
				_sm->m_smej_matr[i][j] = 
					_sm->m_smej_matr[i][j] == 0 ? INT_MAX : _sm->m_smej_matr[i][j];
			}
		}
	}
}

string ChangeDistElem(int _elem)
{
	return (_elem == INT_MAX ? "inf" : to_string(_elem));
}

string ChangeVisitElem(int _elem)
{
	return to_string(_elem) + " ";
}

// проверка на номер команды
bool check_number(int _num)
{
	return 0 <= _num && _num <= 14;
}

// пример готовой программы
void example_program()
{
	// считывание матрицы смежности из файла
	SmejMatr* sm = read_smej_matr_from_file();

	// позиция, от которой ищется путь
	int pos = 1;

	// поиск кратчайшего пути
	Diykstra(sm, pos);

	// печать матрицы смежности
	print_smej_matr(sm);
}

template<class T = int>
T** mem_alloc(int _size_x, int _size_y)
{
	T** arr = new T * [_size_x];

	for (int i = 0; i < _size_x; i++)
	{
		arr[i] = new T[_size_y];
	}

	return arr;
}

// ввод и проверка значений
template<typename T, class FUNC>
T input_and_check(FUNC _comp,
	string welcome_str, string err_str)
{
	// размер массива
	T symb;

	// вывод сообщения
	cout << welcome_str << "\n";
	cin >> symb;

	// если было введено некорректное значение
	if (!_comp(symb)) {
		// если была введено не то, что нужно было
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(INT_MAX, '\n');
		}
		cout << err_str << "\n";

		// рекурсивное обращение
		symb = input_and_check<T, FUNC>(_comp, welcome_str, err_str);
	}
	return symb;
}

// функция ведения диалога с пользователем
void dialog()
{
	// переменная содержащая коды действий
	input_codes in_code;

	// временное число
	int temp;

	// матрица смежности
	SmejMatr* sm = nullptr;

	/* коды комманд
		exit = 0,
		template_program,
		read_matrix,
		find_short_path_diykstra,
		create_min_spanning_tree_prim,
		clear_console
	*/

	do
	{
		// запрос у пользователя следующих действий
		in_code = input_codes(
			input_and_check<int>(check_number, command_str)
			- 1);

		// запуск соответствующих функций
		switch (in_code)
		{
		case input_codes::exit:
			INFO("Произведен выход");
			break;

		case input_codes::read_matrix:
			INFO("Чтение из файла");
			delete sm;
			sm = read_smej_matr_from_file();
			break;

		case input_codes::template_program:
			INFO("Запуск примера кода");
			example_program();
			break;

		case input_codes::find_short_path_diykstra:
			if (!sm)
			{
				INFO("ДИАЛОГ ДЕЙКСТРА: матрица смежности не существует");
			}
			else
			{
				temp = input_and_check<int>([&sm](int el)
					{
						return el >= 1 & el <= sm->m_numb_of_vertexes;
					},
					"Введите номер вершины, от которой будет происходить поиск расстояния: ",
						"Номер должен быть в пределах: [1," + to_string(sm->m_numb_of_vertexes) + "]");
				Diykstra(sm, temp - 1);
			}
			break;

		case input_codes::create_min_spanning_tree_prim:
			INFO("Остовное дерево минимальной стоимости");
			if (!sm)
			{
				INFO("ДИАЛОГ ДЕЙКСТРА: матрица смежности не существует");
			}
			else
			{
				temp = input_and_check<int>([&sm](int el)
					{
						return el >= 1 & el <= sm->m_numb_of_vertexes;
					},
					"Введите номер вершины, от которой будет строиться остовное дерево: ",
						"Номер должен быть в пределах: [1," + to_string(sm->m_numb_of_vertexes) + "]");
				ConverOrientedIntoDisoriened(sm);
				print_smej_matr(sm);
				Prima(sm, temp-1);
			}
			break;

		case input_codes::delete_smej_matr:
			INFO("Удаление матрицы смежности");
			delete sm;
			sm = nullptr;
			break;

		case input_codes::print_smej_matr:
			INFO("Печать матрицы смежности");
			if (!sm) { INFO("ДИАЛОГ ПЕЧАТЬ: матрица смежности не существует"); }
			else
			{
				print_smej_matr(sm);
			}
			break;

		case input_codes::clear_console:
			system("cls");
			break;

		default:
			INFO("Неизвестный код");
			break;
		}

	} while (
		// пока пользователь не захотел выйти из программы
		// или пока не запустил пример программыЫ
		in_code != input_codes::exit &&
		in_code != input_codes::template_program
		);
}

template<class T, class FUNC>
void PrintArr(T* _arr, int _size, FUNC _prt)
{
	for (int i = 0; i < _size; i++)
	{
		cout << '\t' << _prt(_arr[i]);
	}
	cout << endl;
}

template<class T>
void SetMartixValue(T** _mat, int _sizex, int _sizey, T _value)
{
	for (int i = 0; i < _sizex; i++)
	{
		for (int j = 0; j < _sizey; j++)
		{
			_mat[i][j] = _value;
		}
	}
}

void Diykstra(SmejMatr* _sm, int _start_vert)
{
	INFO("Алгоритм Дейкстры");

	// количество вершин
	int size = _sm->m_numb_of_vertexes;

	// кратчайшие пути до вершин относительно вершины _start_vert
	int* short_distance = new int[size];

	// хранение информации о посещенных вершинах
	bool* visited_node = new bool[size];

	// минимальное расстояние
	int min = INT_MAX;
	// индекс элемента с минимальным расстоянием
	int minindex = INT_MAX;

	// изначально все вершины делаем непосещенными
	// и расстояние до каждой делаем максимално возможным
	for (int i = 0; i < size; i++)
	{
		short_distance[i] = INT_MAX;
		visited_node[i] = false;
	}
	cout << "расстояния: ";
	PrintArr(short_distance, size, ChangeDistElem);

	cout << " посещения: ";
	PrintArr<bool>(visited_node, size, ChangeVisitElem);

	// расстояние для вершины, из которой идем, обнуляем
	short_distance[_start_vert] = 0;

	// шаг алгоритма
	for (int count = 0; count < size; count++)
	{
		cout << "ШАГ: " << count + 1 << endl;

		// поиск непосещенной вершины с минимальным расстоянием
		min = INT_MAX;
		for (int i = 0; i < size; i++)
		{
			if (!visited_node[i] && short_distance[i] < min)
			{
				min = short_distance[i];
				minindex = i;
			}
		}

		// отмечаем посещенным элемент с индексом minindex
		visited_node[minindex] = true;

		// если у элемента есть какое-то расстояние
		if (short_distance[minindex] != INT_MAX)
		{
			// ищем смежные элементы с minindex 
			// и меняем минимальное расстояние до смежных вершин
			// если это необходимо
			for (int i = 0; i < size; i++)	// i - вершина
			{
				if (
					// если существует ребро между i и minindex
					_sm->m_smej_matr[minindex][i] &&
					// если вершина не была посещена
					!visited_node[i] &&
					// если расстояние от _start_vert до i > 
					// расстояния от _start_vert до minindex + 
					// от minindex до i
					short_distance[minindex] + _sm->m_smej_matr[minindex][i]
					< short_distance[i]
					)
				{
					short_distance[i] = short_distance[minindex] + _sm->m_smej_matr[minindex][i];
				}
			}
		}

		cout << "расстояния: ";
		PrintArr(short_distance, size, ChangeDistElem);

		cout << " посещения: ";
		PrintArr<bool>(visited_node, size, ChangeVisitElem);
	}

	// Восстановление путей до вершин
	cout << "\nКратчайшие пути\n";
	// массив с путем 
	int* out_path = new int[size];
	for (int i = 0; i < size; i++)
	{
		// конечная вершина
		int end = i;
		// записываем конечную вершину в массив с путем
		out_path[0] = end + 1;
		// счетчик, для правильной записи элементов в массив с путем
		int k = 1;
		// длина пути до конечной вершины
		int weight = short_distance[end];

		// пока не дошли до начальной вершины и 
		// пока есть путь от начальной до конечной вершины
		while (end != _start_vert && short_distance[end] != INT_MAX)
		{
			// ищем смежные вершины с конечной
			for (int i = 0; i < size; i++)
			{
				if (_sm->m_smej_matr[i][end])
				{
					// если нашли смежную вершину
					// проверяем совпадет ли вес текущей вершины
					// с весом конечной - ребро, смежное с конечной и текущей
					if (weight - _sm->m_smej_matr[i][end] == short_distance[i])
					{
						// вычитаем вес
						weight -= _sm->m_smej_matr[i][end];
						// обновляем конечную вершину
						end = i;
						// добавляем эту вершину в массив с путем
						out_path[k] = i + 1;
						k++;
					}
				}
			}
		}

		// вывод последовательности с длиной
		// если путь существует до вершины
		// и вершина i не является начальной
		if (short_distance[i] != INT_MAX && i != _start_vert)
		{
			// печатаем путь
			for (int j = k - 1; j >= 0; j--)
			{
				cout << out_path[j];
				if (j > 0)	cout << " > ";
			}
			cout << " = " << short_distance[i] << endl;
		}
		// иначе говорим, что пути нет
		else
		{
			cout << _start_vert + 1 << " > " << i + 1 << " = " << "маршрут недоступен" << endl;
		}
	}

	// чистим память
	delete[] out_path;
	delete[] visited_node;
	delete[] short_distance;
}

void Prima(SmejMatr* _sm, int _st)
{
	// количество вершин
	int size = _sm->m_numb_of_vertexes;

	// пройденные вершины
	bool* nodes = new bool[size];
	
	for (int i = 0; i < size; i++)
	{
		nodes[i] = false;
	}

	//Количество рассмотренных вершин
	int num_of_nodes = 0;
	//Берем первую вершину
	nodes[_st] = true;
	//Связанные вершины, минимум
	int x, y, min;
	int weight = 0;
	cout << "Минимальное остовное дерево" << endl;

	//Алгоритм Прима
	while (num_of_nodes < size - 1) {
		x = 0;
		y = 0;
		min = INT32_MAX;
		for (int i = 0; i < size; i++) {
			if (nodes[i]) {
				for (int j = 0; j < size; j++) {
					//Еще не выбирали эту вершину и существует связь между ними
					if (!nodes[j] && _sm->m_smej_matr[i][j] < INT_MAX) {
						if (_sm->m_smej_matr[i][j] < min) {
							min = _sm->m_smej_matr[i][j];
							x = i;
							y = j;
						}
					}
				}
			}
		}
		weight += min;
		cout << x + 1 << " > " << y + 1 << " = " << _sm->m_smej_matr[x][y] << endl;
		nodes[y] = true;
		num_of_nodes++;
	}
	cout << "Вес минимального остовного дерева равен " << weight << endl;

	// вывод матрицы

}

/**************** End Of main.cpp File ***************/
