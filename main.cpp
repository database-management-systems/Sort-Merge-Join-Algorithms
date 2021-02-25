#include<vector>
#include<cstdlib>
#include<cstring>
#include<iostream>
#include<fstream>
#include<sstream>
#include<cmath>
#include<algorithm>
using namespace std;


struct Dept{
    int did;
    string dname;
    double budget;
    int managerid;

    bool operator < (Dept* a) const{
        return did < a->did;
    }
};
bool compare1(const Dept *a, const Dept *b)
{
   return a->managerid < b->managerid;
}

struct Emp {
    int eid;
    string ename;
    int age;
    double salary;
};
bool compare2(const Emp *a, const Emp *b)
{
   return a->eid < b->eid;
}

struct Join {
    int id;
    string ename;
    int age;
    double salary;
    int did;
    string dname;
    double budget;
};

Join* addtoOutput(Dept* dept, Emp* emp){
    Join* table = new Join;
    table->id = dept->managerid;
    table->ename = emp->ename;
    table->age = emp->age;
    table->salary = emp->salary;
    table->did = dept->did;
    table->dname = dept->dname;
    table->budget = dept->budget;
    return table;
}

vector<Join*> sort_merge(vector<Dept*>& dept, vector<Emp*>& emp, int memory_size){
    // sort_merge 
    vector<Join*> result;
    Join* tp;
    int i = 0;
    int j = 0;
    int k = 0;
    vector<Emp*> e_temp = emp;
    memory_size =-1; // since left a block for result
    dept.push_back(NULL);
    e_temp.push_back(NULL);
    
    while (dept.at(i) != NULL && e_temp.at(k) != NULL) {
        while (dept[i]->managerid < e_temp[k]->eid) {
            i++;
        }
        while (dept[i]->managerid > e_temp[k]->eid) {
            k++;
        }
        emp = e_temp;
        j = k;
        while (dept[i]->managerid == e_temp[k]->eid) {
            emp = e_temp;
            j=k;
            while (emp[j]->eid == dept[i]->managerid) {
                tp = addtoOutput(dept[i], emp[j]);
                result.push_back(tp);
                j+=1;
                
            }
            i++;
            if(dept.at(i) == NULL || e_temp.at(k) == NULL)
                break;
        }
        e_temp = emp;
        k = j;
    }
    return result;
}
Dept* create_dept_blcok(int did, string dname, double budget, int managerid){
    Dept* temp = new Dept;
    temp->did = did;
    temp->dname = dname;
    temp->budget = budget;
    temp->managerid = managerid;
    return temp;
};

Emp* create_emp_blcok(int eid, string ename, int age, double salary){
    Emp* temp = new Emp;
    temp->eid = eid;
    temp->ename = ename;
    temp->age = age;
    temp->salary = salary;
    return temp;
};

void fileopen1(string filename1, vector<Dept*>& dept){
    fstream fin;
    vector<string> row;
    string word, temp;
    Dept* dt = new Dept;

    fin.open(filename1,ios::in);
    while (fin){
        getline(fin, temp);
        row.clear();
        stringstream d(temp);
        
        while (getline(d,word,',')) {
            row.push_back(word);
        }
        dt = create_dept_blcok(atoi(row[0].c_str()), row[1], atof(row[2].c_str()), atoi(row[3].c_str()));
        dept.push_back(dt);
    }
    fin.close();
}

void fileopen2(string filename2, vector<Emp*>& emp){
    fstream fin;
    vector<string> row;
    string line, word, temp;
    Emp* ep = new Emp;
    
    fin.open(filename2,ios::in);
    while (fin){
        getline(fin, temp);
        row.clear();
        stringstream d(temp);
        
        while (getline(d,word,',')) {
            row.push_back(word);
        }
        ep = create_emp_blcok(atoi(row[0].c_str()), row[1], atoi(row[2].c_str()), atoi(row[3].c_str()));
        emp.push_back(ep);
    }
    fin.close();
}

int main(){

    string filename1 = "Dept.csv";
    string filename2 = "Emp.csv";
    vector<Dept*> dept;
    vector<Emp *> emp;
    vector<Join*> result;
    int memory_size = 22;
    fileopen1(filename1, dept);
    fileopen2(filename2, emp);
    dept.pop_back();
    emp.pop_back();
    
    sort(dept.begin(), dept.end(), compare1);
    sort(emp.begin(), emp.end(), compare2);

    result = sort_merge(dept, emp, memory_size);

    ofstream testfile;
    testfile.open("join.csv", ios::out);
    testfile << "id" << ',' << "ename" << ',' << "age" << ',' << "salary" << ',' << "did" << ',' << "dname" << ',' << "budget" << '\n';;
    for(int i = 0; i<result.size(); i++){
        testfile << result[i]->id << ',' << result[i]->ename << ',' << result[i]->age << ',' << result[i]->salary << ',' << result[i]->did << ',' << result[i]->dname <<',' << result[i]->budget <<',' << endl ;
    }
    testfile.close();
    return 0;
}
