#include<iostream>
#include<algorithm>
#include<string>

using std::string;

void compareVersionNum(string a, string b, char sep)
{
	size_t posA;
	size_t posB;
	posA = a.find_first_of(sep);
	posB = b.find_first_of(sep);
	string substrA = a.substr(0, posA);
	string substrB = b.substr(0, posB);
	int ret = substrA.compare(substrB);
	if(ret == 0)
	{
			
		if( posA == string::npos || posB == string::npos)	
		{
			if(a.length() > b.length())
			{
				std::cout<<"a > b"<<std::endl;
			}
			else if(a.length() < b.length())			
			{	
				std::cout<<"a < b"<<std::endl;
			}
			else
			{	
				std::cout<<"a = b"<<std::endl;
			}

			//std::cout<<"a = b"<<std::endl;
			return ;	
		}
		
		string nextA(a.substr(posA + 1)); 	
		string nextB(b.substr(posB + 1)); 	
		if(!nextA.empty() && !nextB.empty())
		{
			compareVersionNum(nextA, nextB, sep);	
		}


		return ;
	}
	else if(ret == 1)
	{
		std::cout<<"a bigger"<<std::endl;
		return ;
	}
	else
	{
		std::cout<<"b bigger"<<std::endl;
		return ;
	}

	std::cout<<"a = b"<<std::endl;
}

int main()
{
	
	string a("1.1.3a");
	string b("1.1.3a.b");
	string c("1.2.4b");

	char sep[] = {'.'};

	compareVersionNum(b, c, '.');
		
	//reutrn 0;
}
