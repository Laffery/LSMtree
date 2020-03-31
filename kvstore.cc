// #include "kvstore.h"

KVStore::KVStore(const string &dir): KVStoreAPI(dir)
{
	dir_ = dir;
	mem  = new MemTable(MEM_MAX_SIZE);
	// disc = new disk();
}

KVStore::~KVStore()
{
	// delete disc;
}

/**
 * Insert/Update the key-value pair.
 * No return values for simplicity.
 */
void KVStore::put(uint64_t key, const string &s)
{
	memtable.insert(key, s);
	if(memtable->size() == MEM_MAX_SIZE)
		compaction();
	return;
}
/**
 * Returns the (string) value of the given key.
 * An empty string indicates not found.
 */
std::string KVStore::get(uint64_t key)
{
	return "";
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
	return;
}


void KVStore::compaction()
{
	return;
}