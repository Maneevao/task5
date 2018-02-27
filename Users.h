#ifndef _HAVE_USERS_H
#define _HAVE_USERS_H

#include<stdio.h>
#include<string.h>
#include<map>

struct char_cmp
{
	bool operator () (const char* a, const char* b) const
	{
		return strcmp(a,b)<0;
	}
};

struct userInfo
{
	unsigned int ReqCount   = 0;
	unsigned int AllReqSize = 0;
	std::map<const char*, unsigned int, char_cmp> PeriodicReq;
};

struct srcIPinfo
{
	std::map<const char*, unsigned int, char_cmp> PeriodicReq;
};

int problem_1and2and3(FILE* fresult, std::map<const char*, userInfo, char_cmp>* Users, const int n);

int problem_4(FILE* fresult, std::map<const char*, srcIPinfo, char_cmp>* SrcIPs, const int n);

int AppendUser(std::map<const char*, userInfo, char_cmp>* Users,\
               const char* UserName, const unsigned bytes, char* tmpDestIpPort);

int AppendIP(std::map<const char*, srcIPinfo, char_cmp>* SrcIPs, char* src_ip, char* tmpDestIpPort);

#endif /* _HAVE_USERS_H */
