#include<iostream>

using namespace std;

namespace first_space{
    void func(){
        cout << "Inside first namespace" << endl;
    }
}

namespace second_space{
    void func(){
        cout << "Inside second function" << endl;
    }
}

using namespace second_space;

int main(){
    func();

    int x;

    cout << "type a number: " << endl;
    cin >> x;
    cout << "Your number is " << x << endl; 

    string name = "pranav";

    cout << "name :" << name << endl;

    string firstName = "Joe";
    string lastName = "Rogan";
    string fullName = firstName + lastName;
    cout << "full name: " << fullName << endl;  
    cout << "reached here" <<endl;
    return 0;
}