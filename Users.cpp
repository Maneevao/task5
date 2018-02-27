#include<map>
#include<stdio.h>

//Headers:
#include "Users.h"

std::map<const char*, unsigned int>::iterator findMax(std::map<const char*, unsigned int, char_cmp>* PeriodicReq)
{
	std::map<const char*, unsigned int>::iterator maxIt = PeriodicReq->begin();
	for (auto it = PeriodicReq->begin(); it!=PeriodicReq->end(); it++)
	{
		if (it->second > maxIt->second) maxIt = it;
	}
	return maxIt;
}

int problem_1and2and3(FILE* fresult, std::map<const char*, userInfo, char_cmp>* Users, const int n)
// TODO: binary search (not needed for n==5)
{
	std::map<const char*, userInfo, char_cmp>::iterator topN1[n+1];
	std::map<const char*, userInfo, char_cmp>::iterator topN2[n+1];
	std::map<const char*, userInfo, char_cmp>::iterator topN3[n+1];
	char* topN3dest[n+1];
	unsigned int topN3req[n+1];

	//Get first n elements in sorted order
	auto it = Users->begin();
	topN1[0] = it;
	topN2[0] = it;
	topN3[0] = it;
	auto tmpFind = findMax(&(it->second.PeriodicReq));
	if (tmpFind != it->second.PeriodicReq.end())
	{
		topN3dest[0] = (char*)tmpFind->first;
		topN3req[0]  = tmpFind->second;
	}
	else
	{
		topN3dest[0] = NULL;
		topN3req[0]  = 0;
	}
	for (int i = 1; i<n && it != Users->end(); i++, it++)
	{
		int tmpI = i;
		while(tmpI && topN1[tmpI-1]->second.ReqCount < it->second.ReqCount)
		{
			topN1[tmpI] = topN1[tmpI-1];
			tmpI--;
		}
		topN1[tmpI] = it;

		tmpI = i;
		while(tmpI && topN2[tmpI-1]->second.AllReqSize < it->second.AllReqSize)
		{
			topN2[tmpI] = topN2[tmpI-1];
			tmpI--;
		}
		topN2[tmpI] = it;

		tmpI = i;
		auto tmpFind = findMax(&(it->second.PeriodicReq));
		const char* tmpUserName = NULL;
		unsigned int tmpReq  = 0;

		if (tmpFind != it->second.PeriodicReq.end())
		{
			tmpUserName = tmpFind->first;
			tmpReq = tmpFind->second;
		}
		while(tmpI && (topN3req[tmpI-1] < tmpReq))
		{
			topN3[tmpI] = topN3[tmpI-1];
			topN3dest[tmpI] = topN3dest[tmpI-1];
			topN3req[tmpI] = topN3req[tmpI-1];
			tmpI--;
		}
		topN3[tmpI] = it;
		topN3dest[tmpI] = (char*)tmpUserName;
		topN3req[tmpI] = tmpReq;
	}

	// Continue, if we can
	while(it != Users->end())
	{
		int tmpI = n;
		topN1[tmpI] = it;
		topN2[tmpI] = it;
		topN3[tmpI] = it;

		auto tmpFind = findMax(&(it->second.PeriodicReq));
		if (tmpFind != it->second.PeriodicReq.end())
		{
			topN3dest[tmpI] = (char*)tmpFind->first;
			topN3req [tmpI] = tmpFind->second;
		}
		else
		{
			topN3dest[tmpI] = (char*)"";
			topN3req [tmpI] = 0;
		}

		while(tmpI && topN1[tmpI-1]->second.ReqCount < topN1[tmpI]->second.ReqCount)
		{
			auto tmp      = topN1[tmpI];
			topN1[tmpI]   = topN1[tmpI-1];
			topN1[tmpI-1] = tmp;
			tmpI--;
		}

		tmpI = n;
		while(tmpI && topN2[tmpI-1]->second.AllReqSize < topN2[tmpI]->second.AllReqSize)
		{
			auto tmp      = topN2[tmpI];
			topN2[tmpI]   = topN2[tmpI-1];
			topN2[tmpI-1] = tmp;
			tmpI--;
		}

		tmpI = n;
		while(tmpI && (topN3req[tmpI-1] < topN3req[tmpI]))
		{
			auto tmp = topN3[tmpI];
			topN3[tmpI] = topN3[tmpI-1];
			topN3[tmpI-1] = tmp;

			char* tmpC = topN3dest[tmpI];
			topN3dest[tmpI] = topN3dest[tmpI-1];
			topN3dest[tmpI-1] = tmpC;

			unsigned int tmpInt = topN3req[tmpI];
			topN3req[tmpI] = topN3req[tmpI-1];
			topN3req[tmpI-1] = tmpInt;

			tmpI--;
		}

		it++;
	}

	//save to file
	fprintf(fresult,"# Поиск 5ти пользователей, сгенерировавших наибольшее количество запросов\n");
	for(int i = 0; i<n; i++)
	{
		fprintf(fresult,"%d: %s - %u\n",i+1, topN1[i]->first, topN1[i]->second.ReqCount);
	}

	fprintf(fresult,"# Поиск 5ти пользователей, отправивших наибольшее количество данных\n");
	for(int i = 0; i<n; i++)
	{
		fprintf(fresult,"%d: %s - %u\n",i+1, topN2[i]->first, topN2[i]->second.AllReqSize);
	}

	fprintf(fresult, "# Поиск регулярных запросов (запросов выполняющихся периодически) по полю src_user\n");
	for(int i = 0; i<n; i++)
	{
		fprintf(fresult, "%d: Пользователь %s переодически заходит на %s, %u раз\n",i+1, topN3[i]->first, topN3dest[i], topN3req[i]);
	}

	return 0;
}

