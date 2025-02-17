#include<iostream>
using namespace std;
class student{
    int ID;
    float hr;
    public:
    void input(){
        cout<<"enter the student id: ";
        cin>>ID;
        cout<<"enter your study hour: ";
        cin>>hr;
    }
    void operator+(){
        float n;
        cout<<"enter the hours you want to add: ";
        cin>>n;
        hr = hr + n;
        cout<<"new study hr: "<<hr<<endl;
    }
    void operator-(){
        float m;
        cout<<"enter your play time: ";
        cin>>m;
        hr = hr -m;
        cout<<"new study hour: "<<hr<<endl;
    }
    friend void compare(student &s1, student &s2);
};
void compare(student &s1, student &s2){
    if(s1.hr>s2.hr){
        cout<<"First student study more: "<<s1.hr;
    }
    else if(s1.hr==s2.hr){
        cout<<"both has same study  time : "<<s1.hr;
    }
    else{
        cout<<"second student study more: "<<s2.ID;
    }
}
int main(){
    student s1;
    student s2;
    s1.input();
    s2.input();
    +s1;
    -s2;
    compare(s1,s2);

}
