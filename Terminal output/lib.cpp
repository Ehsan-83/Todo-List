#include "lib.h"

User::User(string u, string p):username(u),password(p){}
string User::get_username()const{return username;}
string User::get_password()const{return password;}



Task::Task(string n, time_t e):name(n),end_time(e){
    
}
string Task::get_name()const{return name;}
time_t Task::get_time()const{return end_time;}
void Task::set_name(string n){
    name=n;
}
void Task::set_time(time_t t){
    end_time=t;
}

App::App(){}

void App::load_user(){
    fstream file("Accounts\\accounts.txt", ios::app | ios::in);

    string username;
    string password;
    while (getline(file,username) && getline(file,password)) {
        
        accounts.emplace_back(username,password);
        file.ignore(10,'\n');
    }
}
void App::write_user(){
    ofstream file("Accounts\\accounts.txt");
    
    for(int i=0;i<accounts.size();i++){
        file<<accounts[i].get_username()<<'\n';
        file<<accounts[i].get_password()<<'\n';
        file<<"#\n";
    }
}
void App::load_task(vector<Task> &user_tasks){
    fstream file("Users\\"+username+".txt",ios::app | ios::in);
    string name;
    string t;
    while(getline(file, name) && getline(file, t)){

        user_tasks.emplace_back(name,stoi(t));
        file.ignore(10,'\n');
    }
}
void App::write_task(vector<Task> &tasks){
    ofstream file("Users\\"+username+".txt");
        
    for(int i=0;i<tasks.size();i++){
        file<<tasks[i].get_name()<<'\n';
        file<<to_string(tasks[i].get_time())<<'\n';
        file<<"#\n";
    }
}


void App::Run(){
    cout<<"-----TODO LIST app-----\n";

    load_user();
    
    do{
        logged_out=false;
        bool successful_login=login(user_list_changed,logged_out);
        
        if(successful_login){
            vector<Task> tasks;
            load_task(tasks);

            while (program(tasks,logged_out));

            //writing tasks
            write_task(tasks);
        }
    }while(logged_out);
    
    //writing users
    if(user_list_changed){
        write_user();
    }
}