int problem_4(FILE* fresult, std::map<const char*, srcIPinfo, char_cmp>* SrcIPs, const int n)
{
	std::map<const char*, srcIPinfo, char_cmp>::iterator topN[n+1];
	char* topNdest[n+1];
	unsigned int topNreq[n+1];

	//Get first n elements in sorted order
	auto it = SrcIPs->begin();
	topN[0] = it;
	auto tmpFind = findMax(&(it->second.PeriodicReq));
	if (tmpFind != it->second.PeriodicReq.end())
	{
		topNdest[0] = (char*)tmpFind->first;
		topNreq[0]  = tmpFind->second;
	}
	else
	{
		topNdest[0] = NULL;
		topNreq[0]  = 0;
	}

	for (int i = 1; i<n && it != SrcIPs->end(); i++, it++)
	{
		int tmpI = i;
		auto tmpFind = findMax(&(it->second.PeriodicReq));
		const char* tmpUserName = NULL;
		unsigned int tmpReq  = 0;

		if (tmpFind != it->second.PeriodicReq.end())
		{
			tmpUserName = tmpFind->first;
			tmpReq = tmpFind->second;
		}
		while(tmpI && (topNreq[tmpI-1] < tmpReq))
		{
			topN[tmpI] = topN[tmpI-1];
			topNdest[tmpI] = topNdest[tmpI-1];
			topNreq[tmpI] = topNreq[tmpI-1];
			tmpI--;
		}
		topN[tmpI] = it;
		topNdest[tmpI] = (char*)tmpUserName;
		topNreq[tmpI] = tmpReq;
	}

		// Continue, if we can
	while(it != SrcIPs->end())
	{
		int tmpI = n;
		topN[tmpI] = it;

		auto tmpFind = findMax(&(it->second.PeriodicReq));
		if (tmpFind != it->second.PeriodicReq.end())
		{
			topNdest[tmpI] = (char*)tmpFind->first;
			topNreq [tmpI] = tmpFind->second;
		}
		else
		{
			topNdest[tmpI] = (char*)"";
			topNreq [tmpI] = 0;
		}

		while(tmpI && (topNreq[tmpI-1] < topNreq[tmpI]))
		{
			auto tmp = topN[tmpI];
			topN[tmpI] = topN[tmpI-1];
			topN[tmpI-1] = tmp;

			char* tmpC = topNdest[tmpI];
			topNdest[tmpI] = topNdest[tmpI-1];
			topNdest[tmpI-1] = tmpC;

			unsigned int tmpInt = topNreq[tmpI];
			topNreq[tmpI] = topNreq[tmpI-1];
			topNreq[tmpI-1] = tmpInt;

			tmpI--;
		}

		it++;
	}

	//Save to file
        fprintf(fresult, "# Поиск регулярных запросов (запросов выполняющихся периодически) по полю src_ip\n");
        for(int i = 0; i<n; i++)
        {
                fprintf(fresult, "%d: Пользователь по адреcу %s переодически заходит на %s, %u раз\n",i+1, topN[i]->first, topNdest[i], topNreq[i]);
        }
	return 0;
}

int AppendUser(std::map<const char*, userInfo, char_cmp>* Users,\
               const char* UserName, const unsigned bytes, char* tmpDestIpPort)
{
	auto it = Users->find(UserName);

	if (__builtin_expect(it == Users->end() || Users->empty(), 0))
	{
		userInfo tmpUser;
		tmpUser.ReqCount    = 1;
		tmpUser.AllReqSize  = bytes;
		tmpUser.PeriodicReq.insert(std::pair<char*, unsigned int>(tmpDestIpPort, 1));
		Users->insert(std::pair<const char*, userInfo>(UserName, tmpUser));
	}
	else
	{
		it->second.ReqCount++;
		it->second.AllReqSize += bytes;
		auto it2 = it->second.PeriodicReq.find(tmpDestIpPort);
		if (it2 != it->second.PeriodicReq.end())
		{
			it2->second += 1;
		}
		else
		{
			it->second.PeriodicReq.insert(std::pair<const char*, unsigned int>(tmpDestIpPort, 1));
		}
	}

	return 0;
}

int AppendIP(std::map<const char*, srcIPinfo,char_cmp>* SrcIPs, char* src_ip, char* tmpDestIpPort)
{
	auto it = SrcIPs->find(src_ip);
	if (it == SrcIPs->end() || SrcIPs->empty())
	{
		srcIPinfo tmpInfo;
		tmpInfo.PeriodicReq.insert(std::pair<const char*, unsigned int>(tmpDestIpPort, 1));
		SrcIPs->insert(std::pair<const char*, srcIPinfo>(src_ip, tmpInfo));
	}
	else
	{
		auto it2 = it->second.PeriodicReq.find(tmpDestIpPort);
		if (it2 == it->second.PeriodicReq.end())
		{
			it->second.PeriodicReq.insert(std::pair<const char*, unsigned int>(tmpDestIpPort,1));
		}
		else
		{
			it2->second += 1;
		}
	}
	return 0;
}
