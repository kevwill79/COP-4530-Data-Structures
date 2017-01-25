// Kevin Williams
// COP 4530
// Proj5
// Section 2

#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

template <typename K, typename V>
HashTable<K, V>::HashTable(size_t size) 
//	: currentSize{0}, bucketVector(prime_below(n)) 
{
	bucketVector.getSize(prime_below(size));
	currentSize = 0;
}

template <typename K, typename V>
HashTable<K, V>::~HashTable() 
{
	clear();
}

template <typename K, typename V>
bool HashTable<K, V>::contains(const K & k) const 
{
	auto & whichList = bucketVector[myhash(k)];
	return find(begin(whichList), end(whichList), k) != end(whichList);
}

template <typename K, typename V>
bool HashTable<K, V>::match(const std::pair<K, V>& kv) const 
{
	auto & whichList = bucketVector[myhash(kv.first)];

	return(find(begin(whichList), end(whichList), kv) != end(whichList));
}

template <typename K, typename V>
bool HashTable<K, V>::insert(const std::pair<K, V> & kv) 
{
	auto & whichList = bucketVector[myhash(kv)];
	
	if(find(begin(whichList), end(whichList), kv) != end(whichList))
 		return false;

	whichList.push_back(kv);

 	// Rehash; see Section 5.5
 	if(++currentSize > bucketVector.size())
	rehash();

 	return true;
}

template <typename K, typename V>
bool HashTable<K, V>::insert(std::pair<K, V> && kv) 
{
	auto & whichList = bucketVector[myhash(kv.first)];
	
	if(find(begin(whichList), end(whichList), kv) != end(whichList))
 		return false;

	whichList.push_back(std::move(kv));

 	// Rehash; see Section 5.5
 	if(++currentSize > bucketVector.size())
	rehash();

 	return true;
}

template <typename K, typename V>
bool HashTable<K, V>::remove(const K & k) 
{
	auto & whichList = bucketVector[myhash(k)];

	auto itr = find(begin(whichList), end(whichList), k);

	if(itr == end(whichList))
		return false;

	whichList.erase(itr);
	--currentSize;
	return true;
}

template <typename K, typename V>
void HashTable<K, V>::clear() 
{
	makeEmpty();
}

//---------------------------------------------Redo these 3 functions------------------
template <typename K, typename V>
bool HashTable<K, V>::load(const char *filename) 
{
	std::pair<K,V> kvpair;
	std::ifstream fin;
	fin.open( filename, std::ifstream::in );

	if( !fin )
		return false;

	char wspace;
	while( fin )  {
	// keep reading in kv pairs until reaching the end of file
		std::getline( fin, kvpair.first, ' ' );

		while( isspace( fin.peek() ) )
			fin.get( wspace );
		std::getline( fin, kvpair.second, '\n' );

		while( isspace( fin.peek() ) )
			fin.get( wspace );

		insert( kvpair );
	}

	fin.close();
	return true;
/*	
	ifstream file(filename);
	string readLine;

	while(getline(file, readLine))
	{
		isstringstream s(readLine);
		K key;
		V val;

		s >> key >> val;
		insert(key);
		insert(val);
	}
*/
}

template <typename K, typename V>
void HashTable<K, V>::dump() const 
{
	if(currentSize != 0)
	{
		for(auto & whichlist: bucketVector)
		{
			for(auto itr = whichlist.begin(); itr != whichlist.end(); ++itr)
				std::cout << itr->first << ' ' << itr->second;
		}
	}
}

template <typename K, typename V>
bool HashTable<K, V>::write_to_file(const char *filename) const 
{
	std::ofstream fout;
	fout.open( filename );

	if( !fout )
		return false;

	for( int i = 0; i < bucketVector.size(); i++ )  {
		for( auto itr = bucketVector[i].begin(); itr != bucketVector[i].end(); ++itr )  {
			fout << itr->first << ' ' << itr->second << '\n';
		}
	}
	fout.close();
	return true;
/*
	ofstream out(filename);
	streambuf* coutbuf = cout.rdbuf();
	cout.rdbuf(out.rdbuf());
	return true;
*/
}
//--------------------------------------------------------------------------------------

template <typename K, typename V>
int HashTable<K, V>::getSize()const
{
	return currentSize;
}

//-------------------- Private Helper Functions --------------------

template <typename K, typename V>
void HashTable<K, V>::makeEmpty() 
{
	for(auto & thisList : bucketVector)
		thisList.clear();
}

template <typename K, typename V>
void HashTable<K, V>::rehash()
{
	std::vector<std::list<std::pair<K, V>>> oldLists = bucketVector;

	// Create new double-sized, empty table
	bucketVector.resize(prime_below(2 * bucketVector.size()));

	for(auto & thisList : bucketVector)
		thisList.clear();

	// Copy table over
	currentSize = 0;
	
	for(auto & thisList : oldLists)
		for(auto & kv : thisList)
			insert(std::move(kv));
}

template <typename K, typename V>
size_t HashTable<K, V>::myhash(const K & k) const 
{
	std::hash<K> myhash;
	return myhash(k) % bucketVector.size();
}

// returns largest prime number <= n or zero if input is too large
// This is likely to be more efficient than prime_above(), because
// it only needs a vector of size n
template <typename K, typename V>
unsigned long HashTable<K, V>::prime_below (unsigned long n)
{
	if (n > max_prime)
	{
		std::cerr << "** input too large for prime_below()\n";
		return 0;
	}
	if (n == max_prime)
	{
		return max_prime;
	}
	if (n <= 1)
	{
		std::cerr << "** input too small \n";
		return 0;
	}

	// now: 2 <= n < max_prime
	std::vector <unsigned long> v (n+1);
	setPrimes(v);
	while (n > 2)
	{
		if (v[n] == 1)
			return n;
		--n;
	}

	return 2;
}

//Sets all prime number indexes to 1. Called by method prime_below(n) 
template <typename K, typename V>
void HashTable<K, V>::setPrimes(std::vector<unsigned long>& vprimes)
{
	int i = 0;
	int j = 0;

	vprimes[0] = 0;
	vprimes[1] = 0;
	int n = vprimes.capacity();

	for (i = 2; i < n; ++i)
		vprimes[i] = 1;

	for( i = 2; i*i < n; ++i)
	{
		if (vprimes[i] == 1)
			for(j = i + i ; j < n; j += i)
				vprimes[j] = 0;
	}
}
#endif
