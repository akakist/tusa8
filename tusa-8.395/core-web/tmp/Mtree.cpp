#ifdef __GNUC__
#pragma implementation				// gcc: implement sql_udf.h
#endif

#include "stdafx.h"
#include "_mutex.h"
#include "mutex_inspector.h"
#include "lockctl.h"
#include "Mtree.h"
#include <map>
map<long int/*unique_key*/,void * /*treenode*/> unique_key_2_treenode;
_mutex unique_key_2_treenodeMX("unique_key_2_treenodeMX");

void unique_key_2_treenode_remove_key(long int k)
{
	M_LOCK(unique_key_2_treenodeMX);
	unique_key_2_treenode.erase(k);
}
long int unique_key_2_treenode_get_key(void *value)
{
	M_LOCK(unique_key_2_treenodeMX);
	bool cond = true;
	long int k;
	while (cond) {
		 k = rand();
		if (unique_key_2_treenode.find(k) == unique_key_2_treenode.end()) {
			unique_key_2_treenode[k] = value;
			cond = false;
		}
	}
	return k;
}
