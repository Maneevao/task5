#include<string.h>
#include<map>

#include "Ngram.h"

int InsertWord(TrieNode* root, unsigned int word[5], int shift)
{
	TrieNode* tmp = root;
	for (int level = 0; level<5; level++)
	{
		auto it = tmp->childrens.find(word[(level + shift)%5]);

		if (it != tmp->childrens.end())
		{
			tmp = &it->second;
			tmp->Count++;
		}
		else
		{
			TrieNode newSymbol;
			newSymbol.symbol = word[(level+shift) % 5];
			newSymbol.parent = tmp;
			newSymbol.level  = level+1;
			tmp->childrens[word[(level+shift)%5]] = newSymbol;
			tmp = &(tmp->childrens[word[(level+shift)%5]]);
		}
	}
	return 0;
}

int problem_5(FILE* fresult, TrieNode* NgramRoot, std::map<char*, unsigned int, char_cmp>* symbols, const int n)
{

	fprintf(fresult, "# Рассматривая события сетевого трафика как символы неизвестного языка,\n# найти 5 наиболее устойчивых N-грамм журнала событий\n# (текста на неизвестном языке)\n# id, count word\n");

	TrieNode* top3[n+1]; int top3Count = 0;
	TrieNode* top4[n+1]; int top4Count = 0;
	TrieNode* top5[n+1]; int top5Count = 0;

	for(auto it1 = NgramRoot->childrens.begin(); it1 != NgramRoot->childrens.end(); it1++)
	{
	TrieNode* curNode1 = &it1->second;
	for(auto it2 = curNode1->childrens.begin(); it2 != curNode1->childrens.end(); it2++)
	{
	TrieNode* curNode2 = &it2->second;
	for(auto it3 = curNode2->childrens.begin(); it3 != curNode2->childrens.end(); it3++)
	{
		TrieNode* curNode3 = &it3->second;
			
		//magic top3	
		if (top3Count < n)
		{
			int tmpI = top3Count;
			while(tmpI && top3[tmpI-1]->Count < curNode3->Count)
			{
				top3[tmpI] = top3[tmpI-1];
				tmpI--;
			}
			top3[tmpI] = curNode3;
			top3Count++;
		}
		else
		{
			top3[n] = curNode3;
			int tmpI = n;
			while(tmpI && top3[tmpI-1]->Count < top3[tmpI]->Count)
			{
				TrieNode* tmp = top3[tmpI];
				top3[tmpI] = top3[tmpI-1];
				top3[tmpI-1] = tmp;
				tmpI--;
			}
		}
		for(auto it4 = curNode3->childrens.begin(); it4 != curNode3->childrens.end(); it4++)
		{
			TrieNode* curNode4 = &it4->second;

			//magic top4
                	if (top4Count < n)
                	{
                        	int tmpI = top4Count;
                        	while(tmpI && top4[tmpI-1]->Count < curNode4->Count)
                        	{
                                	top4[tmpI] = top4[tmpI-1];
                                	tmpI--;
                        	}
                        	top4[tmpI] = curNode4;
                        	top4Count++;
                	}
                	else
                	{
                        	top4[n] = curNode4;
                        	int tmpI = n;
                        	while(tmpI && top4[tmpI-1]->Count < top4[tmpI]->Count)
                        	{
                                	TrieNode* tmp = top4[tmpI];
                                	top4[tmpI] = top4[tmpI-1];
                                	top4[tmpI-1] = tmp;
                                	tmpI--;
                        	}
                	}
			for(auto it5 = curNode4->childrens.begin(); it5 != curNode4->childrens.end(); it5++)
			{
				TrieNode* curNode5 = &it5->second;

				//magic top5
				if (top5Count < n)
				{
					int tmpI = top5Count;
					while(tmpI && top5[tmpI-1]->Count < curNode5->Count)
					{
						top5[tmpI] = top5[tmpI-1];
						tmpI--;
					}
					top5[tmpI] = curNode5;
					top5Count++;
				}
				else
				{
					top5[n] = curNode5;
					int tmpI = n; 
					while(tmpI && top5[tmpI-1]->Count < top5[tmpI]->Count)
					{
						TrieNode* tmp = top5[tmpI];
						top5[tmpI] = top5[tmpI-1];
						top5[tmpI-1] = tmp;
						tmpI--;
					}
				}
			}
		}
	}
	}
	}

	//Print stage

	
	fprintf(fresult, "3grams\n");
	for (int i = 0; i<n; i++)
	{
		TrieNode* tmpNode = top3[i];
		fprintf(fresult, "%d: %u, ", i, tmpNode->Count);
		
		while (tmpNode->parent != NULL)
		{
			for (auto it = symbols->begin(); it != symbols->end(); it++)
			{
				if (it->second == tmpNode->symbol)
				{
					fprintf(fresult, "%s ", it->first);
					break;
				}
			}
			tmpNode = tmpNode->parent;
		}
		fprintf(fresult, "\n");
	}
	
	fprintf(fresult, "4grams\n");
	for (int i = 0; i<n; i++)
	{
		TrieNode* tmpNode = top4[i];
		fprintf(fresult, "%d: %u, ", i, tmpNode->Count);
		
		while (tmpNode->parent != NULL)
		{
			for (auto it = symbols->begin(); it != symbols->end(); it++)
			{
				if (it->second == tmpNode->symbol)
				{
					fprintf(fresult,"%s ", it->first);
					break;
				}
			}
			tmpNode = tmpNode->parent;
		}
		fprintf(fresult,"\n");
	}

	fprintf(fresult,"5grams\n");
	for (int i = 0; i<n; i++)
	{
		TrieNode* tmpNode = top5[i];
		fprintf(fresult,"%d: %u, ", i, tmpNode->Count);
		
		while (tmpNode->parent != NULL)
		{
			for (auto it = symbols->begin(); it != symbols->end(); it++)
			{
				if (it->second == tmpNode->symbol)
				{
					fprintf(fresult,"%s ", it->first);
					break;
				}
			}
			tmpNode = tmpNode->parent;
		}
		fprintf(fresult,"\n");
	}

	return 0;
}
