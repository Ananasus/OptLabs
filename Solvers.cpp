#include "Solvers.h"
#include <algorithm>
#include <cmath>
#include <math.h>


#if defined(_MSC_VER)&&_MSC_VER<1600
float round(float value)
{
	
   return floor(value + 0.5);
}
#endif


bool LabOptimizations::CloseToZero(double a){
	const volatile float EPSILON = 0.01;
	return (fabs(a) < EPSILON);
}
//is_f_integer - �������� �� f - �����
bool LabOptimizations::IsInteger(float a){
	if (CloseToZero(a))
		return true;
	return fabs(round(a) - a) < 0.01;
}
//���������� ���, ���� ���� ������ ������� - �������������

//CLASSES
void LabOptimizations::SimplexMatrix::addLineLabel(){
	line_labels.push_back(max_x++);
};
void LabOptimizations::SimplexMatrix::addRowLabel(){
	this->row_labels.push_back(max_x++);
};
void LabOptimizations::SimplexMatrix::genLabels(){
	row_labels.clear();
	line_labels.clear();
	int i = 0;
	max_x = 0;
	for (i = 0; i < (*this)[0].size(); ++i)
		row_labels.push_back(max_x++);
	for (i = 0; i < this->size(); ++i)
		line_labels.push_back(max_x++);
};
void LabOptimizations::SimplexMatrix::swapLabels(size_t line, size_t row){
	size_t temp = this->row_labels[row];
	this->row_labels[row] = this->line_labels[line];
	this->line_labels[line] = temp;
};
//������� ��������
LabOptimizations::SimplexMatrix::SimplexMatrix(const Matrix& m) :Matrix(m){
	max_x = 0;
	genLabels();
};
LabOptimizations::SimplexMatrix::SimplexMatrix(const SimplexMatrix& sm) :Matrix(sm){
	max_x = 0;
	this->addLabelsFrom(sm);
}
LabOptimizations::SimplexMatrix::SimplexMatrix(){
	max_x = 0;
};

LabOptimizations::SimplexMatrix& LabOptimizations::SimplexMatrix::operator = (const Matrix& m){
	int i = 0, j = 0;
	this->max_x = 0;
	this->assign(m.begin(), m.end());
	genLabels();
	return *this;
};

//�������	
//�����
std::string LabOptimizations::SimplexMatrix::toString() const{
	int i, j;
	const short width = 8;
	std::stringstream ss;
	ss.precision(2);
	ss.setf(ios::fixed);
	size_t lines = this->size(), rows = (*this)[0].size();
	//������ �� �������
	ss << std::setw(width) << " " << " ";
	for (i = 0; i < rows; ++i)
		ss << std::setw(width + 1) << (i ? "x" : "s") << row_labels[i];
	ss << endl;
	for (i = 0; i < lines; ++i){
		ss << std::setw(width);
		if (i < lines - 1)
			ss << "x" << line_labels[i];
		else
			ss << std::setw(width + 1) << " F";
		ss << " ";

		for (j = 0; j < rows; ++j)
			ss << std::setw(width + 1) << (*this)[i][j] << " ";
		ss << std::endl;
	}
	ss << std::endl;
	return ss.str();

};
//������
void LabOptimizations::SimplexMatrix::addLabelsFrom(const SimplexMatrix& sm) {
	this->line_labels = sm.line_labels;
	this->row_labels = sm.row_labels;
	this->max_x = sm.max_x;
};

LabOptimizations::SimplexMatrix LabOptimizations::SimplexMatrix::SimplexInvert() const {
	size_t rows = (*this)[0].size(),
		lines = this->size();

	int i, j;
	SimplexMatrix b;
	//resize matrix lines
	b.resize(rows);
	//resize matrix rows
	for (i = 0; i < rows; ++i)
		b[i].resize(lines);

	for (i = 0; i < lines - 1; ++i)
		b[rows - 1][i + 1] = (*this)[i][0];
	for (i = 1; i < rows; ++i)
		for (j = 1; j < lines; ++j)
			b[i - 1][j] = (*this)[j - 1][i];
	for (i = 1; i < rows; ++i)
		b[i - 1][0] = (*this)[lines - 1][i];
	for (i = 0; i < rows-1; ++i)
		for (j = 0; j < lines; ++j)
			b[i][j] *= -1;
	b.row_labels.push_back(0);
	b.row_labels.insert(b.row_labels.begin()+1,this->line_labels.begin(), this->line_labels.end()-1);
	b.max_x = this->max_x;
	b.line_labels.assign(this->row_labels.begin()+1, this->row_labels.end());
	b.line_labels.push_back(b.max_x-1);
	return b;
}

