#include<stdio.h>
#include<string.h>
#include<map>
#include<time.h>

//Headers:
#include "Users.h"
#include "ParseShkib.h"
#include "Ngram.h"

int ParseShkib(unsigned int size, char* shkib, \
               std::map<const char*, userInfo, char_cmp>* Users, \
               std::map<const char*, srcIPinfo, char_cmp>* SrcIPs, \
               TrieNode* NgramRoot, std::map<char* ,unsigned int, char_cmp>* symbols)
/* Parse only shkib.csv file from https://yadi.sk/d/GPLjKA_f3Rykid
// shkib file constants calculated before coding, like headers in bmp :)
// TODO: make names for constants
// WARNING: not function - it's procedure
//  1) procedure change data in shkib
//  2) delete char* in symbols and PeriodiReq in Users and SrcIPs after use.
*/
{
	unsigned int tmpWord[5];

	//skip first (header) line
	unsigned int pos = 100;
	shkib[99] = '\x00';

	//parse data by line in each iteration
	unsigned long LineCount = 0;
	while(pos<size)
	{
		unsigned int tmppos;

		char* tmpSymbol     = new char[32+6+32+6+1];
		char* tmpDestIpPort = new char[32+6+1];
		int  tmpSymbolPos = 0;

		time_t _time;
		char*  src_user;
		char*  src_ip;
		char* dest_user;
		char* dest_ip;

		unsigned int  src_port;
		unsigned int dest_port;

		unsigned int  input_byte;
		unsigned int output_byte;
		

		//_time (lenght of time string = 30)
		shkib[pos + 30] = '\x00';
			tm Time;
			//without parsing of timezone
			//strptime(&shkib[pos], "\"%Y-%m-%dT%H:%M:%S.000+0300\"", &Time);
			//_time = timegm(&Time);
		pos += 31;
		//src_user 0 or 32
		if (shkib[pos] == ',') tmppos = pos;
		else tmppos = pos + 32;
		shkib[tmppos] = '\x00';
			src_user = &shkib[pos];
			strcpy(tmpSymbol, src_user);
		pos = tmppos + 1;
		//src_ip length = 32
		shkib[pos + 32] = '\x00';
			src_ip = &shkib[pos];
		pos += 33;
		//src_port
		tmppos = pos;
		while (shkib[tmppos] != ',') tmppos++;
		shkib[tmppos] = '\x00';
			strcat(tmpSymbol, &shkib[pos]);
			src_port = atoi(&shkib[pos]);
		pos = tmppos+1;
		//dest_user 0 or 32
		if (shkib[pos] == ',') tmppos = pos;
		else tmppos = pos + 32;
		shkib[tmppos] = '\x00';
			dest_user = &shkib[pos];
		pos = tmppos + 1;
		//dest_ip length = 32
		shkib[pos + 32] = '\x00';
			strcat(tmpSymbol, &shkib[pos]);
			strcpy(tmpDestIpPort, &shkib[pos]);
			dest_ip = &shkib[pos];
		pos += 33;
		//dest_port
		tmppos = pos;
		while (shkib[tmppos] != ',') tmppos++;
		shkib[tmppos] = '\x00';
			strcat(tmpSymbol, &shkib[pos]);
			strcat(tmpDestIpPort, ":");
			strcat(tmpDestIpPort, &shkib[pos]);
			dest_port = atoi(&shkib[pos]);
		pos = tmppos+1;
		//input_byte
		tmppos = pos;
		while (shkib[tmppos] != ',') tmppos++;
		shkib[tmppos] = '\x00';
			input_byte = atoi(&shkib[pos]);
		pos = tmppos+1;
		//output_byte
		tmppos = pos;
		while (shkib[tmppos] != '\n') tmppos++;
		shkib[tmppos] = '\x00';
			output_byte = atoi(&shkib[pos]);
		pos = tmppos+1;

		// Users update
		if (*src_user  != '\x00')
		{
			AppendUser(Users,  src_user, output_byte, tmpDestIpPort);
			AppendIP(SrcIPs, src_user, tmpDestIpPort);
		}
		if (*dest_user != '\x00') AppendUser(Users, dest_user, input_byte , tmpDestIpPort);


		//Ngrams update
		auto it = symbols->find(tmpSymbol);

		if (it == symbols->end() || symbols->empty())
		{
			unsigned int pos = symbols->size();
			symbols->insert(std::pair<char*,unsigned int>(tmpSymbol,pos));
			tmpWord[LineCount % 5] = pos;
		}
		else
		{
			tmpWord[LineCount % 5] = it->second;
		}

		if (LineCount >= 4)
		{
			InsertWord(NgramRoot, tmpWord, (LineCount - 4) % 5);
		}

//		if (LineCount % 100000 == 0) printf("%lu lines, ", LineCount);
		LineCount++;
	}

	printf("\n");
	return 0;
}