bool App::login(bool &user_list_changed,bool &logged_out){
    string password;

    while(true){
        cout<<"##################################\nSign Up (1) / Login (2) / Exit (anything else)\n";
        cout<<"Enter: ";
        int choice;
        cin>>choice;
        bool signed_up;
        if (cin.peek() != '\n') {choice=0;}
        
        switch (choice)
        {
        case  1:
            cout<<"#######\nSIGN UP\nUsername: ";
            if(!(cin>>username) || cin.peek() != '\n' || username=="admin"){
                cout<<"Invalid username\n";
                cin.ignore(100,'\n');
                cin.clear();
            }
            else{
                signed_up=false;
                for(int i=0;i<accounts.size();i++){
                    if(username==accounts[i].get_username()){
                        signed_up=true;
                        cout<<"You have already signed up!\n";
                        break;
                    }
                }
                if(!signed_up){
                    cout<<"Password: ";
                    if(cin>>password && cin.peek() == '\n'){
                        accounts.emplace_back(username,password);
                        cout<<"You just signed up! Wellcome "+username+'\n';
                        user_list_changed=true;
                        return true;
                    }else{
                        cout<<"Invalid password!\n";
                        cin.ignore(100,'\n');
                        cin.clear();
                    }
                }
            }
            break;
        
        case 2:
            cout<<"#######\nLOGIN\nUsername: ";
            if(!(cin>>username) || cin.peek() != '\n'){
                cout<<"Invalid username\n";
                cin.ignore(100,'\n');
                cin.clear();
            }
            else{
                signed_up=false;
                for(int i=0;i<accounts.size();i++){
                    if(username==accounts[i].get_username()){
                        signed_up=true;
                        cout<<"Password: ";
                        if(cin>>password && cin.peek() == '\n'){
                            if(password==accounts[i].get_password()){
                                cout<<"Wellcome "+accounts[i].get_username()+"!!!\n";
                                return true;
                            }
                            else{
                                cout<<"Password is wrong!\n";
                            }
                        }else{
                        cout<<"Invalid password!\n";
                        cin.ignore(100,'\n');
                        cin.clear();
                        }
                        
                        break;
                    }
                }
                if(username=="admin"){
                    cout<<"Password: ";
                    if(cin>>password && cin.peek()=='\n' && password=="admin"){

                        while(true){
                            cout<<"###############\nAdmin\n----------------------\n";
                            cout<<"(Accounts : Passwords)\n";

                            if(accounts.size()==0){
                                cout<<"empty!\n";
                            }
                            else{
                                for(int i=0;i<accounts.size();i++){
                                    cout<<i+1<<'.'<<accounts[i].get_username()<<" : "<<accounts[i].get_password()<<'\n';
                                }
                            }
                            cout<<"----------------------\n";
                            cout<<"1.delete account\n";
                            cout<<"2.Log out\n";
                            cout<<"Exit (anything else)\n";
                            cout<<"Enter:";
                            
                            int ch;
                            cin>>ch;
                            if (cin.peek() != '\n') {ch=0;}

                            
                            if (ch==1)
                            {
                                int d;
                                cout<<"Enter number to delete account: ";
                                cin>>d;

                                if(d>accounts.size() || d<1){
                                    cout<<"Invalid number!\n";
                                }
                                else{
                                    if (remove(("Users\\"+accounts[d-1].get_username()+".txt").c_str()) == 0) {
                                        accounts.erase(accounts.begin() + d-1);
                                        user_list_changed=true;
                                        cout << "Account deleted successfully.\n";
                                    } else {
                                        cout<<"Error occurred while deleting\n";
                                    }
                                }
                            }
                            else if(ch==2){
                                cout<<"You logged out!\n";
                                logged_out=true;
                                return false;
                            }
                            else{
                                cout<<"Good bye!";
                                return false;
                            }
                        }
                    }
                    else{
                        cout<<"Wrong admin password!\n";
                        cin.ignore(100,'\n');
                        cin.clear();
                    }
                    
                }
                else if(!signed_up){
                    cout<<"You haven't signed up yet! Please sign up\n";
                }
            }
            break;
        
        default:
            cout<<"goodbye!";
            return false;
        }
    }
}


