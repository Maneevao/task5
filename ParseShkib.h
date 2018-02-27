#ifndef _HAVE_PARSESHKIB_H
#define _HAVE_PARSESHKIB_H

#include "Users.h"
#include "Ngram.h"

int ParseShkib(unsigned int size, char* shkib, \
               std::map<const char*, userInfo, char_cmp>* Users,\
               std::map<const char*, srcIPinfo, char_cmp>* SrcIPs,\
               TrieNode* NgramRoot, std::map<char*, unsigned int, char_cmp>* symbols);

#endif /*_HAVE_PARSESHKIB_H*/
