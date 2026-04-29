#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <ctime>
#include <vector>
#include <algorithm>
using namespace std;
namespace LibrarySystem{

template <typename T>
void debugValue(T val){
    // Silent template (no output change)
}

template <typename T>
class TemplateStorage{
    T data[10];
    int size;
public:
    TemplateStorage(){ size=0; }
    void add(T item){
        if(size<10) data[size++]=item;
    }
    int getSize(){ return size; }
};

class Resource {
protected:
    int resourceID;
    string title;
    string author;
    bool availability;

public:
    Resource(int id=0,string t="",string a="",bool avail=true){
        resourceID=id; title=t; author=a; availability=avail;
    }

    virtual ~Resource(){}

    int getID(){ return resourceID; }
    string getTitle(){ return title; }
    string getAuthor(){ return author; }
    bool isAvailable(){ return availability; }

    virtual void display(){
        cout<<"\nID: "<<resourceID;
        cout<<"\nTitle: "<<title;
        cout<<"\nAuthor: "<<author;
        cout<<"\nStatus: "<<(availability?"Available":"Not Available")<<endl;
    }

    void update(string t,string a){
        title=t; author=a;
    }
};

class Book: public Resource{
    string isbn;
public:
    Book(int id=0,string t="",string a="",bool avail=true,string i="")
    :Resource(id,t,a,avail){ isbn=i; }

    void display() override{
        Resource::display();
        cout<<"ISBN: "<<isbn<<endl;
    }
};

class AudioMaterial: public Resource{
    double duration;
public:
    AudioMaterial(int id=0,string t="",string a="",bool avail=true,double d=0)
    :Resource(id,t,a,avail){ duration=d; }

    void display() override{
        Resource::display();
        cout<<"Duration: "<<duration<<endl;
    }
};

void writeLog(string msg){
    ofstream log("log.txt", ios::app);

    time_t now = time(0);
    tm *ltm = localtime(&now);

    log << setw(2) << setfill('0') << ltm->tm_mday << "-"
        << setw(2) << setfill('0') << 1 + ltm->tm_mon << "-"
        << 1900 + ltm->tm_year << " "
        << setw(2) << setfill('0') << ltm->tm_hour << ":"
        << setw(2) << setfill('0') << ltm->tm_min << " "
        << msg << endl;
}

void loadResources(Resource* r[], int &size){
    ifstream fin("resources.txt");

    if(!fin){
        cout<<"File not found!\n";
        return;
    }

    string type;

    while(getline(fin,type)){

        if(type!="Book" && type!="Audio")
            continue;

        int id,avail;
        string title,author;

        fin>>id;
        fin.ignore();

        getline(fin,title);
        getline(fin,author);

        fin>>avail;
        fin.ignore();

        if(type=="Book"){
            string isbn;
            getline(fin,isbn);
            r[size++] = new Book(id,title,author,avail,isbn);
        }
        else{
            double d;
            fin>>d;
            fin.ignore();
            r[size++] = new AudioMaterial(id,title,author,avail,d);
        }
    }
    fin.close();
}

void saveResources(Resource* r[], int size){
    ofstream fout("resources.txt");

    for(int i=0;i<size;i++){
        if(r[i]==nullptr) continue;

        if(dynamic_cast<Book*>(r[i])) fout<<"Book\n";
        else fout<<"Audio\n";

        fout<<r[i]->getID()<<endl;
        fout<<r[i]->getTitle()<<endl;
        fout<<r[i]->getAuthor()<<endl;
        fout<<(r[i]->isAvailable() ? 1 : 0)<<endl;
        if(dynamic_cast<Book*>(r[i]))
            fout<<"ISBN"<<r[i]->getID()<<endl;
        else
            fout<<2.5<<endl;

        fout<<endl;
    }
}

class Payment{
public:
    int id;
    double amount;
    string date;
    string status;

    Payment(int i=0,double a=0,string d="",string s="Pending"){
        id=i; amount=a; date=d; status=s;
    }
};

class User{
public:

    bool login(string email,string pass){
        return (pass=="1234");
    }

    void logout(){
        cout<<"\nLogged out\n";
    }

    void viewResources(Resource* r[], int size){
        cout<<"\nAvailable Resources:\n";
        for(int i=0;i<size;i++)
            if(r[i]!=nullptr)
                r[i]->display();

        writeLog("Viewed Resources");
    }