bool LabOptimizations::SimplexMatrix::SplitChildren(SimplexMatrixNode &node){
	//������� ����
	SimplexMatrix m = *node;
	int line_num;
	SimplexMatrixNode child_1(m), child_2(m);
	size_t i, lines = m.size(), rows = m[0].size();
	//����� �����������: ����������� �� ���������� ���� � �����
	std::vector<float> line_to_add, line_to_add_ceil;
	for (i = 0; i < lines - 1; ++i)
		//�������� - �������� �� ���������� �������������
		if (!IsInteger((*this)[i][0])&&IsTargetFunctionParameter(i,true))
		{
			//������, �� ������� ���������� ����������
			line_num = i;
			//�������� ������
			line_to_add.resize(rows);
			line_to_add_ceil.resize(rows);
			for (int j = 0; j < rows; j++)
				//��������� ������� �����������
				if (j == 0)
				{
					line_to_add[j] = floorf(m[line_num][j]) - m[line_num][j];
					line_to_add_ceil[j] = m[line_num][j] - ceilf(m[line_num][j]);
				}
				else {
					line_to_add_ceil[j] = m[line_num][j];
					line_to_add[j] = -(m[line_num][j]);
				}
				//������� �� �����
				break;
		}
	//���� �� ����� ���������� ���������
	if (i == lines - 1)
		return false;
	//��������� � ������������� ������ ��� ������ ����� ��������-�������.
	(*child_1).insert((*child_1).begin() + (*child_1).size() - 1, line_to_add);
	(*child_2).insert((*child_2).begin() + (*child_2).size() - 1, line_to_add_ceil);
	//��������� �����
	(*child_1).addLineLabel();
	(*child_2).addLineLabel();
	//��������� � ������ ��� �������
	node.addChild(child_1);
	node.addChild(child_2);
	return true;
};

float LabOptimizations::SimplexMatrix::SimplexSolve(bool &is_failed, bool to_min, bool show_progress, bool show_final_tab){
	int lines = this->size();
	int rows = (*this)[0].size();

	is_failed = false; //���� ������������ �������


	int r, k; //������� ����������� ������� � ������ 
	float RP; //�������� ������������ ��������


	while (1)
	{
		is_failed = true;

		//�������� ������ ������� �� ������������� ��������
		for (int i = 1; i < rows && is_failed; i++) {
			float &v = (*this)[lines - 1][i];
			if (v<0.01&&v>-0.01)
				v = 0;
			if ((v < 0 && to_min) || (v > 0 && !to_min))
				is_failed = false;
		}
			

		//�������� ������� ��������� ������ �� ������������� ��������
		r = k = -1;
		float min = 100000;
		bool met_negative = false; //����, �������� �� ������������� ������� � ������� ������
		int i;
		for (i = 0; i < lines - 1; i++) {
			float &v = (*this)[i][0];
			 if (v<0.01&&v>-0.01)
				 v = 0;
			if ( (v > 0 && to_min) || (v < 0 && !to_min) )
			{

				is_failed = true;
				met_negative = true;
				int j;
				for (j = 1; j < rows; j++)
				if ((*this)[i][j] < 0)
				{
					is_failed = false;
					if (((*this)[i][0] / (*this)[i][j]) < min)
					{
						r = i;
						k = j;
						RP = (*this)[i][j];
						min = ((*this)[i][0] / (*this)[i][j]);
						this->swapLabels(r, k);
					}
				}
			}
			else if (!met_negative)
				is_failed = false;
		}
			

		//����� ������, ���� ������� �� �������
		if (is_failed)
			return 0;

		//���������� ������������ ������� � ������ (���� ��� �� �����)
		if (r == -1 || k == -1)
		{
			bool flag = 1, first_time = true;
			min = 0;
			int max = 100000;
			int min_pos = -1;

			do
			{
				r = -1; k = -1;
				for (int i = 1; i < rows; i++)
				{
					float m = (*this)[lines - 1][i];

					if (
						((abs(m) > min) && (abs(m) < max) &&
						((m < 0 && !to_min) || (m > 0 && to_min))) 
					   )
						//old((m < 0 && (choice == LAB_2 || choice == LAB_3)) || (m < 0 && choice == LAB_1))) //����� ���� �������
					{
						first_time = false;
						min = abs(m);
						min_pos = i;
					}
				}

				if (min_pos > 0)
				{
					k = min_pos;
					max = abs((*this)[lines - 1][k]);
				}
				else
					break;

				min = 100000;
				min_pos = -1;
				for (int j = 0; j < lines - 1; j++)
				{
					if ((min >(*this)[j][0] / (*this)[j][k]) && (((*this)[j][0] / (*this)[j][k]) > 0))
					{
						min = (*this)[j][0] / (*this)[j][k];
						min_pos = j;
					}
				}

				if (min_pos >= 0)
					r = min_pos;

			} while (r == -1 && k == -1);



		}

		//����� �� ����� ������� , ���� ���-�� �� ���
		if (r == -1 || k == -1)
			break;

		RP = this->operator[](r)[k];
		this->swapLabels(r, k);
		std::cout << "������ : " << r + 1 << " || �������: " << k + 1 << " ��: " << RP << endl;
		// ������ ��������� ��� ������������ ������� � ������
		for (int i = 0; i < lines; i++)
			if (i != r)
				for (int j = 0; j < rows; j++)
					if (j != k)
						(*this)[i][j] -= (*this)[i][k] * (*this)[r][j] / RP;

		//���������� ������ �������� ������������ ��������
		(*this)[r][k] = 1 / (*this)[r][k]; // a[r][k] -> a*[r][k]

		//������ ����������� ������
		for (int j = 0; j < rows; j++)
			if (j != k)
				(*this)[r][j] = (*this)[r][j] / RP;

		//������ ������������ �������
		for (int i = 0; i < lines; i++)
			if (i != r)
				(*this)[i][k] = (*this)[i][k] / RP*(-1);

		//�������� �� ��������������� ������� �� ������ �������
		int count = 0;
		for (int j = 1; j < rows; j++)
			if ((*this)[lines - 1][j] <= 0) // a[m-1][j] <= 0
				count++;

		std::cout << endl;
		if (show_progress)
		{
			std::cout << this->toString();
			std::cout << endl;
		}

		//����� �� �����, ���� ������� �������������
		if (count == rows - 1)
			break;
	}
	//����� ��������
	if (show_final_tab)
	{
		std::cout << this->toString();
		cout << endl;
	}
	std::cout << (*this)[lines - 1][0] << " - �������" << endl
		<< endl
		<< "----------------------------------------------------------------------------" << endl;
	return (*this)[lines - 1][0];
};

