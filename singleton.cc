#include <iostream>

class Singleton {
public:
	static Singleton& Instance(){
		static Singleton inst;
		return inst;
	}
    void Print(){
        std::cout << "this is a singleton!\n";
    }
private:
	Singleton ()= default;
	~Singleton ()= default;
	Singleton (const Singleton &)= delete;
	Singleton & operator=(const Singleton &)= delete;
};

int main()
{
    Singleton::Instance().Print();
}