    void searchResource(Resource* r[], int size, int id){

    vector<Resource*> temp;

    for(int i=0;i<size;i++)
        if(r[i]!=nullptr)
            temp.push_back(r[i]);

    auto it = find_if(temp.begin(), temp.end(), [id](Resource* res){
        return res->getID() == id;
    });

    if(it != temp.end())
        (*it)->display();
    else
        cout<<"Not found\n";
}

    void searchResource(Resource* r[], int size, string title){
        bool found=false;

        for(int i=0;i<size;i++){
            if(r[i]!=nullptr && r[i]->getTitle()==title){
                r[i]->display();
                found=true;
            }
        }

        if(!found) cout<<"Not found\n";
    }

    void searchResourceByAuthor(Resource* r[], int size, string author){
        bool found=false;

        for(int i=0;i<size;i++){
            if(r[i]!=nullptr && r[i]->getAuthor()==author){
                r[i]->display();
                found=true;
            }
        }

        if(!found) cout<<"Not found\n";
    }

    void userMenu(Resource* r[], int size){

        int ch;

        do{
            cout<<"\nUSER MENU";
            cout<<"\n1. View Resources";
            cout<<"\n2. Search Resource";
            cout<<"\n3. Logout";
            cout<<"\nEnter choice: ";
            cin>>ch;

            if(ch==1)
                viewResources(r,size);

            else if(ch==2){

                int option;

                cout<<"\n1. ID\n2. Title\n3. Author\nChoice: ";
                cin>>option;

                if(option==1){
                    int id;
                    cout<<"Enter ID: ";
                    cin>>id;
                    searchResource(r,size,id);
                }
                else if(option==2){
                    string title;
                    cin.ignore();
                    cout<<"Enter Title: ";
                    getline(cin,title);
                    searchResource(r,size,title);
                }
                else if(option==3){
                    string author;
                    cin.ignore();
                    cout<<"Enter Author: ";
                    getline(cin,author);
                    searchResourceByAuthor(r,size,author);
                }
            }

        }while(ch!=3);
    }
};

class Librarian: public User{
public:

    bool login(string id,string pass){
        return (pass=="admin");
    }

    void addResource(Resource* r[], int &size){

        int id;
        string t,a;

        cout<<"ID:"; cin>>id;
        cin.ignore();

        cout<<"Title:"; getline(cin,t);
        cout<<"Author:"; getline(cin,a);

        r[size++] = new Book(id,t,a,true,"ISBN"+to_string(id));

        writeLog("Added Resource");
        cout<<"Added\n";
    }

    void updateResource(Resource* r[], int size){

        int id;
        cout<<"Enter ID: ";
        cin>>id;
        cin.ignore();

        for(int i=0;i<size;i++){
            if(r[i]!=nullptr && r[i]->getID()==id){

                string t,a;
                cout<<"New Title:"; getline(cin,t);
                cout<<"New Author:"; getline(cin,a);

                r[i]->update(t,a);

                writeLog("Updated Resource");
                cout<<"Updated\n";
                return;
            }
        }

        cout<<"Not found\n";
    }

    void deleteResource(Resource* r[], int size){

        int id;
        cout<<"Enter ID: ";
        cin>>id;

        for(int i=0;i<size;i++){
            if(r[i]!=nullptr && r[i]->getID()==id){
                delete r[i];
                r[i]=nullptr;

                writeLog("Deleted Resource");
                cout<<"Deleted\n";
                return;
            }
        }

        cout<<"Not found\n";
    }

    void generateReport(Resource* r[], int size){

    int total=0, books=0, audio=0;

    cout << "\n==================== RESOURCE REPORT ====================\n\n";

    cout << left << setw(6) << "ID"
         << setw(30) << "Title"
         << setw(25) << "Author"
         << setw(10) << "Type"
         << setw(15) << "Status" << endl;

    cout << string(90, '-') << endl;

    for(int i=0;i<size;i++){
        if(r[i]!=nullptr){

            total++;

            string type = "Audio";
            if(dynamic_cast<Book*>(r[i])){
                type = "Book";
                books++;
            } else {
                audio++;
            }

            cout << left << setw(6) << r[i]->getID()
                 << setw(30) << r[i]->getTitle()
                 << setw(25) << r[i]->getAuthor()
                 << setw(10) << type
                 << setw(15) << (r[i]->isAvailable() ? "Available" : "Not Available")
                 << endl;
        }
    }

    cout << string(90, '-') << endl;

    cout << "\nTotal Resources: " << total << endl;
    cout << "Books: " << books << endl;
    cout << "Audio Materials: " << audio << endl;

    cout << "\nReport Generated Successfully.\n";

    writeLog("Report Generated");
}
    void librarianMenu(Resource* r[], int &size){

        int ch;

        do{
            cout<<"\nLIBRARIAN MENU";
            cout<<"\n1. Add Resource";
            cout<<"\n2. Update Resource";
            cout<<"\n3. Delete Resource";
            cout<<"\n4. Generate Report";
            cout<<"\n5. Logout";
            cout<<"\nChoice: ";
            cin>>ch;

            switch(ch){
                case 1: addResource(r,size); break;
                case 2: updateResource(r,size); break;
                case 3: deleteResource(r,size); break;
                case 4: generateReport(r,size); break;
            }

        }while(ch!=5);
    }
};

class Notification{
public:
    void sendEmail(){
        cout<<"Email Sent\n";
        writeLog("Email Sent");
    }

