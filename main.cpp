
#pragma warning (disable:4996)
#include <iostream>
#include <conio.h>
#include <iomanip> 
#include <fstream>
#include <Windows.h>
#include <vector>
#include <locale.h>
#include <vector>
#include "Solvers.h"
#include "tree.h"
using namespace LabOptimizations;
using namespace std;


bool check_solution_integer(const SimplexMatrix &a, bool &is_f_integer){
	size_t lines = a.size();

	for (size_t i = 0; i < lines - 1; ++i)
		if (!IsInteger(a[i][0])&&a.IsTargetFunctionParameter(i,true))
			return false;

	is_f_integer = IsInteger(a[lines - 1][0]);
	return true;

}

Labs Matrix_interface(Matrix &a, int &rows, int &lines){
	Labs choice;
	int t;
	bool err_flag = false;
	do {
		std::cout << "Введите номер лабораторой работы:	1-" << (Labs::LAB_MAX + 1) << endl; //индекс с нуля
		cin >> t;
		choice = (Labs)(t - 1); //конвертируем в t
		if ((err_flag = (choice >= Labs::LAB_MAX + 1 || t <= 0)))
			std::cout << "Ошибка, повторите ввод" << endl;
	} while (err_flag);


	std::ifstream f(choice==LAB_5?"in5.txt":choice==LAB_6?"in6.txt":"in.txt");
	f >> lines >> rows;

	//создание матрицы
	//выделяем память
	a.resize(lines);
	//ввод матрицы
	for (int i = 0; i < lines; i++) {
		a[i].resize(rows);
		for (int j = 0; j < rows; j++)
			f >> (a)[i][j];

	}
	


	f.close();
	return choice;
};


// Разделение по дробной переменной   
// возвращает нифига
///@param[in,out] node - узел, в котором происходит разбиение по дробной переменной


bool is_same_SimplexMatrixes(const SimplexMatrix &a, const SimplexMatrix &b){
	/*
	size_t i, j, lines = a.size(), rows;
	if (a.size() != b.size())
		return false;
	if (a.size() && b.size() && a[0].size() != b[0].size())
		return false;
	rows = a[0].size();
	for (i = 0; i < lines; ++i)
	for (j = 0; j < rows; ++j)
	if ( fabs(a[i][j] - b[i][j]) > 0.1)
		return false;

	return true;*/
	return a.size() == b.size() && fabs(a[a.size() - 1][0] - b[b.size() - 1][0] ) < 0.01;
}

//1) считаем симплекс
//2) если симплекс не имеет решения - эта симплекс таблица (а, следовательно, и грань) отсекается
//3) если столбец симплекс-таблицы целочисленный - сравниваем с уже запомненным значением предыдущих вычислений (старыми симплексами)
//4) если нет -> разделяем по дробным переменным и РЕКУРСИЯ! Бум!
static size_t tree_count = 0;
static int king_batka_value = 0;
static bool king_updated = false;

