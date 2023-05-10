#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <windows.h>
using namespace std;


struct Task 
{
    string name;
    int priority;
    int burstTime;
    int memoryUsage;
    int cpuUsage;
    bool completed;
    bool isProgram;
    int duration;
    int network_usage;

};
// This struct defines a Task with various properties such as name, priority, burst time, memory usage, CPU usage, 
//completion status, whether it is a program or not, duration, and network usage.

class Node 
{

public:

    Task task;
    Node* next;
    Node* prev;

    Node(Task t) : task(t), next(nullptr), prev(nullptr) {}

};
//This is a class definition for a doubly linked list node. 
//It has a Task object to hold the task data, as well as two pointers: one to the next node in the list (next) and another to the previous node (prev). 
//The constructor initializes the node's task and sets the next and prev pointers to nullptr.

class DoublyLinkedList 
{

public:

    Node* head;
    Node* tail;
    int size;

    DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    ~DoublyLinkedList() 
    {
        Node* current = head;
        while (current != nullptr) 
        {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }
    void addNode(Task t) 
    {
        Node* newNode = new Node(t);

        if (head == nullptr) 
        {
            head = newNode;
            tail = newNode;
        }
        else 
        {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }

        size++;
    }

    void removeNode(Node* node) 
    {

        if (node == head) 
        {
            head = node->next;
            if (head != nullptr) 
            {
                head->prev = nullptr;
            }
        }

        else if (node == tail) 
        {
            tail = node->prev;
            if (tail != nullptr) 
            {
                tail->next = nullptr;
            }
        }
        else 
        {
            node->prev->next = node->next;
            node->next->prev = node->prev;
        }

        delete node;
        size--;
    }

    void displayNodes() 
    {
        if (size == 0) 
        {
            cout << "No tasks to display." << endl;
            return;
        }

        cout << "Task\tPriority\tBurst Time\tMemory Usage\tCPU Usage\tCompleted\tTotal Network Usage:" << endl;
        Node* current = head;

        while (current != nullptr) 
        {
            Task t = current->task;
            cout << t.name << "\t" << t.priority << "\t\t" << t.burstTime << "\t\t"
                 << t.memoryUsage << "\t\t" << t.cpuUsage << "\t\t";

            if (t.completed) 
            {
                cout << "Yes";
            }
            else 
            {
                cout << "No";
            }

            cout << endl;
            current = current->next;
        }

        int total_network_usage = 0;

        for (Node* current = head; current != nullptr; current = current->next) 
        {
            total_network_usage += current->task.network_usage;
        }

        cout << total_network_usage << " MB" << endl;
    }
};

vector<Task> tasks;
DoublyLinkedList taskList;

class TaskManagerBase 
{
public:
    virtual void run() = 0;
};

class TaskManagerException : public std::exception 
{
    const char* message;

public:

    TaskManagerException(const char* msg) : message(msg) {}

