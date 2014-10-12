#pragma warning (disable:4996)
#include <iostream>
#include <conio.h>
#include <iomanip>
#include <fstream>

using namespace std;
#define EPSILON 0.00001
std::ostream& print_array(float **a, int n, int m);

float simplex_method(float **a, int n, int m, bool direct) {
	int r, k;	//������ �. ������ � �������
	float RP;	//���������� �. ��������


	while (1)
	{
		r = k = -1;
		//���� ����������� ������� � ������ �� ������� ��������
		float min = 100000;
		for (int i = 0; i < m - 1; i++)
			if (a[i][0] < 0)
				for (int j = 1; j < n; j++)
					if (a[i][j] < 0 && (a[i][0] / a[i][j]) < min)
					{
						r = i;
						k = j;
						RP = a[i][j];
						min = (a[i][0] / a[i][j]);
					}

		//���� ����������� ������� � ������ �� ������ �������
		min = 100000;
		if (r == -1 || k == -1)
		{
			for (int j = 0; j < n; j++)
				if (((a[m - 1][j] < 0 && direct) || (!direct && a[m - 1][j] > 0)) && j != 0)
					for (int i = m - 2; i >= 0; i--)
						if (a[i][0] / a[i][j] < min && a[i][0] / a[i][j] > 0)
						{
							r = i;
							k = j;
							RP = a[i][j];
							min = a[i][0] / a[i][j];
						}
		}
		//�������� �� ������� �. �������, ������
		if (r == -1 || k == -1)
			break;

		cout<<"stroka "<<r+1<<"	stolbec "<<k+1; 
		//������ ������������� ��� �. �������, ������
		for (int i = 0; i < m; i++)
			if (i != r)
				for (int j = 0; j < n; j++)
					if (j != k)
						a[i][j] -= a[i][k] * a[r][j] / RP;

		a[r][k] = 1 / a[r][k]; // a[r][k] -> a*[r][k]

		//������ �. ������
		for (int j = 0; j < n; j++)
			if (j != k)
				a[r][j] = a[r][j] / RP;

		//������ �. ��������
		for (int i = 0; i < m; i++)
			if (i != r)
				a[i][k] = a[i][k] / RP*(-1);

		//�������, ����������� �� ��� ������� (�� ����������� ������������� ��������� � �������)
		int count = 0;
		for (int j = 0; j < n; j++)
			if (a[m - 1][j] <= 0) // a[m-1][j] <= 0
				count++;
		
		cout << endl;
		print_array( a, n , m);
		cout << endl;

		//������� ��������� � ������� �� �����, ���� ������� ����������
		if (count == n)
			break;
	}
	//����� ������������� ������
	cout<<"otvet "<<a[m-1][0]<<endl<<endl<<"----------------------------------------------------------------------------"<<endl;
	return a[m-1][0];
	
}



float** adv_transp(float** a, int rows, int lines) {
	//�������� ������
	int i, j;
	float **b = new float *[rows];
	for (i = 0; i< rows; i++)
		b[i] = new float[lines];

	//������������� �������� ��������� ������. ��������� �������������
	for (i = 0; i < lines; ++i)
		b[rows - 1][i] = a[lines - i - 1][0];
	//������������� ������� �����������
	for (i = 1; i < rows; ++i)
		for (j = 1; j < lines; ++j)
			b[i-1][j] = a[j-1][i];
	//������������� �������� F
	for (i = 1; i < rows; ++i)
		b[i-1][0] = a[lines - 1][i];
	//��������� �������
	for (i = 0; i < rows; ++i)
		for (j = 0; j < lines; ++j)
			b[i][j] *= -1;
		
	return b;
}

void free_array(float **a, int lines, int rows){
	for (int i = 0; i < lines; ++i)
		delete a[i];
	delete a;
};


//������� ������
std::ostream& print_array( float **a , int m, int n){
	cout<<endl;
	cout.precision(2);
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
			cout << std::setw(8) << a[i][j] << "  " ;
		cout << endl;
	}
	cout<<endl;
	return cout;
}

bool matrix_interface(float*** a, int &rows, int &lines){
	size_t choice;
	std::ifstream f("in.txt");
	f >> lines >> rows;
	//�������� �������
	*a = new float*[rows];
	for (int i = 0; i < lines; i++)
		(*a)[i] = new float[lines];
	
	//���� �������
	for (int i = 0; i < lines; i++)
		for (int j = 0; j < rows; j++)
			f >> (*a)[i][j];
	do {
		cout << "Lab 1 or 2? (Type 0 for Lab1 or 1 for Lab2): ";
		cin >> choice;
	} while (choice!=1 && choice != 0);
	f.close();
	return !((bool)choice);
};

void main() {
	int rows, lines;
	float ** a = NULL;

	bool choice = matrix_interface(&a, rows, lines);
	
	//�����������
	float** a_s = new float*[rows];

	for (int i = 0; i < lines; i++)
	{
		a_s[i] = new float[lines];
		for (int j = 0; j < rows; j++)
			a_s[i][j] = a[i][j];			
	}

	for (int j = 0; j < rows; j++)
		a_s[lines-1][j] *= -1;
	
	//������ ��	
	cout << "formulirovka PZ:";
	print_array(a_s, rows, lines);
	float r1 = simplex_method(a_s, rows, lines, !choice);


	//������ ��
	if (!choice)
	{
		float **b = adv_transp(a, rows, lines);
		cout << "Formulirovka DZ: " << endl;
		print_array(b, lines, rows);
		float r2 = simplex_method(b, lines, rows, choice);
		cout << " DZ: " << r2 << endl;
		//������ ������ � �����
		free_array(b, lines, rows);
	}
	cout << "PZ: " << r1<<endl;
	
	system("PAUSE");
	getch();

	//������ ������
	free_array(a, rows, lines);
	free_array(a_s, rows, lines);
}


