#ifndef _HAVE_NGRAM_H
#define _HAVE_NGRAM_H

#include<stdio.h>
#include<string.h>
#include<map>

#include "Users.h"

struct NgramSymbol
{
	char* SrcIP;
	unsigned int SrcPort;
	char* DstIP;
	unsigned int DstPort;
	bool operator==(const NgramSymbol& x)
	{
		return (SrcPort == x.SrcPort && DstPort == x.DstPort && strcmp(SrcIP, x.SrcIP)==0 && strcmp(DstIP, x.DstIP)==0);
	}
	bool operator <(const NgramSymbol& x)
	{
		return SrcPort<x.SrcPort && \
		       DstPort<x.DstPort && \
		       strcmp(SrcIP, x.SrcIP)<0 && \
		       strcmp(DstIP, x.DstIP)<0;
	}
};

struct TrieNode
{
	std::map<unsigned int, TrieNode> childrens;
	TrieNode* parent = NULL;
	unsigned int level = 0;
	unsigned int symbol;
	unsigned int Count = 1;
};

struct symbol_cmp
{
	bool operator () (const NgramSymbol a, const NgramSymbol b) const
	{
		return a.SrcPort<b.SrcPort && \
		       a.DstPort<b.DstPort && \
		       strcmp(a.SrcIP, b.SrcIP)<0 && \
		       strcmp(a.DstIP, b.DstIP)<0;
	}
};

int InsertWord(TrieNode* root, unsigned int word[5], int shift);

int problem_5(FILE* fresult, TrieNode* NgramRoot, std::map<char*, unsigned int, char_cmp>* symbols, const int n);

#endif /*_HAVE_NGRAM_H*/
