#ifndef MTree__HH
#define MTree__HH
#include <deque>
#include <map>
#include <set>
#include <vector>
#include "_mutex.h"
#include "errs.h"
using namespace std;
long int unique_key_2_treenode_get_key(void *value);
void unique_key_2_treenode_remove_key(long int k);

template <class T1, class T2>
class treenode{
public:
	enum{
	ALL_NODES=0,
	EXPANDED_NODES=1,
	COLLAPSED_NODES=2
	};
	treenode(){
		//val=0;
		parent=NULL;
		unique_key=0;
		expanded=false;
		unique_key=unique_key_2_treenode_get_key(this);
	}
	~treenode()
	{
		unique_key_2_treenode_remove_key(unique_key);
//			M_LOCK(unique_key_2_treenodeMX);
//			unique_key_2_treenode.erase();
	}
	
private:	
public:	
	T1 key;
	T2 val;
//private:	
	long int unique_key;
	bool expanded;
	map<T1,treenode<T1,T2> > children;
	treenode<T1,T2> *parent;
public:	
	treenode<T1,T2> *get_child(const T1 &s)
	{
		bool findres=true;
		{	
			if(children.find(s)==children.end())
			findres=false;
		}
		if(findres==false)
		{	
			treenode<T1,T2> *cnode=&children[s];
			if(cnode)
			{
				cnode->parent=this;
				cnode->key=s;
				return cnode;
			}
			else
			throw cError((string)"---Error: cnode==NULL");

		}
		treenode<T1,T2> *cnode=NULL;
		{
				cnode=&children[s];
		}
		if(!cnode)throw cError((string)"---Error: cnode==NULL");
		return cnode;
	}
	void remove(T1 k)
	{
		children.erase(k);
	}
	deque<T1> get_children_keylist()
	{
		deque<T1> l;
		for(map<T1,T2>::iterator i=children.begin();i!=children.end();i++)
		{
			l.push_back(i->first);
		}
	}
	private:
	void __get_all_children_keylist(int how_nodes,
		deque<deque<T1> >& __store, 
		deque<T1> &__tmp,
		int (*__cmp)(
			const map<T1,treenode<T1,T2> >::iterator &a,
			const map<T1,treenode<T1,T2> >::iterator &b)
			)
	{
		__tmp.push_back(key);
		__store.push_back(__tmp);

		vector<map<T1,treenode<T1,T2> >::iterator> vi;
		for(map<T1,treenode<T1,T2> >::iterator i=children.begin();i!=children.end();i++)
		{
			vi.push_back(i);
		}
		sort(vi.begin(),vi.end(),__cmp);
		for(unsigned i=0;i<vi.size();i++)
		{
			if(how_nodes==ALL_NODES || vi[i]->second.expanded==true)
			{
				vi[i]->second.__get_all_children_keylist(how_nodes,__store,__tmp,__cmp);
			}
		}
		if(__tmp.size()){__tmp.pop_back();}
	}
	void __get_all_children_keylist_by_unique_keys(set<long int> &unique_keys,deque<deque<T1> >& __store, deque<T1> &__tmp,int (*__cmp)(const map<T1,treenode<T1,T2> >::iterator &a,const map<T1,treenode<T1,T2> >::iterator &b))
	{
		__tmp.push_back(key);
		__store.push_back(__tmp);

		vector<map<T1,treenode<T1,T2> >::iterator> vi;
		for(map<T1,treenode<T1,T2> >::iterator i=children.begin();i!=children.end();i++)
		{
			vi.push_back(i);
		}
		sort(vi.begin(),vi.end(),__cmp);
		for(unsigned i=0;i<vi.size();i++)
		{
			if(unique_keys.find(vi[i]->second.unique_key)!=unique_keys.end())
			{
				vi[i]->second.__get_all_children_keylist(unique_keys,__store,__tmp,__cmp);
			}
		}
		if(__tmp.size()){__tmp.pop_back();}
	}
public:
	deque<deque<T1> > get_all_children_keylist(int how_nodes,int (*__cmp)(const map<T1,treenode<T1,T2> >::iterator &a,const map<T1,treenode<T1,T2> >::iterator &b))
	{
		deque<deque<T1> > __store; 
		deque<T1> __tmp;
		__get_all_children_keylist(how_nodes,__store,__tmp,__cmp);
		return __store;
	}
	deque<deque<T1> > get_all_children_keylist_by_unique_keys(set<long int> &unique_keys,	int (*__cmp)(const map<T1,treenode<T1,T2> >::iterator &a,const map<T1,treenode<T1,T2> >::iterator &b))
	{
		deque<deque<T1> > __store; 
		deque<T1> __tmp;
		__get_all_children_keylist_by_unique_keys(unique_keys,__store,__tmp,__cmp);
		return __store;
	}
public:
	void run_on_branch(deque<T1> &__ll,void (*__fn)(treenode<T1,T2> *a,void*),void* __whatval)
	{
		deque<T1> __l=__ll;
		__run_on_branch(__l,__fn,__whatval);
	}		
private:
	void __run_on_branch(deque<T1> &__l,void (*__fn)(treenode<T1,T2> *a, void*), void *__whatval)
	{
		__fn(this,__whatval);
		if(__l.size())
		{
			T1 __k=*__l.begin();
			__l.erase(__l.begin());
			treenode<T1,T2> *__n2=get_child(__k);
			if(__n2) {
				__n2->__run_on_branch(__l,__fn, __whatval);
			}
			else {
				throw cError("!if(__n2)");
			}
		}
	}		

};

template < class T1, class T2 > deque < T1 > 
unique_key_2_keylist(long int ukey)
{
	deque < T1 > l;
	treenode < T1, T2 > *node = NULL;
	M_LOCK(unique_key_2_treenodeMX);
	if (unique_key_2_treenode.find(ukey) == unique_key_2_treenode.end()) {
		return l;
	}
	node = (treenode < T1, T2 > *) & unique_key_2_treenode[ukey];
	while (node) {
		l.push_front(node->key);
		node = node.parent;
	}
	return l;
}


template < class T1, class T2 > 
void delete_node_by_unique_key(long int ukey)
{
	deque < T1 > l;
	treenode < T1, T2 > *node = NULL;
	M_LOCK(unique_key_2_treenodeMX);
	if (unique_key_2_treenode.find(ukey) == unique_key_2_treenode.end()) {
		return;
	}
	node = (treenode < T1, T2 > *) & unique_key_2_treenode[ukey];
	if (node) {
		if(node->parent)
		{
			node->parent->remove(node->key);
		}
	}
}

#endif
