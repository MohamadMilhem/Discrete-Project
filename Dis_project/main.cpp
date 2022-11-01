#include <iostream>
#include <bits/stdc++.h>
using namespace std;

int var_num;
vector<int> ones, zeros;
vector<char> var;
vector<string> expressions;
map<char, int> ind;
int truth_table[100][10];
vector<pair<int,int>> values;


int now = 0;
vector<int> included;
bool inverted[100];
bool notInverted[100];
int q;

void init(){
    var_num = 0;
    ones.clear(); zeros.clear();
    var.clear(); expressions.clear();
    ind.clear();
    memset(truth_table, 0 , sizeof(truth_table));
    values.clear();
    now = 0;
    included.clear();
    memset(inverted, 0 , sizeof(inverted));
    memset(notInverted, 0 , sizeof(notInverted));
    q = 0;
    return;
}


void display_ops(){
    cout << "Please Select from the following:\n 1. Read expression from a file. \n 2. Check loaded expression. \n 3. Display all the assignments that makes the output TRUE. \n 4. Exit.\n";
    return;
}

void check(){

    if (ones.size() == (1 << var_num)){
        cout << "Tautology\n";
    }
    else if (zeros.size() == (1 << var_num)){
        cout << "Contradicion\n";
    }
    else {
        cout << "Contingency\n";
    }

    return;


}


void gen_truth_table(){
    int flip = (1 << (var_num - 1 ));
    int put = 0;

    for (int i = 0; i < var_num ; i++){
        int cont = flip;
        for (int j = 0 ; j < (1 << var_num) ; j++){
            truth_table[j][i] = put;
            cont--;
            if (cont == 0){put = 1 - put; cont = flip;}
        }
        flip/=2;
    }

}

void cut_exp(string s){

    included.clear();
    values.clear();
    memset(inverted , 0 , sizeof(inverted));
    memset(notInverted, 0 , sizeof(notInverted)) ;

    string now = "";

    int invert = 0;

    for (int i = 0 ; i < s.size() ; i++){
        if (s[i] == ' '){
            if (now.size() == 1){
                if (invert == 1){
                    inverted[ind[now[0]]] = 1;

                    invert = 0;
                }
                else {
                    notInverted[ind[now[0]]] = 1;
                }
                included.push_back(ind[now[0]]);
                now = "";
            }
            else if (now == "not" || now == "Not"){
                invert = 1;
                now = "";
            }
            else {
                now = "";
            }
        }
        else now+= s[i];
    }

    if (now.size() == 1){
        if (invert == 1){
        inverted[ind[now[0]]] = 1;


        invert = 0;
    }
    else {
        notInverted[ind[now[0]]] = 1 ;
    }
        included.push_back(ind[now[0]]);
    }
    else {
        invert = 1;
    }


    for (int i = 0 ; i < included.size() ; i++){

        if (notInverted[included[i]]){
            values.push_back({included[i], 0});
        }
        if(inverted[included[i]] == 1 )
            values.push_back({included[i], inverted[included[i]]});

    }


    return ;

}



void get_values(){

    int expression_truth_table[100][10];

    memset(expression_truth_table, 0 , sizeof(expression_truth_table));

    for (int i = 0 ; i < expressions.size() ; i++){
        cut_exp(expressions[i]);
        int row = 0 ;

        for (int row = 0 ; row < (1 << var_num) ; row++){
            bool ex_com = 0;
            for (int variable = 0 ; variable < values.size() ; variable++){

                if (values[variable].second == 1){
                    ex_com|= (!truth_table[row][values[variable].first]);
                }
                else {
                    ex_com|= (truth_table[row][values[variable].first]);
                }
            }

            expression_truth_table[row][i] = ex_com;

        }

    }


    for (int i = 0 ; i < (1 << var_num) ; i++){
        int andd = 1;
        for (int j = 0 ; j < q ; j++){
            andd&= expression_truth_table[i][j];
        }
        if (andd){
            ones.push_back(i);
        }
        else {
            zeros.push_back(i);
        }
    }





}







void read_exp(){
    string filename;
    ifstream fin;
    cerr << "Please enter a file name > ";
    cin >> filename;
    fin.open( filename.c_str( ) );
    fin >> var_num;


    var.resize(var_num);

    for (int i = 0 ; i < var_num ; i++){
        fin >> var[i];
        ind[var[i]] = i;
    }

    fin >> q;

    expressions.clear();
    now = 0;

    int cont = 0;
    while(cont <= q){
        cont++;
        string here;
        getline(fin , here);
        expressions.push_back(here);
        if (cont == 1)expressions.clear();
    }

    cout << "Loaded.\n";

    return;
}


void display_true(){

    for (int i = 0 ; i < var_num ; i++){
        cout << var[i] << " ";
    }

    cout << ":\n";





    for (int i = 0 ; i < ones.size(); i++){
        int thisrow = ones[i];
        for (int j = 0 ; j < var_num ; j++){
            cout << truth_table[thisrow][j] << " ";
        }
        cout << "\n";
    }


}




int main()
{


    while(true){
        display_ops();
        int opt;
        cin >> opt;

        if (opt == 1){
            init();
            read_exp();
            gen_truth_table();
            get_values();
        }
        else if (opt == 2){
            check();


        }
        else if (opt == 3){
            display_true();
        }
        else if (opt == 4){
            cout << "Bye!\n";
            break;
        }
        else {
            cout << "Wrong option\n";
        }
    }



    return 0;
}
