#include <iostream>
#include <string>
#include <sstream>
 
void print_istream(std::istringstream &in)
{
    std::cout << "----\n";
    std::cout << "state = " << in.rdstate() << std::endl
              << "good? " << in.good() << " eof? " << in.eof() << " fail? " << in.fail() << " bad? " << in.bad() << "\n"
              << "pos = " << in.tellg() << std::endl;
}

int main()
{
    std::string str = "Hello world";
    std::istringstream in(str);
    std::string word1, word2;
 
    in >> word1;
    in >> word2;

    print_istream(in);
    std::cout << "word1 = " << word1 << '\n'
              << "word2 = " << word2 << '\n';

    in.seekg(0); 
    print_istream(in);

    in.clear();
    print_istream(in);
    
    in.seekg(0); 
    print_istream(in);
    
    in >> word2;
    print_istream(in);
    std::cout << "word1 = " << word1 << '\n'
              << "word2 = " << word2 << '\n';
}