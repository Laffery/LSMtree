#pragma once

#ifndef KVSTORE_H
#define KVSTORE_H

#include <string>
#include "kvstore_api.h"
#include "MemTable.hpp"
#include "SkipList.hpp"
#include "SSLevel.hpp"
#include "disk.hpp"

#define MEM_MAX_SIZE 1024
#define LSM_DEPTH 5

using namespace std;

class KVStore : public KVStoreAPI 
{
private:
	string dir_;
	MemTable *mem;
	disk disc;
	
public:
	KVStore(const string &dir);

	~KVStore();

	void put(uint64_t key, const string &s) override;

	std::string get(uint64_t key) override;

	bool del(uint64_t key) override;

	void reset() override;
	
	void compaction();
};

KVStore::KVStore(const string &dir): KVStoreAPI(dir)
{
	dir_ = dir;
	mem  = new MemTable(MEM_MAX_SIZE);
	disc.SET_DEPTH(LSM_DEPTH);
	disc.SET_DIR_PATH(dir_);
}

KVStore::~KVStore()
{
	delete mem;
	disc.FREE_DISK();
}

/**
 * Insert/Update the key-value pair.
 * No return values for simplicity.
 */
void KVStore::put(uint64_t key, const string &s)
{
	mem->PUT(key, s);
	if(mem->IS_FULL())
		compaction();
}
/**
 * Returns the (string) value of the given key.
 * An empty string indicates not found.
 */
std::string KVStore::get(uint64_t key)
{
	return mem->GET(key);
}
/**
 * Delete the given key-value pair if it exists.
 * Returns false iff the key is not found.
 */
bool KVStore::del(uint64_t key)
{
	return false;
}

/**
 * This resets the kvstore. All key-value pairs should be removed,
 * including memtable and all sstables files.
 */
void KVStore::reset()
{
	mem->MEM_RESET();
	disc.DISK_RESET();
}

void KVStore::compaction()
{
	return;
}

#endif // kvstore_h