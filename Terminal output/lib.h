#ifndef LIB_H
#define LIB_H

#include<vector>
#include<string>
#include<ctime>
#include<iostream>
#include<fstream>
#include<sstream>
//#include<cstdlib>
#include <thread>
#include<chrono>
#include<iomanip>
using namespace std;

class User{
    private:
        string username;
        string password;
    public:
        User(string u, string p);
        string get_username()const;
        string get_password()const;
};

class Task{
    private:
        string name;
        time_t end_time;
    public:
        Task(string n, time_t e);
        string get_name()const;
        time_t get_time()const;
        void set_name(string n);
        void set_time(time_t t);
};

class File_management{
    virtual void load_user()=0;
    virtual void write_user()=0;
    virtual void load_task(vector<Task> &user_tasks)=0;
    virtual void write_task(vector<Task> &tasks)=0;
};

class App:File_management{
    private:
        vector<User> accounts;
        string username;
        bool user_list_changed=false;
        bool logged_out;
        bool user_entered;
        
        bool login(bool &user_list_changed,bool &logged_out);
        bool program(vector<Task> &tasks,bool &logged_out);
        void update_tasks_list(const vector<Task> &tasks);

        void load_user();
        void write_user();
        void load_task(vector<Task> &user_tasks);
        void write_task(vector<Task> &tasks);

    public:
        App();
        void Run();
};

string readable_time(int sec);
int time_in_sec(const string input);

#endif
