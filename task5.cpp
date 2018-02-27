#include<stdio.h>
#include<stdlib.h>
#include<map>
#include<chrono>

//Headers:
#include "ParseShkib.h"
#include "Users.h" //problem_1and2and3 problem_4
#include "Ngram.h" //problem 5

int main(int argc, char** argv, char** envp)
{
	int ExitStatus = 0;
	char* shkibPath;
	
	if (argc < 2)
	{
		shkibPath = (char*)"shkib.csv";
	}
	else
	{
		shkibPath = argv[1];
	}
	FILE* f = fopen(shkibPath,"r");
	if (f == NULL)
	{
		ExitStatus = 1;
		printf("Usage: ./task5 path/to/shkib.csv\n   or: ./task5 (and shkib.csv in current directory)\n");
	}
	else 
	{
		fseek(f, 0, SEEK_END);
		unsigned int fsize = ftell(f);
		fseek(f, 0, SEEK_SET);

		char* shkib = new char[fsize];
		if (!shkib) 
		{		
			ExitStatus = 2;
		}
		else
		{
			if (fread(shkib, 1, fsize, f) != fsize)
			{
				ExitStatus = 3;
			}
			else
			{
				std::map<const char*, userInfo , char_cmp> Users;
				std::map<const char*, srcIPinfo, char_cmp> SrcIPs;
				TrieNode NgramRoot;
				std::map<char*, unsigned int, char_cmp> symbols;

				printf("Start  parsing shkib\n");
				auto start   = std::chrono::system_clock::now();

					ParseShkib(fsize, shkib, &Users, &SrcIPs, &NgramRoot, &symbols);

				auto end     = std::chrono::system_clock::now();
				auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
				printf("Finish parsing shkib: %ld miliseconds \n", elapsed.count());

				FILE* fresult = fopen("result.txt","w");
				if (fresult == NULL)
				{
					ExitStatus = 4;
				}
				else
				{
					//Problems part

					printf("Start  problem_1and2and3\n");
					start   = std::chrono::system_clock::now();

						problem_1and2and3(fresult, &Users, 5);

					end     = std::chrono::system_clock::now();
					elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
					printf("Finish problem_1and2and3: %ld milliseconds\n", elapsed.count());

					printf("Start  problem_4\n");
					start   = std::chrono::system_clock::now();

						problem_4(fresult, &SrcIPs , 5);

					end     = std::chrono::system_clock::now();
					elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
					printf("Finish problem_4: %ld milliseconds\n", elapsed.count());

					printf("Start  problem_5\n");
					start   = std::chrono::system_clock::now();

						problem_5(fresult, &NgramRoot, &symbols, 5);

					end     = std::chrono::system_clock::now();
					elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
					printf("Finish problem_5: %ld milliseconds\n", elapsed.count());

					fclose(fresult);
				}

				// free symbols
				for(auto it = symbols.begin(); it!=symbols.end(); it++)
				{
					delete[] it->first;
				}

				// free Users
				for (auto it = Users.begin(); it!=Users.end(); it++)
				{
					auto tmpPeriodMap = &(it->second.PeriodicReq);
					for (auto it2 = tmpPeriodMap->begin(); it2 != tmpPeriodMap->end(); it2++)
					{
						delete it2->first;
					}
				}				

				// free SrcIPs
				for (auto it = SrcIPs.begin(); it!=SrcIPs.end(); it++)
				{
					auto tmpPeriodMap = &(it->second.PeriodicReq);
					for (auto it2 = tmpPeriodMap->begin(); it2 != tmpPeriodMap->end(); it2++)
					{
						delete it2->first;
					}
				}				
			}
			delete shkib;
		}
		fclose(f);
	}

	return ExitStatus;
}
