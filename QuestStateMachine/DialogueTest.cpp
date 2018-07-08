#include <functional>
#include <UniversalException/CustomTerminate.hpp>
#include <QuestStateMachine/QuestState.hpp>
#include <QuestStateMachine/QuestStateMachine.hpp>

class DialogueState : public QuestState
{
public:
    using InputCallback = std::function <bool (int&)>;

    DialogueState (int stateCode, const std::string &toPrint, const InputCallback &callback) :
            QuestState (stateCode, ""),
            toPrint_ (toPrint),
            callback_ (callback)
    {

    }

    virtual ~DialogueState () = default;

    virtual void OnEnter ()
    {
        QuestState::OnEnter ();
        std::cout << toPrint_;
    }

    virtual void OnUpdate (float timeStep)
    {
        int output;
        if (callback_ (output))
        {
            SetMustExit (true, output);
        }
    }

private:
    std::string toPrint_;
    InputCallback callback_;
};

enum StateCodes
{
    GREET = 0,
    CALC_MAIN,
    CALC_INPUT_NUMBER,
    CALC_INPUT_OPERATOR,
    CALC_CALCULATE,
    GOODBYE
};

int main ()
{
    std::set_terminate (CustomTerminate);
    char lastSuccessfulOperator = '+';
    int firstNumber = INT_MAX;
    int secondNumber = INT_MAX;

    QuestStateMachine machine;
    machine.AddState (new DialogueState (GREET, "What's your name?\n",
            [] (int &output) -> bool
            {
                std::string name;
                std::cin >> name;
                std::cout << "Hello, " << name << "!" << std::endl;

                output = CALC_MAIN;
                std::cin.clear ();
                while (std::cin.get () != '\n') {}
                return true;
            }));

    machine.AddState (new DialogueState (CALC_MAIN, "Do more calculations? (y/n) ",
            [] (int &output) -> bool
            {
                char answer;
                std::cin >> answer;

                output = answer == 'y' ? CALC_INPUT_NUMBER : GOODBYE;
                std::cin.clear ();
                while (std::cin.get () != '\n') {}
                return true;
            }));

    machine.AddState (new DialogueState (CALC_INPUT_NUMBER, "Input integer number: ",
            [&firstNumber, &secondNumber] (int &output) -> bool
            {
                int number;
                if (!(std::cin >> number))
                {
                    std::cout << "Incorrect input, try again!" << std::endl;
                    output = CALC_INPUT_NUMBER;
                }
                else
                {
                    if (firstNumber == secondNumber && firstNumber == INT_MAX)
                    {
                        firstNumber = number;
                        output = CALC_INPUT_OPERATOR;
                    }
                    else
                    {
                        secondNumber = number;
                        output = CALC_CALCULATE;
                    }
                }

                std::cin.clear ();
                while (std::cin.get () != '\n') {}
                return true;
            }));

    machine.AddState (new DialogueState (CALC_INPUT_OPERATOR, "Input operator: ",
            [&lastSuccessfulOperator] (int &output) -> bool
            {
                char input;
                std::cin >> input;

                if (input == '+' || input == '-' || input == '*' || input == '/')
                {
                    lastSuccessfulOperator = input;
                    output = CALC_INPUT_NUMBER;
                }
                else
                {
                    std::cout << "Unknown operator, try again!" << std::endl;
                    output = CALC_INPUT_OPERATOR;
                }

                std::cin.clear ();
                while (std::cin.get () != '\n') {}
                return true;
            }));

    machine.AddState (new DialogueState (CALC_CALCULATE, "Result: ",
            [&firstNumber, &secondNumber, &lastSuccessfulOperator] (int &output) -> bool
            {
                try
                {
                    switch (lastSuccessfulOperator)
                    {
                        case '+':
                            std::cout << firstNumber + secondNumber << '.' << std::endl;
                            break;
                        case '-':
                            std::cout << firstNumber - secondNumber << '.' << std::endl;
                            break;
                        case '*':
                            std::cout << firstNumber * secondNumber << '.' << std::endl;
                            break;
                        case '/':
                            std::cout << firstNumber / secondNumber << '.' << std::endl;
                            break;
                        default:
                            std::cout << "?" << std::endl;
                    }
                }

                catch (...)
                {
                    std::cout << "?" << std::endl;
                }

                firstNumber = INT_MAX;
                secondNumber = INT_MAX;
                output = CALC_MAIN;
                return true;
            }));

    machine.AddState (new DialogueState (GOODBYE, "Goodbye!\n",
            [] (int &output) -> bool
            {
                std::cin.get ();
                output = QuestStateMachine::EXIT_CODE;
                return true;
            }));

    machine.Start (GREET);
    while (machine.GetCurrentState () != nullptr)
    {
        machine.Update (1.0f / 60.0f);
    }
    return 0;
}
