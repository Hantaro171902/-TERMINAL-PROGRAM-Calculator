#include <iostream>
#include <cmath>

using namespace std;

void clearTerminal() {
    cout << "\033[2J\033[1;1H";
}

void show_calculator()
{
    
        cout << "   _____________________\n";
        cout << " /|  _________________  |\n";
        cout << "| | |               0 | |\n";
        cout << "| | |_________________| |\n";
        cout << "| |  ___ ___ ___   ___  |\n";
        cout << "| | | 7 | 8 | 9 | | + | |\n";
        cout << "| | |___|___|___| |___| |\n";
        cout << "| | | 4 | 5 | 6 | | - | |\n";
        cout << "| | |___|___|___| |___| |\n";
        cout << "| | | 1 | 2 | 3 | | x | |\n";
        cout << "| | |___|___|___| |___| |\n";
        cout << "| | | . | 0 | = | | / | |\n";
        cout << "| | |___|___|___| |___| |\n";
        cout << "| |_____________________|\n";
        cout << "|/_____________________/\n";


}


void display_output(double answer)
{

        cout << "   _____________________\n";
        cout << " /|  _________________  |\n";
        cout << "| | |              " << answer <<" | |\n";
        cout << "| | |_________________| |\n";
        cout << "| |  ___ ___ ___   ___  |\n";
        cout << "| | | 7 | 8 | 9 | | + | |\n";
        cout << "| | |___|___|___| |___| |\n";
        cout << "| | | 4 | 5 | 6 | | - | |\n";
        cout << "| | |___|___|___| |___| |\n";
        cout << "| | | 1 | 2 | 3 | | x | |\n";
        cout << "| | |___|___|___| |___| |\n";
        cout << "| | | . | 0 | = | | / | |\n";
        cout << "| | |___|___|___| |___| |\n";
        cout << "| |_____________________|\n";
        cout << "|/_____________________/\n";


}
int main()
{
    double num1 = 0.0;
    double num2 = 0.0;

    double answer = 0.0;
    char inp = '\n';
    bool converter = 0;
    while(inp != '.')
    {
        if(converter == 1)//if we want to display output
        {//we will take only one input
            display_output(answer);
            cout<<"Enter: ";
            cin>>inp;
            if(inp == '+' || inp == '-' || inp == '*' || inp == '/' || inp == 'm' || inp == 'p')
            {
                num1 = answer;

                cout<<"Enter num 2: ";
                cin>>num2;
            }
            else if(inp == 'k' || inp== '.')
            {
                //just don't take any input
            }
            else
            {
                num1 = answer;
            }

        }
        else// if we want to show basic calculator
        {// we will take two inputs
            show_calculator();
            cout<<"Enter: ";
            cin>>inp;
            if(inp == '+' || inp == '-' || inp == '*' || inp == '/' || inp == 'm' || inp == 'p')
            {
                cout<<"Enter num 1: ";
                cin>> num1;

                cout<<"Enter num 2: ";
                cin>>num2;
            }
            else if(inp == 'k' || inp== '.')
            {
                //just don't take any input
            }
            else
            {
                cout<<"Enter num 1: ";
                cin>>num1;
            }

            converter = 1;

        }

        switch(inp)
        {
        case '+':
            answer = num1 + num2;
            break;
        case '-':
            answer = num1 - num2;
            break;
        case '*':
            answer = num1 * num2;
            break;
        case '/':
            answer = num1 / num2;
            break;
        case 'm':
            answer =   int(num1)  % int(num2);
            break;
        case 'L':
            answer = log(num1);
            break;
        case 'S':
            answer = num1*num1;
            break;
        case 'C':
            answer = num1*num1*num1;
            break;
        case 'p':
            answer = pow(num1,num2);
            break;
        case 's':
            answer = sin(num1);
            break;
        case 'c':
            answer = cos(num1);
            break;
        case 't':
            answer = tan(num1);
            break;
        case 'r':
            answer = sqrt(num1);
            break;
        case 'k':
            converter = 0;
            break;
        case '.':
            break;
        default:
            cout<<"~~~~You Entered Invalid Value!~~~~ "<<endl;
        }

        // system("cls");
        clearTerminal();

    }
    return 0;
}