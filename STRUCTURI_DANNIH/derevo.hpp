#include <iostream>
#include <vector>
#include <fstream>
#include <Windows.h>

using namespace std;

const int N = 9;//Количество вершин

inline void prim()
{
	setlocale(LC_ALL, "Russian");

	//setlocale(LC_ALL, "ru");
	int min_sum = 0; //сумма весов ребер остовного дерева

	ifstream file("data.txt");

	// Массив с пройденными вершинами
	bool nodes[N] = { false };

	//Матрица весов
	vector<vector<int>>W(N, vector<int>(N));

	// Считывание матрица смежности
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			int el;
			file >> el;
			W[i][j] = el == 0 ? INT32_MAX : el;
		}
	}

	// Вывод матрицы смежности
	cout << "Матрица смежности : " << endl;
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
			cout << (W[i][j] < INT32_MAX ? W[i][j] : 0) << " ";
		cout << endl;
	}

	//Количество рассмотренных вершин
	int num_of_nodes = 0;
	//Берем первую вершину
	nodes[0] = true;
	//Связанные вершины, минимум
	int x, y, min;
	int weight = 0;
	cout << "Минимальное остовное дерево" << endl;
	cout << "Вершина" << " : " << "Вес вершины" << endl;

	//Алгоритм Прима
	while (num_of_nodes < N - 1) {
		x = 0;
		y = 0;
		min = INT32_MAX;
		for (int i = 0; i < N; i++) {
			if (nodes[i]) {
				for (int j = 0; j < N; j++) {
					//Еще не выбирали эту вершину и существует связь между ними
					if (!nodes[j] && W[i][j] < INT32_MAX) {
						if (W[i][j] < min) {
							min = W[i][j];
							x = i;
							y = j;
						}
					}
				}
			}
		}
		weight += min;
		cout << x << " - " << y << " : " << W[x][y] << endl;
		nodes[y] = true;
		num_of_nodes++;
	}
	cout << "Вес минимального остовного дерева равен " << weight << endl;
}