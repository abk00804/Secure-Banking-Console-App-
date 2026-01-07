#include<iostream>
#include<fstream> //for function input and ouput
#include<string>
#include<ctime> //for automatic logout
#include<cctype> // for isalpha and isdigit

using namespace std;

struct account{
string name;
string cnic;
string acct_Numb;
int pin;
double balance;
};
//chking only alphabat
bool isAlpha(string s){
 for(int i=0;i<s.length();i++){
  if(!isalpha((unsigned char)s[i]))
  return false;}
  return true;
  }
  //chk only digits and length
bool isDigit(string s, int len){
    if(s.length() != len)
        return false;

    for(int i = 0; i < s.length(); i++){
        if(!isdigit((unsigned char)s[i]))
            return false;
    }
    return true;
}


//account exixt
bool acct_Exist(string acct){
ifstream in("account.txt");
account a;
while(in>>a.name>>a.cnic>>a.acct_Numb>>a.pin>>a.balance){
if(a.acct_Numb==acct)
return true;
}
return false;
}
// Function for Registered New User
void register_User(){
account a;
int pin_2;
cout<<"------Registered New Account------"<<endl;
do{
cout<<"Enter Name: ";
cin>>a.name;
}
while(!isAlpha(a.name));

do{
cout<<"Enter CNIC No(Without Dashes): ";
cin>>a.cnic;
if(!isDigit(a.cnic,13)){
cout<<"----Invalid CNIC!Plzzz Enter 13 Digits----"<<endl;}
}
while(!isDigit(a.cnic,13));
do{
cout<<"Enter account Number (8 Digits): ";
cin>>a.acct_Numb;
if(acct_Exist(a.acct_Numb))
cout<<"-----Account Already Exist! Enter Different Account Number."<<endl;
}while(!isDigit(a.acct_Numb,8)||acct_Exist(a.acct_Numb));
do{
cout<<"Enter 4-Digit Pin: ";
cin>>a.pin;
cout<<"Re-Enter the Pin: ";

cin>>pin_2;
if(a.pin!=pin_2)
cout<<"----Pin Not Match----"<<endl;

}
while(a.pin!=pin_2);
a.balance=2000;
ofstream out("account.txt",ios::app);   // append is use for save data  on  the next line   
                                        // without rewrite data
out<<a.name<<" "
   <<a.cnic<<" "
   <<a.acct_Numb<<" "
   <<a.pin<<" "
   <<a.balance<<endl;

cout<<"-----Registration Successfully!----"<<endl;
}
//login account
bool login_User(account &user){
string last4;
int pin;
cout<<"----Login----"<<endl;
cout<<"Enter last 4 Digits of Account Number: ";
cin>>last4;
if(!isDigit(last4,4)){
cout<<"----Invalid Inout!Reenter Exactly 4 Digits."<<endl;
return false;
}


cout<<"Enter  Pin: ";
cin>>pin;

ifstream in("account.txt");

while(in >> user.name >> user.cnic >> user.acct_Numb >> user.pin >> user.balance){
if(user.acct_Numb.substr(user.acct_Numb.length()-4,4)==last4 && user.pin==pin){
return true;
}
}

return false;
}


// view balance
void view_Balance(account &user){
cout<<"\n Your Balance: "<<user.balance<<endl;
}
//Transfer Money
void transfer_Money(account &user){
string to_Acct;
double amount;
cout<<"Enter Recipent Account Number: ";
cin>>to_Acct;
cout<<"Enter Amount: ";
cin>>amount;
if(amount>user.balance){
cout<<"Insufficient Balance!";
return;
}
ifstream in("account.txt");
ofstream temp("temp.txt");
account a;
bool found=false;
while (in>>a.name>>a.cnic>>a.acct_Numb>>a.pin>>a.balance){
if(a.acct_Numb==to_Acct){
a.balance +=amount;
found=true;
}
if(a.acct_Numb== user.acct_Numb){
a.balance -=amount;
user.balance -=amount;
}
temp<<a.name<<" "<<a.cnic<<" "<<a.acct_Numb<<" "<<a.pin<<" "<<a.balance<<endl;

}
in.close();
temp.close();
remove("account.txt");   // dell old data
rename("temp.txt","account.txt");// replace with  new data
if(found){
cout<<"-----Transfer Successfully Done!-----"<<endl;
}
else{
cout<<"-----Account Not Found!-----"<<endl;
}
}
//Pay Bill
void pay_Bill(account &user){
double bill;
int choice;
cout << "\n--- Pay Bills ---\n";
cout << "1. Water Bill\n";
cout << "2. Gas Bill\n";
cout << "Choose: ";
cin >> choice;
if (choice != 1 && choice != 2) {
cout << "Invalid choice!\n";
return;
}

cout << "Enter Bill Amount: ";
cin >> bill;

if (bill <= 0) {
cout << "Invalid amount!\n";
return;
}

if (bill > user.balance) {
cout << "Insufficient balance!\n";
return;
}
//Remaining Balance
user.balance -=bill;
//Files
ifstream in("account.txt");
ofstream temp("temp.txt");
account a;
while(in>>a.name>>a.cnic>>a.acct_Numb>>a.pin>>a.balance){
if(a.acct_Numb==user.acct_Numb)
a.balance=user.balance;
temp<<a.name<<" "<<a.cnic<<" "<<a.acct_Numb<<" "<<a.pin<<" "<<a.balance<<endl;
}
in.close();
temp.close();

if (choice == 1)
cout << "Water Bill Paid Successfully!\n";
else
cout << "Gas Bill Paid Successfully!\n";
}
//Auto Logout After 1 Min
bool is_Time_Out(time_t start){
return time(NULL)- start >=60;

}



//User Manu
void user_Manu(account user){
int choice;
time_t last=time(NULL);
do{
if(is_Time_Out(last)){
cout<<"\n----Logout!Inactive For 1 Minute-----"<<endl;
return;
}
cout<<"\n1.View Balance\n";
cout<<"2.Transfer Money\n";
cout<<"3.Pay Bills\n";
cout<<"4.Logout\n";
cout<<"Enter:";
cin>>choice;
last =time(NULL);
if(choice==1)view_Balance(user);
else if(choice==2)transfer_Money(user);
else if(choice==3)pay_Bill(user);
}
while(choice!=4);
}
// Main Manu
int main(){
int ch;
account user;
while(true){
cout<< "================================\n";
cout<< " SECURE BANKING CONSOLE APP\n";
cout<< "================================\n";
cout<<"1.Rgister"<<endl;
cout<<"2.Login"<<endl;
cout<<"3.Exit"<<endl;
cout<<"Enter:";
cin>>ch;
if(ch==1){
register_User();
}
else if(ch==2){
if(login_User(user)){
cout<<"-----Login Success!----"<<endl;
user_Manu(user);
}
else{
cout<<"-----Invalid User!-----"<<endl;
}
}
else if(ch==3)
break;

}
return 0;
}
