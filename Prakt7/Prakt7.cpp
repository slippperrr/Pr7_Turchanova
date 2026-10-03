// Prakt7.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

#define DEBUG_MODE

using namespace std;
class Task
{
private:
    int number; //номер задачи;
    string title; //название;                  
    string description; //описание;
    string executor; //исполнитель;
    int priority; //приоритет;
    string status; //статус;
public:
    Task()
    {
        number = 0;
        title = "";
        description = "";
        executor = "";
        priority = 0;
        status = "";
    }
    Task(int Tnumber,string Ttitle,string Tdescription,string Texecutor,int Tpriority,string Tstatus)
    {
        number = Tnumber;
        title = Ttitle;
        description = Tdescription;
        executor = Texecutor;
        priority = Tpriority;
        status = Tstatus;
    }

    int getNumber() { return number; }
    string getTitle() { return title; }
    string getDescription() { return description; }
    string getExecutor() { return executor; }
    int getPriority() { return priority; }
    string getStatus() { return status; }

    void setNumber(int newNumber)
    {
        number = newNumber;
    }

    void setTitle(string newTitle)
    {
        title = newTitle;
    }

    void setDescription(string newDescription)
    {
        description = newDescription;
    }

    void setExecutor(string newExecutor)
    {
        executor = newExecutor;
    }

    void setPriority(int newPriority)
    {
        priority = newPriority;
    }

    void setStatus(string newStatus)
    {
        status = newStatus;
    }

    void printInfo()
    {
        cout << "Номер: " << number << endl;
        cout << "Название: " << title << endl;
        cout << "Описание: " << description << endl;
        cout << "Исполнитель: " << executor << endl;
        cout << "Приоритет: " << priority << endl;
        cout << "Статус: " << status << endl;
    }

    string toFileString() 
    {
        return to_string(number) + ";" + title + ";" + description + ";" + executor + ";" + to_string(priority) + ";" +  status;
    }
};

void loadTasks(Task tasks[], int& count, string fileName)
{
    ifstream file(fileName);

    if (!file.is_open())
    {
        throw runtime_error("Не удалось открыть файл: " + fileName);
    }

    string line;
    count = 0;

    while (getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        string fields[6]; //массив полей
        int fieldCount = 0; // заполненые поля
        size_t start = 0; //позиция начала

        while (fieldCount < 6)
        {
            size_t position = line.find(';', start);

            if (position == string::npos)
            {
                fields[fieldCount] = line.substr(start);
                fieldCount++;
                break;
            }

            fields[fieldCount] = line.substr(start, position - start);
            fieldCount++;
            start = position + 1;
        }

        if (fieldCount != 6)
        {
            throw runtime_error("В строке недостаточно полей: " + line);
        }

        int number;
        int priority;

        try
        {
            number = stoi(fields[0]);
            priority = stoi(fields[4]);
        }
        catch (const invalid_argument& e)
        {
            throw runtime_error("Ошибка числового формата в строке: " + line);
        }

        if (count >= 100)
        {
            throw runtime_error("Слишком много задач. Максимум: 100.");
        }

        tasks[count] = Task(
            number,
            fields[1],
            fields[2],
            fields[3],
            priority,
            fields[5]
        );

        count++;
    }

    file.close();
    #ifdef DEBUG_MODE
        cout << "Загружено записей: " << count << endl;
    #endif
}

int findTask(Task tasks[], int count, int number)
{
    for (int i = 0; i < count; i++)
    {
        if (tasks[i].getNumber() == number)
        {
            return i;
        }
    }

    return -1;
}
int main()
{
    setlocale(NULL, "Russian");

    Task tasks[100];
    int count = 0;

    try
    {
        loadTasks(tasks, count, "tasks.txt");

        cout << "Загружено задач: " << count << "\n";

        int choice = 0;

        while (choice != 7)
        {
            cout << "1. Поиск задачи\n";
            cout << "2. Поиск по части названия\n";
            cout << "3. Вывод задач определённого исполнителя\n";
            cout << "4. Фильтрация по статусу и приоритету\n";
            cout << "5. Изменение статуса задачи\n";
            cout << "6. Сохранение файла\n";
            cout << "7. Выход\n";
            cout << "Выберите действие: ";
            cin >> choice;

            // поиск задачи
            if (choice == 1)
            {
                int number;
                cout << "\nВведите номер задачи: ";
                cin >> number;

                int index = findTask(tasks, count, number);
                if (index != -1)
                {
                    cout << "\nЗадача найдена:\n";
                    tasks[index].printInfo();
                }
                else
                {
                    cout << "\nЗадача с таким номером не найдена.\n";
                }
            }

            // поиск по части
            else if (choice == 2)
            {
                string part;
                bool found = false;

                cout << "\nВведите часть названия: ";
                cin >> part;

                for (int i = 0; i < count; i++)
                {
                    if (tasks[i].getTitle().find(part) != string::npos)
                    {
                        tasks[i].printInfo();
                        cout << "\n";
                        found = true;
                    }
                }

                if (!found)
                {
                    cout << "Задач с таким названием не найдено.\n";
                }
            }

            // опр исполнитель
            else if (choice == 3)
            {
                string executor;
                bool found = false;

                cout << "\nВведите имя исполнителя: ";
                cin >> executor;

                for (int i = 0; i < count; i++)
                {
                    if (tasks[i].getExecutor() == executor)
                    {
                        tasks[i].printInfo();
                        cout << "\n";
                        found = true;
                    }
                }

                if (!found)
                {
                    cout << "Задач такого исполнителя не найдено.\n";
                }
            }

            // фильтр
            else if (choice == 4)
            {
                string status;
                int priority;
                bool found = false;

                cout << "\nВведите статус: ";
                cin >> status;

                cout << "Введите приоритет: ";
                cin >> priority;

                for (int i = 0; i < count; i++)
                {
                    if (tasks[i].getStatus() == status &&
                        tasks[i].getPriority() == priority)
                    {
                        tasks[i].printInfo();
                        cout << "\n";
                        found = true;
                    }
                }

                if (!found)
                {
                    cout << "Подходящих задач не найдено.\n";
                }
            }

            // изм статус
            else if (choice == 5)
            {
                int number;
                string newStatus;

                cout << "\nВведите номер задачи: ";
                cin >> number;

                int index = findTask(tasks, count, number);

                if (index != -1)
                {
                    cout << "Введите новый статус: ";
                    cin >> newStatus;

                    tasks[index].setStatus(newStatus);

                    cout << "Статус задачи изменён.\n";

                    tasks[index].printInfo();
                }
                else
                {
                    cout << "Задача с таким номером не найдена.\n";
                }
            }

            // сохр
            else if (choice == 6)
            {
                ofstream file("tasks.txt");

                if (!file.is_open())
                {
                    throw runtime_error("Не удалось открыть файл для сохранения.");
                }

                for (int i = 0; i < count; i++)
                {
                    file << tasks[i].toFileString() << endl;
                }

                file.close();

                cout << "Данные успешно сохранены в tasks.txt\n";

                #ifdef DEBUG_MODE
                    cout << "Сохранено записей: " << count << endl;
                #endif
            }
            
            else if (choice != 7)
            {
                cout << "Такого пункта меню нет.\n";
            }
           
        }
    }
    catch (const invalid_argument& e)
    {
        cout << "Ошибка: " << e.what() << endl;
    }
    catch (const exception& e)
    {
        cout << "Ошибка: " << e.what() << endl;
    }
}
