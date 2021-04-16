#include "tokens.hpp"
#include <iostream>

using std::string;
using std::cout;
using std::endl;

string gen_string(string s);
bool isInRange(string byte);
string byte2char(string byte);

string token_list[] = {"", "VOID", "INT", "BYTE", "B", "BOOL", "AND", "OR", "NOT", "TRUE", "FALSE", "RETURN", "IF", "ELSE", "WHILE", "BREAK", "CONTINUE", "SWITCH", "CASE", "DEFAULT", "COLON", "SC", "COMMA",
                       "LPAREN", "RPAREN", "LBRACE", "RBRACE", "ASSIGN", "RELOP", "BINOP" , "COMMENT", "ID", "NUM", "STRING"};
int main()
{
    int token = yylex();
    while(token) {
        switch (token){
            case (COMMENT):{
                cout<<yylineno<<" "<<token_list[token]<<" //"<<endl;
                break;
            }
            case (NUM):{
                cout<<yylineno<<" "<<token_list[token]<<" "<<atoi(yytext)<<endl;
                break;
            }
            case (STRING):{
                string s = gen_string(yytext);
                cout<<yylineno<<" "<<token_list[token]<<" "<<s<<endl;
                break;
            }
            default:
            {
                cout<<yylineno<<" "<<token_list[token]<<" "<<string(yytext)<<endl;
            }
        }
        token = yylex();
    }
    return 0;
}


string byte2char(string byte){
    string s;
    s = (char)(int)strtol(byte.c_str(), nullptr, 16);
    return s;
}
string gen_string(string s){
    string str = s.substr(1, s.size()-2);
    int i = str.find_first_of('\\',0);
    while(i!=std::string::npos){
        if(str[i+1] != 'n' && str[i+1] != 't' && str[i+1] != 'r' && str[i+1] != '\\' && str[i+1] != '0' && str[i+1] != '\"' && str[i+1] != 'x'){
            printf("Error undefined escape sequence %c\n", str[i+1]);
            exit(0);
        }
        if(str[i+1] == 'x'){
            int remaining_chars = (int)str.length() - (i + 2);
            string byte = str.substr(i + 2, 2);
            if (remaining_chars < 2 or !isInRange(byte)) {
                printf("Error undefined escape sequence x%s\n",
                       str.substr(i + 2, std::min(2,remaining_chars)).c_str());
                exit(0);
            }
            i=str.find_first_of('\\',i+4);
        }
        else {
            i = str.find_first_of('\\', i + 2);
        }
    }
    i = str.find_first_of('\\',0);
    while(i!=std::string::npos) {
        if(i == str.length()-1){
            printf("Error unclosed string\n");
            exit(0);
        }
        switch(str[i+1]) {
            case ('n'):{
                str = str.replace(i, 2, "\n");
                break;
            }
            case ('t'): {
                str = str.replace(i, 2, "\t");
                break;
            }
            case ('r'): {
                str = str.replace(i, 2, "\r");
                break;
            }
            case ('\\'):{
                str = str.replace(i, 2, "\\");
                break;
            }
            case ('\"'):{
                str = str.replace(i, 2, "\"");
                break;
            }
            case ('0'): {
                str = str.substr(0, i);
                break;
            }
            case ('x'): {
                int remaining_chars = (int)str.length() - (i + 2);
                string byte = str.substr(i + 2, 2);
                if (remaining_chars >= 2 and isInRange(byte)) {
                    string c = byte2char(byte);
                    if(byte == "00"){
                        str = str.substr(0, i);
                    }
                    else {
                        str = str.replace(i, 4, c);
                    }
                }
                else {
                    printf("Error undefined escape sequence x%s\n",
                           str.substr(i + 2, std::min(2,remaining_chars)).c_str());
                    exit(0);
                }
                break;
            }
            default:
                printf("Error undefined escape sequence %c\n", str[i+1]);
                exit(0);
        }
        i=str.find_first_of('\\',i+1);
    }
    return str;
}


bool isInRange(string byte) {
    char first = byte[0];
    char second = byte[1];
    if ((first>='0' && first<='7') &&
        ((second >= '0' && second <= '9') ||
         (second >= 'a' && second <= 'f') ||
         (second >= 'A' && byte[1] <= 'F'))) {
        return true;
    }
    return false;
}
