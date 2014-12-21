#include <sstream>
#include <string>
#include <iomanip>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <vector>
#include "tree.h"

using namespace std;

namespace LabOptimizations {
	//TYPEDEFS AND FORWARD DECLARATIONS
	typedef TreeNode<float**> FloatTreeNode;
	typedef std::vector<std::vector<float>> Matrix;
	class SimplexMatrix;
	typedef TreeNode<SimplexMatrix> SimplexMatrixNode;

	//ENUMS

	enum Labs
	{
		LAB_1,
		LAB_2,
		LAB_3,
		LAB_4,
		LAB_5,
		LAB_MAX = LAB_5
	};

	enum SolutionStatus {
		SS_NONE = 0x00,
		SS_INTEGER_FUNCTION_VALUE = 0x10,
		SS_INTEGER_BASE_COEFFICIENTS = 0x20,
	};
	//PUBLIC UTILS FUNCTIONS

	
	
	bool CloseToZero(double a);
	//is_f_integer - является ли f - целым
	bool IsInteger(float a);
	//возвращает тру, если весь первый столбец - целочисленный

	//CLASSES

	class SimplexMatrix : public Matrix {
		int max_x;
		
		std::vector<size_t> line_labels;
		std::vector<size_t> row_labels;
	protected:
		void addLineLabel();
		void addRowLabel();
		void genLabels();
		void swapLabels(size_t line, size_t row);
		
	public:
		//базовый оверрайд
		SimplexMatrix(const Matrix& m);
		SimplexMatrix(const SimplexMatrix& m);
		SimplexMatrix();
		SimplexMatrix& operator = (const Matrix& m);
		//В строку
		std::string toString() const;
		//Дефолтный конструктор - не нужно самим ничего удалять
		//ПУБЛИЧНЫЕ МЕТОДЫ
		//Копирует надписи из другой матрицы
		void addLabelsFrom(const SimplexMatrix& sm);
		//Инвертирует матрицу
		SimplexMatrix SimplexInvert() const;
		//Делит по первой целевой дробной переменной
		//true - если есть, что делить (поделил). false - если нет.
		bool SplitChildren(SimplexMatrixNode &node);
		//решает матрицу. Осторожней с выбором to_min. Может войти в бесконечный цикл (ибо может быть, что решения нет)
		float SimplexSolve(bool &is_failed, bool to_min, bool show_progress = true, bool show_final_tab = true);
		//проверяет, входит ли переменная в целевую функцию.
		//переменная должна лежать либо в нулевом столбце, либо в последней строке
		//параметр is_line регулирует, по чему ведется проверка - по строке или по столбцу
		//параметр index задает, собственно, индекс из таблицы той переменной, которая проверяется
		bool IsTargetFunctionParameter(size_t index, bool is_line = true) const{
			int r = (is_line ? this->line_labels[index] : this->row_labels[index] );
			return r < this->row_labels.size();
		}
		size_t LabelAtLine(size_t line) const{
			return this->line_labels[line];
		}
		size_t LabelAtRow(size_t row) const{
			return this->row_labels[row];
		}
	};

	//интерфейс для всех решений
	class SolverBase {
	public:
		virtual void Solve() = 0;
		virtual std::string Output() = 0;
	};

	// SOLVERS FOR SPECIFIC LAB
	class SolverLab1 : SolverBase {
		SimplexMatrix sm;
		bool is_failed;
		bool is_solved;
	public:
		SolverLab1();

		void Input(SimplexMatrix &sm);

		virtual std::string Output();
		virtual void Solve();
	};

	class SolverLab3 : SolverBase {

	};

}