bool App::program(vector<Task> &tasks,bool &logged_out){
    user_entered=false;
    thread wait_for_user(&App::update_tasks_list,this, ref(tasks));
    int x;
    cin>>x;
    user_entered=true;
    wait_for_user.join();
    if (cin.peek() != '\n') x=0;
    switch (x)
    {
    case 1:{
        string task_name;
        string remained_time;

        cout<<"Adding:\nName of task: ";
        cin.ignore(100, '\n');
        cin.clear();
        if(getline(cin,task_name) && !task_name.empty()){
            cout<<"Remained time (DD/hh:mm:ss): ";
            cin>>remained_time;

            int r_time=time_in_sec(remained_time);
            if(r_time>0){
                time_t end= time(0) + r_time;
                tasks.emplace_back(task_name,end);
            }else{
                this_thread::sleep_for(chrono::seconds(3));
            }

        }
        else{
            cout<<"Invalid Task name\n";
            cin.ignore(100,'\n');
            cin.clear();
            this_thread::sleep_for(chrono::seconds(3));
        }

        break;
    }
    case 2:{
        if(tasks.size()==0){
            cout<<"Nothing to delete!\n";
        }else{
            int d;
            cout<<"Deleting:\nEnter number of task to delete (0 to delete all): ";
            if(!(cin>>d) || cin.peek()!='\n'){
                cout<<"Invalid entry!\n";
                cin.ignore(100,'\n');
                cin.clear();
            }
            else if(d>tasks.size() || d<0){
                cout<<"Invalid number!\n";
            }
            else if(d!=0){
                tasks.erase(tasks.begin() + d-1);
                cout<<"Task number "<<d<<" was deleted!\n";
            }else{
                tasks.clear();
                cout<<"All of your tasks were deleted!\n";
            }
        }
        this_thread::sleep_for(chrono::seconds(3));
        break;
    }
    case 3:{
        cout<<"enter a task number to be done: ";
        int d;
        if(cin>>d && d<=tasks.size()){
            tasks[d-1].set_time(time(0));
            cout<<"Was set!\n";
        }else{
            cout<<"Couldnt set it done!\n";
        }
        cin.clear();
        cin.ignore(100,'\n');
        
        this_thread::sleep_for(chrono::seconds(3));
        break;
    }
    case 4:{
        cout<<"Editting\nEnter number of task you want to edit: ";
        int d;
        if(cin>>d && d<=tasks.size()){
            cout<<"Name of task: ";
            cin.ignore(100, '\n');

            string task_name;
            string remained_time;
            if(getline(cin,task_name) && !task_name.empty()){
                cout<<"Remained time (DD/hh:mm:ss): ";
                cin>>remained_time;
                int r_time=time_in_sec(remained_time);
                if(r_time>0){
                    time_t end= time(0) + r_time;
                    tasks[d-1].set_name(task_name);
                    tasks[d-1].set_time(end);
                    cout<<"Was editted successfully!\n";
                }
            }
        }else{
            cout<<"Wrong input!\n";
        }
        cin.clear();
        cin.ignore(100,'\n');
        
        this_thread::sleep_for(chrono::seconds(1));
        break;
    }
    case 5:{
        cout<<"You logged out!\n";
        logged_out=true;
        return false;
    }
    default:
        cout<<"Good bye!";
        return false;
    }
    return true;
}

void App::update_tasks_list(const vector<Task> &tasks){
    while(!user_entered){
        ostringstream output;
        output<<"\x1B[2J\x1B[H";
        output<<"####################################################\n";
        output<<"Todo List for "<<username<<"\n(Name: Remained time(DD/hh:mm:ss))\n";
        output<<"------------------------------------\n";

        if(tasks.size()==0){
            output<<"Empty!\n";
        }
        else{
            for(int i=0;i<tasks.size();i++){
                string remained_time= (tasks[i].get_time()-time(0))>0 ? readable_time(tasks[i].get_time()-time(0)) : "Time is up!";
                output<<i+1<<'.'<<left<<setw(20)<<tasks[i].get_name()<<": "<<remained_time<<'\n';
            }
        }
        
        output<<"------------------------------------\n";
        output<<"Please type the coresponding number!\n";
        output<<"1.Add a task\n";
        output<<"2.Delete tasks\n";
        output<<"3.Set task done\n";
        output<<"4.Edit\n";
        output<<"5.Log out\n";
        output<<"Exit (press anything else)\n";
        output<<"Enter: ";
        
        cout<<output.str()<<flush;

        std::this_thread::sleep_for(chrono::seconds(1));
    }
}

string readable_time(int sec){
    string time;
    string day=to_string(sec/(24*3600));
    sec%=(24*3600);
    string hour=to_string(sec/3600);
    sec%=3600;
    string minute=to_string(sec/60);
    string second=to_string(sec%60);

    time=day+'/'+hour+':'+minute+':'+second;

    return time;
}

int time_in_sec(const string input) {
    char sep1, sep2, sep3;
    int day , hour , minute , second;
    stringstream ss(input);
    
    ss >> day >> sep1 >> hour >> sep2 >> minute >> sep3 >> second;

    if (sep1 != '/' || sep2 != ':' || sep3 != ':' || ss.fail() || ss.fail()) {
        cout<<"Invalid format. Expected DD/hh:mm:ss\n";
        return -1;
    }
    if (day < 0 || hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59) {
        cout<< "Invalid time values!\n";
        return 0;
    }

    int time=(day*3600*24)+(hour*3600)+(minute*60)+second;

    return time;
}