    const char* what() const noexcept override 
    {
        return message;
    }
};

template<typename Compare>
void sortTasks(Compare comp) 
{
    if (tasks.empty()) 
    {
        cout << "No tasks to sort." << endl;
        return;
    }

    sort(tasks.begin(), tasks.end(), comp);

    cout << "Tasks sorted successfully." << endl;
}

template<typename Function>
void modifyTask(Function f, const string& taskName) 
{
    for (Task& t : tasks) 
    {
        if (t.name == taskName) 
        {
            f(t);
            return;
        }
    }
    cout << "Task not found." << endl;
}

class TaskManager : public TaskManagerBase 
{
public:
void run() override;
    
void addTask() 
{
    try
    {
     
    Task t;

    cout << "Enter task name: ";
    cin >> t.name;

    cout << "Enter task priority (1-10): ";
    cin >> t.priority;

    cout << "Enter task burst time: ";
    cin >> t.burstTime;

    cout << "Enter task memory usage: ";
    cin >> t.memoryUsage;

    cout << "Enter task CPU usage: ";
    cin >> t.cpuUsage;

    cout << "Enter task duration (in minutes): ";
    cin >> t.duration;

    cout << "Enter task network usage (in MB): ";
    cin >> t.network_usage;

    t.completed = false;

    tasks.push_back(t);

    cout << "Task added successfully!" << endl;

    // Launch the program if its name is entered as a task
    if (t.name.find(".exe") != string::npos) 
    {
        cout << "Launching program: " << t.name << endl;
        system(t.name.c_str());
        t.isProgram = true;  // set flag to true if task is a program
    }
    }
    
    catch (const exception& ex) 
    {
        throw TaskManagerException(ex.what());
    }

}

void displayTasks() 
{
    if (tasks.empty()) 
    {
        cout << "No tasks to display." << endl;
        return;
    }

    cout << "Task\tPriority\tBurst Time\tMemory Usage\tCPU Usage\tCompleted\tTotal Network Usage:" << endl;

    for (Task t : tasks) 
    {
        cout << t.name << "\t" << t.priority << "\t\t" << t.burstTime << "\t\t"
             << t.memoryUsage << "\t\t" << t.cpuUsage << "\t\t";

        if (t.completed) 
        {
            cout << "Yes";
        }
        else 
        {
            cout << "No";
        }

        cout << endl;
    }

    int total_network_usage = 0;

    for (int i = 0; i < tasks.size(); i++) 
    {
        total_network_usage += tasks[i].network_usage;
    }

    cout  << total_network_usage << " MB" << endl;

}


void completeTask() 
{
    string taskName;

    cout << "Enter the name of the task to mark as completed: ";
    cin >> taskName;

    for (Task& t : tasks) 
    {
        if (t.name == taskName) 
        {
            t.completed = true;
            cout << "Task marked as completed successfully." << endl;

            // close the opened task if it was completed
            if (t.name == "chrome.exe") 
            {
                system("taskkill /f /im chrome.exe");
                cout << "Chrome task killed successfully." << endl;
            }

            return;
        }
    }

    cout << "Task not found." << endl;
}


void killTask() 
{
    string name;
    cout << "Enter task name to kill: ";
    cin >> name;

    string command = "taskkill /f /im " + name;
    int result = system(command.c_str());

    if (result == 0) 
    {
        cout << "Task killed successfully." << endl;
    } 
    else 
    {
        cout << "Error: Unable to kill task." << endl;
    }
}


void displayTaskDetails(string taskName) 
{
    for (Task t : tasks) 
    {
        if (t.name == taskName) 
        {
            cout << "Task Details:" << endl;
            cout << "Name: " << t.name << endl;
            cout << "Priority: " << t.priority << endl;
            cout << "Burst Time: " << t.burstTime << endl;
            cout << "Memory Usage: " << t.memoryUsage << "%" << endl;
            cout << "CPU Usage: " << t.cpuUsage << "%" << endl;
            return;
        }
    }

    cout << "Task not found." << endl;
}


void fcfs() 
{
    if (tasks.empty()) 
    {
        cout << "No tasks to sort." << endl;
        return;
    }

    sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) 
    {
        return a.burstTime < b.burstTime;
    });

    cout << "Tasks sorted successfully (FCFS)." << endl;
}


void sjf() 
{
    if (tasks.empty()) 
    {
        cout << "No tasks to sort." << endl;
        return;
    }

    sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) 
    {
        return a.burstTime < b.burstTime && a.priority < b.priority;
    });

    cout << "Tasks sorted successfully (SJF)." << endl;
}


void show_network_usage() 
{
    if (tasks.empty()) 
    {
        cout << "No tasks added yet!" << endl;
        return;
    }
    int total_network_usage = 0;
    for (int i = 0; i < tasks.size(); i++) 
    {
        total_network_usage += tasks[i].network_usage;
    }
    cout << "Total Network Usage: " << total_network_usage << " MB" << endl;
}


private:
    vector<Task> tasks;
};

void TaskManager::run() 
{
    int choice;
    string taskName;

    do 
    {
        try
        {
        cout << "Task Manager" << endl;
        cout << "------------" << endl;
        cout << "1. Add Task" << endl;
        cout << "2. Display Tasks" << endl;
        cout << "3. Sort Tasks (FCFS)" << endl;
        cout << "4. Sort Tasks (SJF)" << endl;
        cout << "5. Mark Task as Completed" << endl;
        cout << "6. Kill Task" << endl;
        cout << "7. Display Task Details" << endl;
        cout << "8. Show All Processes" << endl;
        cout << "9. Show Network Usage" << endl;
        cout << "10. Exit" << endl;

        cout << "Enter your choice (1-10): ";
        cin >> choice;

        switch (choice) 
        {
            case 1:
                addTask();
                break;

            case 2:
                displayTasks();
                break;

            case 3:
                fcfs();
                break;

            case 4:
                sjf();
                break;

            case 5:
                completeTask();
                break;

            case 6:
                killTask();
                break;

            case 7:
                cout << "Enter the name of the task to display details: ";
                cin >> taskName;
                displayTaskDetails(taskName);
                break;

            case 8:
                system("tasklist");
                break;

            case 9:
                show_network_usage();
                break;

            case 10:
                cout << "Exiting...";
                break;

            default:
                cout << "Invalid choice. Please enter a number between 1 and 9." << endl;
                break;
        }

        cout << endl;

        } 
        
        catch (const TaskManagerException& ex) 
        {
            cerr << "Error: " << ex.what() << endl;
        }

    } while (choice != 10);

}

int main() 
{
    TaskManagerBase *str;
    TaskManager obj;
    str = &obj;
    str -> run();
    return 0;
}