bool recursive_integer_method(SimplexMatrixNode &entryNode, SimplexMatrix& best){
	bool failed = false;
	
	SimplexMatrix before(*entryNode);
	float sm = (*entryNode).SimplexSolve(failed, false, false, true);
	cout << "После симплекс-метода" << ". Метод прошел " << (failed ? "с ошибкой" : "успешно") << std::endl;
	//print_array(*entryNode);
	if (failed)
		return false;
	if ( king_updated && king_batka_value >= floor(sm) )
		return false;
	bool is_integer, f_integer = false;
	//если целое число из симплекса и сами коэф-ты целые, и вообще больше беста, или у беста нецелые коэф-ты или симплекс, то меняем бест на наш
	if ((is_integer = check_solution_integer(*entryNode, f_integer)) && f_integer
		&& (sm > best[best.size() - 1][0] || !check_solution_integer(best, f_integer) || !f_integer)){  //атиц всех условий!
		best = *entryNode;
		king_updated = true;
		king_batka_value = sm;
	}
	tree_count++;
	//сплитуем, парни!!!
	if (!is_integer) {
		if (is_same_SimplexMatrixes(before, *entryNode) && tree_count>1)
			return false;
		(*entryNode).SplitChildren(entryNode);
		size_t l = entryNode.length();
		for (size_t i = 0; i < l; ++i) {
			cout << "Ребенок:> " << i << std::endl;
			///print_array(**entryNode.getChild(i));
			cout << (**entryNode.getChild(i)).toString();
			cout << "----------------------------------" << std::endl;
			if (recursive_integer_method(*entryNode.getChild(i), best))
				is_integer = true;
		}
	}
	
	//возвращаем false, если дробное
	return is_integer;
};
/*
// Plays a specified MIDI file by using MCI_OPEN and MCI_PLAY. Returns 
// as soon as playback begins. The window procedure function for the 
DWORD playMIDIFile(HWND hWndNotify, LPSTR lpszMIDIFileName)
{
	UINT wDeviceID;
	DWORD dwReturn;
	MCI_OPEN_PARMS mciOpenParms;
	MCI_PLAY_PARMS mciPlayParms;
	MCI_STATUS_PARMS mciStatusParms;
	MCI_SEQ_SET_PARMS mciSeqSetParms;

	// Open the device by specifying the device and filename.
	// MCI will attempt to choose the MIDI mapper as the output port.
	mciOpenParms.lpstrDeviceType = "sequencer";
	mciOpenParms.lpstrElementName = lpszMIDIFileName;
	if (dwReturn = mciSendCommand(NULL, MCI_OPEN,
		MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,
		(DWORD)(LPVOID)&mciOpenParms))
	{
		// Failed to open device. Don't close it; just return error.
		return (dwReturn);
	}

	// The device opened successfully; get the device ID.
	wDeviceID = mciOpenParms.wDeviceID;

	// Check if the output port is the MIDI mapper.
	mciStatusParms.dwItem = MCI_SEQ_STATUS_PORT;
	if (dwReturn = mciSendCommand(wDeviceID, MCI_STATUS,
		MCI_STATUS_ITEM, (DWORD)(LPVOID)&mciStatusParms))
	{
		mciSendCommand(wDeviceID, MCI_CLOSE, 0, NULL);
		return (dwReturn);
	}

	mciPlayParms.dwCallback = (DWORD)hWndNotify;
	if (dwReturn = mciSendCommand(wDeviceID, MCI_PLAY, MCI_NOTIFY,
		(DWORD)(LPVOID)&mciPlayParms))
	{
		mciSendCommand(wDeviceID, MCI_CLOSE, 0, NULL);
		return (dwReturn);
	}

	return (0L);
} */
// specified window will be notified when playback is complete. 
// Returns 0L on success; otherwise, it returns an MCI error code.

std::string toStringSimplexMatrix( const SimplexMatrix& mat ){
	std::stringstream ss;
	bool dummy = false, is_int = check_solution_integer(mat, dummy);
	ss << std::setprecision(2);
	float f = mat[mat.size() - 1][0];
	ss << "[ Симплекс-таблица F = " << f << " Дробные коэф-ты?:  " << (!is_int ? "y" : "n") <<
																 " Дробное решение?:  " << (!dummy ? "y" : "n") << "]";
	if (is_int){
		ss << std::endl;
		ss << mat.toString();
	}
	return ss.str();
}
std::string(*TreeNode<SimplexMatrix>::tsf) ( const SimplexMatrix& ) = toStringSimplexMatrix;