//SOLVERS LAB 1
LabOptimizations::SolverLab1::SolverLab1() :is_failed(false), is_solved(false) {

}

void LabOptimizations::SolverLab1::Input(SimplexMatrix &sm) {
	this->sm = sm;
};

std::string LabOptimizations::SolverLab1::Output() {
	if (is_solved) {
		if (is_failed)
			return "�������� ����� ����� � �������";
		else {
			std::stringstream ss;
			ss << "������� ������������ ������: " << sm[sm.size() - 1][0] << std::endl;
			ss << sm.toString();
			return ss.str();
		}

	}
	else
		return "������������ ������ �� ������. "
		"��������� ������� ����� Solve()";

}
void LabOptimizations::SolverLab1::Solve() {
	this->sm.SimplexSolve(is_failed, true, false);
	this->is_solved = true;
}
//SOLVERS LAB 3

//SOLVERS LAB 6
LabOptimizations::SolverLab6::SolverLab6() :is_failed(false), is_solved(false) {

}

void LabOptimizations::SolverLab6::Input(SimplexMatrix &sm) {
	this->sm = sm;
};

std::string LabOptimizations::SolverLab6::Output() {
	if (is_solved) {
		if (is_failed)
			return "�������� ����� ����� � �������";
		else {
			std::stringstream ss;
			ss << "������� ������ �������: ";
			ss << sm.toString();
			for each(string out in this->output)
				ss << out << std::endl;

			return ss.str();
		}

	}
	else
		return "������������ ������ �� ������. ��������� ������� ����� Solve()";

}
void LabOptimizations::SolverLab6::Solve() {
	this->output.push_back("��������: \r\n"+Bernulli(this->sm));
	this->output.push_back("��������: \r\n" + MaxMin(this->sm));
	this->output.push_back("���������?: \r\n" + MaxMax(this->sm));
	this->output.push_back("������?: \r\n" + Gurvic(this->sm));
	this->output.push_back("������� ������: \r\n" + Risks(this->sm));
	is_solved = true;
}

std::string LabOptimizations::SolverLab6::Bernulli(Matrix m)
{
	std::stringstream ss;
	int rows = m[0].size(), lines = m.size();
	float* math_wait = new float[lines];
	int sum = 0;
	vector<int> opt_strategy_num;
	float max_math_wait = 0;

	for (int i = 0; i<lines; i++)
	{
		for (int j = 0; j<rows; j++)
			sum += m[i][j];
		math_wait[i] = sum / rows;
		sum = 0;
	}

	for (int i = 0; i < lines; i++) // 
		if (math_wait[i]>max_math_wait)
		{
			max_math_wait = math_wait[i];
		}

	for (int i = 0; i < lines; i++) // 
		if (math_wait[i] == max_math_wait)
			opt_strategy_num.push_back(i);

	ss << " :" << endl;
	for (int i = 0; i<opt_strategy_num.size(); i++)
		ss << "a" << opt_strategy_num[i] << endl;
	ss << " : " << max_math_wait << endl;
	return ss.str();
}

