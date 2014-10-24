
#pragma warning (disable:4996)
#include <iostream>
#include <conio.h>
#include <iomanip> 
#include <fstream>
#include <Windows.h>
#include <vector>
#include <locale.h>
#include <vector>
#include "tree.h"

using namespace std;
typedef TreeNode<float**> FloatTreeNode;
typedef std::vector<std::vector<float>> Matrix;
typedef TreeNode<Matrix> MatrixNode;


std::ostream& print_array(Matrix& a);


enum Labs
{
	LAB_1,
	LAB_2,
	LAB_3,
	LAB_MAX = LAB_3 
};


float simplex_method_min(Matrix &a){
	int lines = a.size(); //LINES
	int rows = a[0].size(); //ROWS



	int r, k;	//����� �. ������ � ������
	float RP;	//��६����� �. �����


	while (1)
	{
		r = k = -1;
		//�饬 ࠧ���騥 �⮫��� � ��ப� �� �⮫��� ����⠭�
		float min = 100000;
		for (int i = 0; i < lines - 1; i++)
			if (a[i][0] < 0)
				for (int j = 1; j < rows; j++)
					if (a[i][j] < 0 && (a[i][0] / a[i][j]) < min)
					{
			r = i;
			k = j;
			RP = a[i][j];
			min = (a[i][0] / a[i][j]);
					}

		//�饬 ࠧ���騥 �⮫��� � ��ப� �� ��ப� �㭪樨
		min = 0;
		bool flag = 1;
		if (r == -1 || k == -1)
		{

			int max = 100000;
			int min_pos = -1;

			do
			{
				r = -1; k = -1;
				for (int i = 1; i < rows; i++)
				{
					if (abs(a[lines - 1][i]) > min && abs(a[lines - 1][i]) < max && a[lines - 1][i] < 0) //&& ((&& direct) || (a[lines - 1][i] > 0 && !direct)))
					{
						min = abs(a[lines - 1][i]);
						min_pos = i;
					}
				}

				if (min_pos > 0)
				{
					k = min_pos;
					max = abs(a[lines - 1][k]);
				}
				else{
					break;
				}

				min = 100000;
				min_pos = -1;
				for (int j = 0; j < lines - 1; j++)
				{
					if ((min > a[j][0] / a[j][k]) && ((a[j][0] / a[j][k]) > 0))
					{
						min = a[j][0] / a[j][k];
						min_pos = j;
					}
				}

				if (min_pos >= 0)
					r = min_pos;

			} while (r == -1 && k == -1);



		}

		//�஢�ઠ �� ����稥 �. �⮫��, ��ப�
		if (r == -1 || k == -1)
			break;
		RP = a[r][k];

		std::cout << "������:  " << r + 1 << "	||	�������: " << k + 1;
		//���� �����樥�⮢ ��� �. �⮫��, ��ப�
		for (int i = 0; i < lines; i++)
			if (i != r)
				for (int j = 0; j < rows; j++)
					if (j != k)
						a[i][j] -= a[i][k] * a[r][j] / RP;

		a[r][k] = 1 / a[r][k]; // a[r][k] -> a*[r][k]

		//���� �. ��ப�
		for (int j = 0; j < rows; j++)
			if (j != k)
				a[r][j] = a[r][j] / RP;

		//���� �. �⮫���
		for (int i = 0; i < lines; i++)
			if (i != r)
				a[i][k] = a[i][k] / RP*(-1);

		//ᬮ�ਬ, ��⨬��쭮� �� �� �襭�� (�� ��������� ����⥫��� ����⮢ � �㭪樨)
		int count = 0;
		for (int j = 1; j <rows; j++)
			if (a[lines - 1][j] <= 0)
				count++;

		std::cout << endl;
		print_array(a);
		std::cout << endl;

		//�뢮��� १���� � ��室�� �� 横��, �᫨ �襭�� ��⨬��쭮
		if (count == rows - 1)
			break;
	}
	//�뢮� ����稢襣��� �⢥�
	std::cout << "�����: " << a[lines - 1][0] << endl << endl << "----------------------------------------------------------------------------" << endl;
	return a[lines - 1][0];
}


