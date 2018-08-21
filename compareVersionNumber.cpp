#include<iostream>
#include<algorithm>
#include<string>

using std::string;


void PrintResultByLength(const string &a, const string& b)
{
	string res;
	if(a.length() > b.length())
	{
		res = "The First is Bigger \n";
	}
	if(a.length() < b.length())
	{
		res = "The Second is Bigger \n";
	}
	if(a.length() == b.length())
	{
		res = "They are equal \n";
	}

	if(!res.empty())
	{
		std::cout<< res;
	}
}


void compareVersionNum(string a, string b, char sep)
{	
	size_t posA;
	size_t posB;
	posA = a.find_first_of(sep);
	posB = b.find_first_of(sep);
	//截取第一个'.'之前的子字符串
	string substrA = a.substr(0, posA);
	string substrB = b.substr(0, posB);
	int ret = substrA.compare(substrB); //ascii码比较大小
	if(ret == 0)
	{
		//如果找不到分隔符了	
		if( posA == string::npos || posB == string::npos)	
		{
			PrintResultByLength(a, b);
			return ;	
		}
		//递归截取下一段子字符串	
		string nextA(a.substr(posA + 1)); 	
		string nextB(b.substr(posB + 1)); 	
		if(!nextA.empty() && !nextB.empty())
		{
			compareVersionNum(nextA, nextB, sep);	
		}
		else if(!nextA.empty() && nextB.empty())
		{	
			std::cout<<"The First is Bigger \n";
		}
		else if(nextA.empty() && !nextB.empty())
		{
			std::cout<<"The Second is Bigger \n";
		}
		else
		{
			std::cout<<"They Are equal";
		}

		return ;
	}
	else if(ret == 1)
	{
		std::cout<<"The First is Bigger \n";
		return ;
	}
	else
	{
		std::cout<<"The Second is Bigger \n";
		return ;
	}

	std::cout<<"They are equal"<<std::endl;
}

int main()
{
	std::cout << "Please Input Two Version , press 'q' to exit : " << std::endl;
	string a;
	string b;
	std::cin >> a;
	if(a == "q")
	{
		return 0;
	}
	std::cin >> b;
	while(1)
	{
		compareVersionNum(a, b, '.');
		std::cout << "Please input TWO Version , press 'q' to exit: " << std::endl;
		std::cin >> a;	
		if(a == "q")
		{
			break;	
		}
		std::cin >> b;
	}
	//reutrn 0;
}