std::string LabOptimizations::SolverLab6::MaxMin(Matrix m)
{
	std::stringstream ss;
	int rows = m[0].size(), lines = m.size();
	int* mins = new int[lines];
	int maxmin = 0;
	int min;
	vector<int> opt_strategy_num;

	for (int i = 0; i<lines; i++)
	{
		min = m[i][0];
		for (int j = 0; j<rows; j++)
			if (m[i][j]<min)
				min = m[i][j];
		mins[i] = min;
	}

	for (int i = 0; i < lines; i++)
		if (mins[i]>maxmin)
			maxmin = mins[i];

	for (int i = 0; i < lines; i++)
		if (mins[i] == maxmin)
			opt_strategy_num.push_back(i);

	ss << " :" << endl;
	for (int i = 0; i<opt_strategy_num.size(); i++)
		ss << "a" << opt_strategy_num[i] << endl;
	ss << " : " << maxmin << endl;
	return ss.str();
}

std::string LabOptimizations::SolverLab6::MaxMax(Matrix m)
{
	std::stringstream ss;
	int rows = m[0].size(), lines = m.size();
	bool add = true;
	vector<int> opt_strategy_num;
	int maxmax = 0;
	for (int i = 0; i<lines; i++)
		for (int j = 0; j<rows; j++)
			if (m[i][j] >= maxmax)
				maxmax = m[i][j];

	for (int i = 0; i<lines; i++)
		for (int j = 0; j<rows; j++)
			if (m[i][j] = maxmax)
				for (int k = 0; k < opt_strategy_num.size(); k++)
				{
					add = true;
					if (opt_strategy_num[k] == i)
						add = false;
					if (add)
						opt_strategy_num.push_back(i);
				}
	ss << " :" << endl;
	for (int i = 0; i<opt_strategy_num.size(); i++)
		ss << "a" << opt_strategy_num[i] << endl;
	ss << " : " << maxmax << endl;
	return ss.str();
}

std::string LabOptimizations::SolverLab6::Gurvic(Matrix m)
{
	std::stringstream ss;
	int rows = m[0].size(), lines = m.size();
	vector<int> opt_strategy_num;
	int min;
	int max = 0;
	int *mins = new int[lines];
	int *maxs = new int[lines];
	float opt_solve = 0;
	float *solves = new float[lines];

	for (int i = 0; i<lines; i++)
	{
		min = m[i][0];
		for (int j = 0; j<rows; j++)
		{
			if (m[i][j]>max)
				max = m[i][j];
			if (m[i][j]<min)
				min = m[i][j];
		}
		maxs[i] = max;
		mins[i] = min;
		max = 0;
	}

	for (int i = 0; i<lines; i++)
	{
		solves[i] = (0.5 * mins[i]) + (0.5 * maxs[i]);
		if (opt_solve < solves[i])
			opt_solve = solves[i];
	}

	for (int i = 0; i < lines; i++)
		if (solves[i] == opt_solve)
			opt_strategy_num.push_back(i);

	ss << " :" << endl;
	for (int i = 0; i<opt_strategy_num.size(); i++)
		ss << "a" << opt_strategy_num[i] << endl;
	ss << " : " << opt_solve << endl;
	return ss.str();
}

std::string LabOptimizations::SolverLab6::Risks(Matrix m)
{
	std::stringstream ss;
	int rows = m[0].size(), lines = m.size();
	vector<int> opt_strategy_num;
	int max = 0;
	int max_sorrow = 0;
	int opt_sorrow;
	int *sorrows = new int[lines];
	int *maxs = new int[rows];
	Matrix risk_matrix = m;

	for (int i = 0; i < lines; i++) // 
		for (int j = 0; j < rows; j++)
		{
			if (m[i][j]>max)
				max = m[i][j];
			maxs[i] = max;
			max = 0;
		}

	for (int i = 0; i <lines; i++) // 
		for (int j = 0; j < rows; j++)
			risk_matrix[i][j] = maxs[i] - risk_matrix[i][j];

	for (int i = 0; i < lines; i++)
		for (int j = 0; j < rows; j++)
		{
			if (risk_matrix[i][j] > max_sorrow)
				max_sorrow = risk_matrix[i][j];
			sorrows[i] = max_sorrow;
			max_sorrow = 0;
		}

	for (int i = 0; i < lines; i++)
	{
		opt_sorrow = risk_matrix[i][0];
		if (sorrows[i]<opt_sorrow)
			opt_sorrow = sorrows[i];
	}

	for (int i = 0; i < lines; i++)
		if (sorrows[i] == opt_sorrow)
			opt_strategy_num.push_back(i);

	ss << " :" << endl;
	for (int i = 0; i<opt_strategy_num.size(); i++)
		ss << "a" << opt_strategy_num[i] << endl;
	ss<< " : " << opt_sorrow << endl;
	return ss.str();
}