    void sendSMS(){
        cout<<"SMS Sent\n";
        writeLog("SMS Sent");
    }
};

void processPayment(){
    cout<<"Payment Processed\n";
    writeLog("Payment Processed");
}

void refundPayment(){
    cout<<"Payment Refunded\n";
    writeLog("Payment Refunded");

}
void loadPayments(Payment p[], int &size){
    ifstream fin("payments.txt");

    if(!fin){
        cout<<"Payments file not found!\n";
        return;
    }

    string line;

    while(getline(fin,line)){

        if(line != "Payment")
            continue;

        int id;
        double amount;
        string date, status;

        fin >> id;
        fin >> amount;
        fin.ignore();

        getline(fin, date);
        getline(fin, status);

        p[size++] = Payment(id, amount, date, status);
    }

    fin.close();
}
void displayPayments(Payment p[], int size){

    cout << "\n==================== PAYMENT RECORDS ====================\n\n";

    cout << left << setw(6) << "ID"
         << setw(12) << "Amount"
         << setw(15) << "Date"
         << setw(15) << "Status" << endl;

    cout << string(50, '-') << endl;

    for(int i=0;i<size;i++){
        cout << left << setw(6) << p[i].id
             << setw(12) << p[i].amount
             << setw(15) << p[i].date
             << setw(15) << p[i].status << endl;
    }

    cout << string(50, '-') << endl;
}
}
using namespace LibrarySystem;
int main(){

    LibrarySystem::Resource* resources[100];
    int count=0;

    loadResources(resources,count);

    // TEMPLATE USED (no output change)
    debugValue(10);
    TemplateStorage<int> temp;
    temp.add(5);

     LibrarySystem::User user;
    LibrarySystem::Librarian lib;
    LibrarySystem::Notification notify;

    Payment payments[50];
int payCount = 0;
loadPayments(payments, payCount);

    int choice;

    do{
        cout<<"\nDIGITAL LIBRARY SYSTEM";
        cout<<"\n1. User\n2. Librarian\n3. External System\n4. Exit";
        cout<<"\nEnter choice: ";

        try{
            cin>>choice;

            if(cin.fail()){
                throw "Invalid input!";
            }
        }
        catch(const char* msg){
            cout<<msg<<endl;
            cin.clear();
            cin.ignore(1000,'\n');
            continue;
        }

        if(choice==1){
            string email,pass;
            cout<<"Enter email: "; cin>>email;
            cout<<"Enter password: "; cin>>pass;

            if(user.login(email,pass)){
                writeLog("User Login");
                user.userMenu(resources,count);
            }
            else cout<<"Invalid Login\n";
        }

        else if(choice==2){
            string id,pass;
            cout<<"Enter ID: "; cin>>id;
            cout<<"Enter Password: "; cin>>pass;

            if(lib.login(id,pass)){
                writeLog("Librarian Login");
                lib.librarianMenu(resources,count);
            }
            else cout<<"Invalid Login\n";
        }

       else if(choice==3){
    int ch;
    cout<<"\n1.Send Email\n2.Send SMS\n3.Process Payment\n4.Refund\n5.View Payments\nChoice:";
    cin>>ch;

    if(ch==1) notify.sendEmail();
    else if(ch==2) notify.sendSMS();
    else if(ch==3) processPayment();
    else if(ch==4) refundPayment();
    else if(ch==5) displayPayments(payments, payCount);
    else cout<<"Invalid choice\n";
}
    }while(choice!=4);

    saveResources(resources,count);

    for(int i=0;i<count;i++)
        delete resources[i];

    return 0;
}
