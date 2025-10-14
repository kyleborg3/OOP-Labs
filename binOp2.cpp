#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>

class BinOp {
private:
    double operand1;
    char oprtr;
    double operand2;

public:
    //Default constructor
    BinOp(double a = 0, char oper = '+', double b = 0) : operand1(a), oprtr(oper), operand2(b) {}

    //Setters to be used for received operands and operator
    void setOperands(double a, double b) { operand1 = a; operand2 = b; }
    void setOperator(char oper) {oprtr = oper;}

    //Getters to be used for received operands and operator
    double getOperand1() const {return operand1;}
    double getOperand2() const {return operand2;}
    char getOperator() const {return oprtr;}

    //Evaluate the expression for all operator cases
    double evaluate() const {
        switch(oprtr) {
            case '+': return operand1 + operand2;
            case '-': return operand1 - operand2;
            case '*': return operand1 * operand2;
            case '/':
                if(operand2 == 0) throw std::runtime_error("Division by zero"); //Undefined
                return operand1 / operand2;
            default: throw std::runtime_error("Invalid operator"); //Not +, -, *, / so raise error. 
        }
    }
};


// Running the code
int main() {

    const int MAX_EXPRESSIONS = 5;
    BinOp expressions[MAX_EXPRESSIONS];
    int count = 0;

    std::string line;
    std::cout << "Enter up to " << MAX_EXPRESSIONS << " expressions. Enter 'q' to quit.\n";

    while (count < MAX_EXPRESSIONS) {
        std::cout << "Expression " << (count + 1) << ": ";
        std::getline(std::cin, line);

        if (line == "q" || line == "Q") break; //Quit
        std::istringstream iss(line);
        double a, b;
        char op;
        if (!(iss >> a >> op >> b)) { //Structure has to be  a op b
            std::cout << "Invalid input. Try again.\n";
            continue;
        }
        try { //Validation
            BinOp temp(a, op, b);
            temp.evaluate(); //Test for errors
            expressions[count++] = temp;
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    //Outputting results to file
    std::ofstream fout("results.txt");
    for (int i = 0; i < count; ++i) {
        try {
            double result = expressions[i].evaluate();

            //Writing to file
            fout << expressions[i].getOperand1() << " "
                 << expressions[i].getOperator() << " "
                 << expressions[i].getOperand2() << " -> "
                 << result << std::endl;

        } catch (const std::exception& e) {
            fout << "Error evaluating expression: " << e.what() << std::endl;
        }
    }
    fout.close();
    std::cout << "Results written to results.txt\n";
    return 0;
    
}