float simplex_method(Matrix& a, bool &is_failed){
	int lines = a.size(); //LINES
	int rows = a[0].size(); //ROWS
	//������� - ���� ��������
	is_failed = false;


	int r, k;	//����� �. ��ப� � �⮫��
	float RP;	//��६����� �. �����


	while (1)
	{
		is_failed = true;
		for (int i = 1; i < rows && is_failed; i++)
			if (a[lines - 1][i] < 0)
				is_failed = false;
		if (is_failed) {
			int j;
			for ( j = 0; (j < lines - 1) && is_failed; j++)
				if (a[j][0] < 0) {
					int i;
					for ( i = 1; (i < rows) && is_failed; i++)
						if (a[j][i] < 0)
							is_failed = false;
					if (i == rows) {
						is_failed = true;
						break;
					}
				}

			if (j == lines - 1)
				is_failed = false;

			if (is_failed)
				return 0;
		}

		r = k = -1;
		//�饬 ࠧ���騥 �⮫��� � ��ப� �� �⮫��� ����⠭�
		float min = 100000;
		for (int i = 0; i < lines - 1; i++)
			if (a[i][0] < 0)
				for (int j = 1; j < rows; j++)
					if (a[i][j] < 0 && (a[i][0] / a[i][j]) < min)
					{
						r = i;
						k = j;
						RP = a[i][j];
						min = (a[i][0] / a[i][j]);
					}

		//�饬 ࠧ���騥 �⮫��� � ��ப� �� ��ப� �㭪樨		
		
		if (r == -1 || k == -1)
		{
			bool flag = 1;
			min = 0;
			int max = 100000;
			int min_pos = -1;

			do
			{
				r = -1; k = -1;
				for (int i = 1; i < rows; i++)
				{
					if (abs(a[lines - 1][i]) > min && abs(a[lines - 1][i]) < max && a[lines - 1][i] < 0) //&& ((&& direct) || (a[lines - 1][i] > 0 && !direct)))
					{
						min = abs(a[lines - 1][i]);
						min_pos = i;
					}
				}
				
				if (min_pos > 0)
				{
					k = min_pos;
					max = abs(a[lines - 1][k]);
				}
				else{
					break;
				}
					
				min = 100000;
				min_pos = -1;
				for (int j = 0; j < lines - 1; j++)
				{
					if ((min > a[j][0] / a[j][k]) && ((a[j][0] / a[j][k] ) > 0))
					{
						min = a[j][0] / a[j][k];
						min_pos = j;
					}
				}

				if (min_pos >= 0)
					r = min_pos;
									
			} while (r == -1 && k == -1);		


			
		}

		//�஢�ઠ �� ����稥 �. �⮫��, ��ப�
		if (r == -1 || k == -1)
			break;

		RP = a[r][k];

		std::cout << "������:  " << r + 1 << "	||	�������: " << k + 1;
		//���� �����樥�⮢ ��� �. �⮫��, ��ப�
		for (int i = 0; i < lines; i++)
			if (i != r)
			for (int j = 0; j < rows; j++)
					if (j != k)
						a[i][j] -= a[i][k] * a[r][j] / RP;

		a[r][k] = 1 / a[r][k]; // a[r][k] -> a*[r][k]

		//���� �. ��ப�
		for (int j = 0; j < rows; j++)
			if (j != k)
				a[r][j] = a[r][j] / RP;

		//���� �. �⮫���
			for (int i = 0; i < lines; i++)
		if (i != r)
			a[i][k] = a[i][k] / RP*(-1);

		//ᬮ�ਬ, ��⨬��쭮� �� �� �襭�� (�� ��������� ����⥫��� ����⮢ � �㭪樨)
		int count = 0;
		for (int j = 1; j <rows; j++)
		if (a[lines - 1][j] <= 0) // a[m-1][j] <= 0
			count++;

		std::cout << endl;
		print_array(a);
		std::cout << endl;

		//�뢮��� १���� � ��室�� �� 横��, �᫨ �襭�� ��⨬��쭮
		if (count == rows-1)
			break;
	}
	//�뢮� ����稢襣��� �⢥�
	std::cout << "����� " << a[lines - 1][0] << endl << endl << "----------------------------------------------------------------------------" << endl;
	return a[lines - 1][0];
}


Matrix adv_transp(Matrix& a) {  
	size_t rows = a[0].size(),
		   lines = a.size();

	int i, j;
	Matrix b;
	
	b.resize(rows);
	for (i = 0; i < rows; ++i){
		b[i].resize(lines);
	}

	
	for (i = 0; i < lines-1; ++i)
		b[rows - 1][i+1] = a[i][0];
	
	for (i = 1; i < rows; ++i)
		for (j = 1; j < lines; ++j)
			b[i - 1][j] = a[j - 1][i];
	
	for (i = 1; i < rows; ++i)
		b[i - 1][0] = a[lines - 1][i];
	
	for (i = 0; i < rows-1; ++i)
		for (j = 0; j < lines; ++j)
			b[i][j] *= -1;



	return b;
}

 
Labs matrix_interface(Matrix &a, int &rows, int &lines){
	Labs choice;
	int t;
	bool err_flag = false;
	std::ifstream f("in.txt");
	f >> lines >> rows;

	//�������� �������
	//�������� ������
	a.resize(lines);
	//���� �������
	for (int i = 0; i < lines; i++) {
		a[i].resize(rows);
		for (int j = 0; j < rows; j++)
			f >> (a)[i][j];
	}
	do {
		std::cout << "������� ����� ����������� ������:	1-" << (Labs::LAB_MAX + 1) << endl; //������ � ����
		cin >> t;
		choice = (Labs)(t - 1); //������������ � t
		if ((err_flag = (choice >= Labs::LAB_MAX + 1 || t <= 0)))
			std::cout << "������, ��������� ����" << endl;
	} while (err_flag);

	f.close();
	return choice;
};