void main() {
	setlocale(LC_ALL, "Russian");
	//playMIDIFile(0, "C:\\Users\\Jerry\\Desktop\\opt lab fun\\SimplexLab.1-3\\gameofthrones.mid");
	int rows, lines;
	bool failed = false; //есть ли решение
	Matrix a_template;
	SimplexMatrix a;
	Labs choice = Matrix_interface(a_template, rows, lines);
	a = a_template;
	//копирование
	SimplexMatrix a_s = a;

	for (int j = 0; j < rows; j++)
		a_s[lines - 1][j] *= -1;
	switch (choice){
	case LAB_1:
	{
		cout << " Формулировка задачи ЛП" << endl;
		cout << a_s.toString();
		float r1 = a_s.SimplexSolve(failed, true, true, false);
		cout << "Решение задачи ЛП: " << r1 << std::endl;
	}
	break;
	case LAB_2:
	{
		cout << "Формулировка прямой задачи:";
		cout << a_s.toString();
		float r1 = a_s.SimplexSolve(failed, false, true, false);

		SimplexMatrix b = a.SimplexInvert();
		cout << "Формулировка Двойственной задачи: " << endl;
		cout << b.toString();
		float r2 = -b.SimplexSolve(failed, false, true, false);
		cout << "Двойственная задача: " << r2 << endl;
		if (abs(r1 - r2) < 0.01)
			cout << "Решение двойственной задачи равно решению прямой задачи" << std::endl;
		else
			cout << "Решение прямой задачи и двойственной не совпадают: " << r1 << "!=" << r2 << std::endl;
	}
	break;
	case LAB_3:
	{
		cout << "Решение начальной матрицы:";
		cout << a_s.toString();
		float r1 = a_s.SimplexSolve(failed, false, false, true);
		bool needed = true;
		bool  is_f_integer = false, is_integer = false;
		SimplexMatrixNode parent(a_s);
		SimplexMatrix best_match = a_s;
		float best_match_simplex;
		//best_match - лучшее решение, на данный момент.

		if ( (is_integer = check_solution_integer(a_s, is_f_integer)) && is_f_integer)
			best_match_simplex = r1;
		else {
			//РИКККУУУУУРССИИИИИЯЯЯЯ
			failed = !recursive_integer_method(parent, best_match);
			best_match_simplex = best_match[best_match.size() - 1][0];
		}


		//выводим результат
		if (!failed) {
			cout << "Дерево симплексов:" << std::endl;
			cout << parent.toString(0);

			best_match.toString();
			cout << endl << "----------------------------------------------------------------------------"
				<< endl << "----------------------------------------------------------------------------"
				<< endl << "Решение целочисленной задачи: " << best_match_simplex << endl;

		}
		else
			cout << "Целочисленная задача не имеет решений!" << endl;
		cout << "Колличество бастардов: " << tree_count << endl;
		parent.clear();
	}
	break;
	
	case LAB_5:
	{
		bool is_failed = true;
		SimplexMatrix b = a.SimplexInvert();

		cout << "Исходная симплекс-таблица игры для игрока А:" << endl;
		cout << a_s.toString();
		cout << "Симплекс-таблица игры для игрока B: " << endl;
		cout << b.toString();
		float r1 = a_s.SimplexSolve(is_failed, false, false, true);
		float r2;
		if (!is_failed)
			r2 = -b.SimplexSolve(is_failed, false, false, true);
		else
			cout << "Расчет оптимального решения для игрока А был завершен с ошибкой." << std::endl;
		if (!is_failed){
			//g - минимальный выигрыш первого (цена игры). h - минимальный проигрыш второго (цена игры)
			float g = 1 / r1, h = 1 / r2;
			//ui = xi/g(x), nui = yi/h(y)
			std::vector<float> u, nu;
			u.assign(a_s[0].size()-1, 0);
			nu.assign(a_s.size()-1, 0);
			auto lambda_assign_probabilities = [](const SimplexMatrix &input_matrix, std::vector<float> &output_vector)->void {
				for (size_t i = 0, s = input_matrix.size() - 1; i < s; ++i)
					if (input_matrix.IsTargetFunctionParameter(i, true))
						output_vector[input_matrix.LabelAtLine(i) - 1] = input_matrix[i][0];
			};
			auto lambda_print_strategies = [](const std::vector<float> &vars, const float price, const std::string &header, const char prefix)->void {
				cout << header << std::endl;
				for (size_t i = 0, s = vars.size(); i < s; i++)
					cout << prefix << i + 1 << "=" << vars[i] * price << " ";
				cout << std::endl;
			};
			lambda_assign_probabilities(a_s, u);
			lambda_print_strategies(u, g, "Для игрока А оптимальной смешанной стратегией является: ", 'x');
			lambda_assign_probabilities(b, nu);
			lambda_print_strategies(nu, h, "Для игрока B оптимальной смешанной стратегией является: ", 'y');
			cout << "Примечание - для игрока B стратегии нихрена не правильные. ЛОЛЛОЛОЛОЛ" << std::endl;
		}
		else
			cout << "Симплекс метод решен с ошибкой." << endl;

	}
		break;
	case LAB_6:
	{
		SolverLab6 solver;
		solver.Input(a);
		solver.Solve();
		cout << solver.Output();
	}
		break;
	default:
		break;
	}
	getch();
}
