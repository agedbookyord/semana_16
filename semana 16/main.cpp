#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

struct Instruction {
    string result;
    string op1;
    string op;
    string op2;
};

class IntermediateCode {
private:
    int tempCount = 1;
    vector<Instruction> code;
    vector<Instruction> optimized;

    string newTemp() {
        return "t" + to_string(tempCount++);
    }

    bool isNumber(const string& s) {
        for (char c : s)
            if (!isdigit(c)) return false;
        return true;
    }

    string calculate(const string& op1, const string& op, const string& op2) {
        if (!isNumber(op1) || !isNumber(op2)) return "";
        int a = stoi(op1), b = stoi(op2);
        int result = 0;
        if (op == "+") result = a + b;
        else if (op == "-") result = a - b;
        else if (op == "*") result = a * b;
        else if (op == "/" && b != 0) result = a / b;
        else return "";
        return to_string(result);
    }

public:
    void generate(const string& left, const string& op1, const string& op, const string& op2) {
        string temp = newTemp();
        code.push_back({temp, op1, op, op2});
        code.push_back({left, temp, "", ""});
    }

    void optimize() {
        for (auto& instr : code) {
            if (!instr.op.empty()) {
                string folded = calculate(instr.op1, instr.op, instr.op2);
                if (!folded.empty()) {
                    optimized.push_back({instr.result, folded, "", ""});
                } else if (instr.op == "*" && instr.op2 == "1") {
                    optimized.push_back({instr.result, instr.op1, "", ""});
                } else if (instr.op == "+" && instr.op2 == "0") {
                    optimized.push_back({instr.result, instr.op1, "", ""});
                } else {
                    optimized.push_back(instr);
                }
            } else {
                optimized.push_back(instr);
            }
        }
    }

    void showTable(const vector<Instruction>& list, const string& title) {
        cout << "\n========== " << title << " ==========\n";
        cout << left << setw(5) << "#"
             << setw(12) << "RESULT"
             << setw(12) << "OP1"
             << setw(8) << "OP"
             << setw(12) << "OP2" << endl;
        cout << string(45, '-') << endl;

        int i = 1;
        for (auto& instr : list) {
            cout << setw(5) << i++
                 << setw(12) << instr.result
                 << setw(12) << instr.op1
                 << setw(8) << instr.op
                 << setw(12) << instr.op2 << endl;
        }
        cout << string(45, '-') << "\n";
    }

    void showResults() {
        showTable(code, "CÓDIGO INTERMEDIO (TAC)");
        showTable(optimized, "CÓDIGO OPTIMIZADO");
    }
};

int main() {
    IntermediateCode gen;

    // Ejemplos de expresiones para generar código
    gen.generate("x", "2", "*", "3");
    gen.generate("y", "a", "*", "1");
    gen.generate("z", "b", "+", "0");
    gen.generate("w", "a", "+", "b");

    gen.optimize();
    gen.showResults();
    return 0;
}