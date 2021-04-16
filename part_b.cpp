#include <iostream>
#include <string>
#include <cstring>
#include <stack>

using std::string;
using std::stack;
#include "tokens.hpp"

void print_brace(string str);
void print_indent(char c,int indent);
bool isBalanced (string str, bool isHalf=false);
void driver_function(string a);



string token_list[] = {"", "VOID", "INT", "BYTE", "B", "BOOL", "AND", "OR", "NOT", "TRUE", "FALSE", "RETURN", "IF", "ELSE", "WHILE", "BREAK", "CONTINUE", "SWITCH", "CASE", "DEFAULT", "COLON", "SC", "COMMA",
                       "LPAREN", "RPAREN", "LBRACE", "RBRACE", "ASSIGN", "RELOP", "BINOP" , "COMMENT", "ID", "NUM", "STRING"};


int main() {
    int token;
    string a;
    token = yylex();
    while(token) {
        switch (token) {
            case LPAREN: {
                a.push_back('(');
                break;
            }
            case RPAREN: {
                a.push_back(')');
                break;
            }
            case LBRACE: {
                a.push_back('{');
                break;
            }
            case RBRACE: {
                a.push_back('}');
                break;
            }
            case ' ': { //todo: define regex in scanner.yy and ignore whitespace here
                break;
            }
            case '\t': { //todo: define regex in scanner.yy and ignore whitespace here
                break;
            }
            case '\r': { //todo: define regex in scanner.yy and ignore whitespace here
                break;
            }
            case '\n': { //todo: define regex in scanner.yy and ignore whitespace here
                break;
            }
            case '\v': { //todo: define regex in scanner.yy and ignore whitespace here
                break;
            }
            case '\f': { //todo: define regex in scanner.yy and ignore whitespace here
                break;
            }
            default: {
                if(!isBalanced(a,true)){
                    std::cout<< "Error: Bad Expression"<<std::endl;
                    exit(0);
                }
                print_brace(a);
                std::cout << "Error: " << token_list[token] << std::endl;
                exit(0);
            }
        }
        token = yylex();
    }
    driver_function(a);
    return 0;
}

void driver_function(string a){
   if(!isBalanced(a)){
       std::cout<< "Error: Bad Expression"<<std::endl;
       exit(0);
    }
   else{
        print_brace(a);
    }
}

//Assuming str is a valid string according to Part B
void print_brace(string str){

    int length = str.length();
    int count_type_1 = 0;
    int count_type_2 = 0;
    int indent = 0;

    for (int i = 0; i < length; ++i) {
        if(str[i] == '('){
            indent = count_type_1 + count_type_2;
            print_indent(str[i],indent);
            count_type_1++;
        }
        else if(str[i] == '{'){
            indent = count_type_1 + count_type_2;
            print_indent(str[i],indent);
            count_type_2++;
        }
        else if(str[i] == ')'){
            count_type_1--;
            indent = count_type_1 + count_type_2;
            print_indent(str[i],indent);

        }
        else if(str[i] == '}'){
            count_type_2--;
            indent = count_type_1 + count_type_2;
            print_indent(str[i],indent);
        }
    }
}


void print_indent(char c,int indent){
    string temp_str;
    for (int i = 0; i < indent; ++i)
        temp_str.push_back('\t');

    temp_str.push_back(c);
    std::cout << temp_str << std::endl;
}

bool isBalanced (string str, bool isHalf){
    stack<char> stack;
    char x;

    for (unsigned int i = 0; i < str.length(); i++)
    {
        if (str[i] == '(' || str[i] == '{'){
            stack.push(str[i]);
            continue;
        }

        if (stack.empty()){
            string pre_brackets = str.substr(0,i);
            print_brace(pre_brackets);
            return false;
        }


        switch (str[i]) {
            case ')':
                x = stack.top();
                stack.pop();
                if (x == '{'){
                    string pre_brackets = str.substr(0,i);
                    print_brace(pre_brackets);
                    return false;
                }

                break;

            case '}':
                x = stack.top();
                stack.pop();
                if (x == '('){
                    string pre_brackets = str.substr(0,i);
                    print_brace(pre_brackets);
                    return false;
                }
                break;
        }
    }

    if (!stack.empty() && !isHalf){
        print_brace(str);
        return false;
    }

    return true;
}

///checks if the string is only half balanced -> balanced from one side -> the closing side. }}