std::ostream& print_array(Matrix &a){
	cout << endl;
	cout.precision(2);
	size_t n = a.size(), m = a[0].size();
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < m; j++)
			cout << std::setw(8) << a[i][j] << "  ";
		cout << endl;
	}
	cout << endl;
	return cout;
}

//���������� ���, ���� ���� ������ ������� - �������������
//is_f_integer - �������� �� f - �����
bool is_integer(float a){
	if (abs(a) < 0.01) //�������� ��� 0
		return true;
	return abs(ceilf(a) - a) < 0.01;
}


bool check_solution_integer(Matrix &a, bool &is_f_integer){
	size_t lines = a.size();

	for (size_t i = 0; i < lines - 1; ++i)
		if (!is_integer(a[i][0]))
			return false;

	is_f_integer = is_integer(a[lines - 1][0]);
	return true;

}

void splitChildren(MatrixNode &node){

	bool is_integer;  // ������ �����!
	Matrix m = *node;
	int line_num;
	MatrixNode child_1(m), child_2(m);
	size_t i, lines = m.size(), rows = m[0].size();
	std::vector<float> line_to_add, line_to_add_ceil;
	// ���������������, �����! �� ���� ���! ��� ��� �� ���?
	for (i = 0; i < lines - 1; ++i) // ���� ��������y ��������
		if (ceilf(m[i][0]) != m[i][0])
		{
			line_num = i;
			line_to_add.resize(rows);
			line_to_add_ceil.resize(rows);
			for (int j = 0; j < rows; j++)
				if (j == 0)
				{
					line_to_add[j] = floorf(m[line_num][j])-m[line_num][j];
					line_to_add_ceil[j] = m[line_num][j]-ceilf(m[line_num][j]);    //Deutschland!!!!!! Deutschland !!!! and rabbits :3
				}
				else {
					line_to_add_ceil[j] = m[line_num][j];
					line_to_add[j] = -(m[line_num][j]);
				}
			//������� �� �����
			break;
		}

	//��������� � ������������� ������
	(*child_1).insert((*child_1).begin() + (*child_1).size() - 1, line_to_add);
	(*child_2).insert((*child_2).begin() + (*child_2).size() - 1, line_to_add_ceil);
	node.addChild(child_1);
	node.addChild(child_2);

	//������ �����!
}

bool recursive_integer_method(MatrixNode &entryNode, Matrix& best){
	bool failed = false;
	float sm = simplex_method(*entryNode, failed);
	cout << "����� ��������-������" << std::endl;
	print_array(*entryNode);
	if (failed)
		return false;

	
	
	bool  is_integer, f_integer = false;
	//���� ����� ����� �� ��������� � ���� ����-�� �����, � ������ ������ �����, ��� � ����� ������� ����-�� ��� ��������, �� ������ ���� �� ���
	if ((is_integer = check_solution_integer(*entryNode, f_integer)) && f_integer
		&& (sm>best[best.size() - 1][0] || !check_solution_integer(best, f_integer) || !f_integer))  //���� ���� �������!
		best = *entryNode;
	//��������, �����!!!
	if (!is_integer)
		splitChildren(entryNode);
	size_t l = entryNode.length();
	for (size_t i = 0; i < l; ++i) {
		cout << "�������:> " << i << std::endl;
		print_array(**entryNode.getChild(i));
		cout << "----------------------------------" << std::endl;
		if (recursive_integer_method(*entryNode.getChild(i), best))
			is_integer = true;
	}
		

	//���������� false, ���� �������
	return is_integer;
};


void main() {
	setlocale(LC_ALL, "Russian");
	int rows, lines;
	bool failed = false; //���� �� �������
	Matrix a;
	Labs choice = matrix_interface(a, rows, lines);

	//�����������
	Matrix a_s = a;

	for (int j = 0; j < rows; j++)
		a_s[lines - 1][j] *= -1;
	//������ �� (1-3 ����)
	cout << "������������ ������ ������:";
	print_array(a_s);
	float r1 = simplex_method(a_s, failed);


	if (choice == LAB_2)
	{
		Matrix b = adv_transp(a);
		cout << "������������ ������������ ������: " << endl;
		print_array(b);
		float r2 = -simplex_method_min(b);
		cout << "������������ ������: " << r2 << endl;
		if (r1 == r2)
			cout << "������� ������������ ������ ����� ������� ������ ������";
	}

	if (choice == LAB_3){
		bool needed = true;
		bool  is_f_integer = false;
		MatrixNode parent(a_s);
		Matrix best_match = a_s;
		float best_match_simplex;
		//best_match - ������ �������, �� ������ ������.
		if (check_solution_integer(a_s, is_f_integer) && is_f_integer) {
			best_match_simplex = r1;
		}
		else {
			//����������������������
			failed = !recursive_integer_method(parent, best_match);
			best_match_simplex = best_match[best_match.size() - 1][0];
		}


		//������� ���������
		if (!failed) {
			print_array(best_match);
			cout << "������������� ������: " << best_match_simplex << endl;
			parent.clear();
		}
		else
			cout << "������������� ������ �� ����� �������!" << endl;

	}

	getch();
}



//http://www.youtube.com/watch?v=yzZg6ZpHHY4