#include "lexer.hpp"

#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <functional>
#include <utility>
#include <locale>
#include <cctype>

bool is_alpha_capital(char c)
{
    return c > 64 && c < 91;
}

bool is_alpha_lower(char c)
{
    return c > 96 && c < 123;
}

bool is_alpha(char c)
{
    return is_alpha_capital(c) || is_alpha_lower(c);
}

bool is_numerical(char c)
{
    return c > 47 && c < 58;
}

bool is_whitespace(char c)
{
    return c == 32 || c == 13 || c == 10 || c == 9;
}

bool is_comment(char c)
{
    return c == 35;
}

std::string& ltrim(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

std::string& rtrim(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

std::string& trim(std::string& s)
{
    return ltrim(rtrim(s));
}
    
std::string ltrim(const std::string& s)
{
    std::string tmp_str = s;
    tmp_str.erase(tmp_str.begin(), std::find_if(tmp_str.begin(), tmp_str.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return tmp_str;
}

std::string rtrim(const std::string& s)
{
    std::string tmp_str = s;
    tmp_str.erase(std::find_if(tmp_str.rbegin(), tmp_str.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), tmp_str.end());
    return tmp_str;
}

std::string trim(const std::string& s)
{
    std::string tmp_str = s;
    return ltrim(rtrim(tmp_str));
}

std::vector<std::string> split_string(const std::string& str, const char token)
{
    std::vector<std::string> segments;
    std::stringstream ss(str);
    std::string segment;
    
    while(std::getline(ss, segment, token))
    {
        if(trim(segment) == "")
        {
            continue;
        }

        if(segment[0] == '/' && segment[1] == '/')
        {
            continue;
        }
            
        segments.emplace_back(std::move(segment));
    }
    
    return segments;
}

std::string file_to_string(const char* filename)
{
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if(!in)
    {
        exit(1);
        return "";
    }

    std::string contents;
    
    in.seekg(0, std::ios::end);
    contents.resize(static_cast<unsigned int>(in.tellg()));
    in.seekg(0, std::ios::beg);
    
    in.read(&contents[0], contents.size());
    in.close();
    
    return contents;
}

// http://stackoverflow.com/questions/5343190/how-do-i-replace-all-instances-of-of-a-string-with-another-string
// <3
void replace_string(std::string& subject, const std::string& search, const std::string& replace)
{
    size_t pos = subject.find(search, 0);
    while(pos != std::string::npos)
    {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();

         pos = subject.find(search, pos);
    }
}

void scanner(const char* filename)
{
    std::string file_string = file_to_string(filename);
    replace_string(file_string, "(", " ( ");
    replace_string(file_string, ")", " ) ");
    replace_string(file_string, ";", " ; ");
    replace_string(file_string, ",", " , ");
    replace_string(file_string, "@", " @ ");
    replace_string(file_string, "+", " + ");
    replace_string(file_string, "-", " - ");
    replace_string(file_string, "*", " * ");
    replace_string(file_string, "=", " = ");

    std::vector<std::string> file = split_string(file_string, '\n');

    std::vector<std::string> lexemes;

    int pos = 0;

    char c;
    for(const std::string& line : file)
    {
        std::vector<std::string> line_lexemes = split_string(line, ' ');
        lexemes.insert(lexemes.end(), line_lexemes.begin(), line_lexemes.end());
    }

    return;
}