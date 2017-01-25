#include "passserver.h"
#include "hashtable.h"


PassServer::PassServer(size_t size) : passwords(size)
{

}

PassServer::~PassServer()
{
	passwords.clear();	
}

//Checks if user is in hashtable
bool PassServer::find( const string & user) const
{
	return passwords.contains(user);
}

//Adds a new user to the hashtable
bool PassServer::addUser(const pair<string, string> & kv)
{
	string encryptPword = encrypt(kv.second);

	pair<string, string> encryptKV(kv.first, encryptPword);

	return passwords.insert(encryptKV);
}

//Adds a new user to the hashtable
bool PassServer::addUser(pair<string, string> && kv)
{

    pair<string, string> encryptKV(std::move(kv));

	return addUser(encryptKV);
}

//Removes a user from the hashtable
bool PassServer::removeUser(const string & k)
{
	passwords.remove(k);
}

//Changes an exisiting user's password
bool PassServer::changePassword(const pair<string, string> & p, const string& newpasswd)
{
	if(p.second == newpasswd)
		return false;
	else if(!passwords.contains(p.first))
		return false;
	else
	{
		string oldPword = encrypt(p.second);
		string newPword = encrypt(newpasswd);

		pair<string, string> oldKV(p.first, oldPword);

		if(!passwords.match(oldKV))
			return false;
		else
		{
			pair<string, string> newKV(p.first, newPword);
			passwords.insert(newKV);

			return true;
		}
	}
}

//Loads hashtable entries from a file
bool PassServer::load(const char * filename)
{
	return passwords.load(filename);
}

//Dumps the hashtable structure and entries to screen
void PassServer::dump () const
{
	passwords.dump();
}

//Dumps the hashtable entries into a file
bool PassServer::write_to_file (const char * filename) const
{
	passwords.write_to_file(filename);
}

//Returns the size of the hashtable
size_t PassServer::size() const
{
	passwords.getSize();
}

//Encrypts the password
string PassServer::encrypt(const string & str)
{
	char salt[] = "$1$########";
	string cryptPword = crypt(str.c_str(), salt);

	cryptPword.replace(0, 12, "");
	return cryptPword;
}
