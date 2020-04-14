#pragma once

#ifndef KVSTORE_H
#define KVSTORE_H

#include <string>
#include "kvstore_api.h"
#include "MemTable.hpp"
#include "SkipList.hpp"
#include "SSLevel.hpp"
#include "disk.hpp"

#define LSM_DEPTH 10

using namespace std;

class KVStore : public KVStoreAPI 
{
private:
	string dir_;
	MemTable mem;
	disk disc;
	
public:
	KVStore(const string &dir);

	~KVStore();

	void put(uint64_t key, const string &s) override;

	std::string get(uint64_t key) override;

	bool del(uint64_t key) override;

	void reset() override;
	
	void compaction(uint64_t key);
};

KVStore::KVStore(const string &dir): KVStoreAPI(dir)
{
	dir_ = dir;
	disc.SET_DEPTH(LSM_DEPTH);
	disc.SET_DIR_PATH(dir_ + "/");
}

KVStore::~KVStore()
{
}

/**
 * Insert/Update the key-value pair.
 * No return values for simplicity.
 */
void KVStore::put(uint64_t key, const string &s)
{
	mem.PUT(key, s);
	
	if(mem.IS_FULL())
	{
		compaction(key);
	}
}

/**
 * Returns the (string) value of the given key.
 * An empty string indicates not found.
 */
std::string KVStore::get(uint64_t key)
{
	bool flag;
	string res = mem.GET(key, flag);
	if(res != "")
		return res;
	else {
		// key exists but val is "": deleted
		if(flag)
			return "";

		res = disc.GET(key, flag);
		return res;
	}
}

/**
 * Delete the given key-value pair if it exists.
 * Returns false iff the key is not found.
 */
bool KVStore::del(uint64_t key)
{
	bool flag;
	if(mem.GET(key, flag) != ""){
		// cout << "type0 : " << key << " 1\n";
		mem.DELETE(key);
		return true;
	}

	if(flag){
		// cout << "type1 : " << key << " 0\n";
		return false;
	}
	
	if(disc.GET(key, flag) != ""){
		// cout << "type2 : " << key <<" 1\n";
		mem.PUT(key, "");
		return true;
	}
	else 
	{
		if(flag){
			// cout << "type3 : " << key <<" 0\n";
			return false;
		}
		// cout << "type4 : " << key <<" 0\n";
		return false;
	}
}

/**
 * This resets the kvstore. All key-value pairs should be removed,
 * including memtable and all sstables files.
 */
void KVStore::reset()
{
	mem.MEM_RESET();
	disc.DISK_RESET();
}

void KVStore::compaction(uint64_t key)
{
	MAP_DATA imm_mem = mem.IMM_MEMTABLE();
	
	// minor compaction
	int lvsize = disc.GET_LEVEL_SST(0);
	if(lvsize < 2){
		disc.WRITE_TO_LV0(imm_mem);
		return;
	}

	// major compaction
	// disc.LV_TO_MAP(0, imm_mem);
	// if(key == 1121){
	// 	cout << lvsize << "affa\n";
	// MAP_DATA::iterator iter = imm_mem.begin();
	// while(iter != imm_mem.end()){
	// 	cout << iter->first <<endl;//<< iter->second << endl;
	// 	iter++;
	// }
	// }
	disc.WRITE_TO_LEVELS(imm_mem);
}

#endif // kvstore_h