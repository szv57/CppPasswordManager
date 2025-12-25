#include "Crypto.h"
#include "sodium.h"

#include <stdexcept>


#define KEY_LEN crypto_secretbox_KEYBYTES


std::optional<std::vector<unsigned char>> Crypto::EncryptPassword(const std::string& Password, const std::vector<unsigned char>& Key, const std::vector<unsigned char>& Nonce)
{
	std::vector<unsigned char> EncryptedPass(Password.size() + 16);

	auto UnsignedPass = reinterpret_cast<const unsigned char*>(Password.c_str());

	if (crypto_secretbox_easy(EncryptedPass.data(), UnsignedPass, Password.size(), Nonce.data(), Key.data()) != 0)
	{
		return std::nullopt;
	}

	return EncryptedPass;
}

std::optional<std::string> Crypto::DecryptPassword(const std::vector<unsigned char>& EncryptedPassword, const std::vector<unsigned char>& Key, const std::vector<unsigned char>& Nonce)
{
	std::vector<unsigned char> DecryptedPassVect(EncryptedPassword.size() - crypto_secretbox_MACBYTES);

	if (crypto_secretbox_open_easy(DecryptedPassVect.data(), EncryptedPassword.data(), EncryptedPassword.size(), Nonce.data(), Key.data()) != 0)
	{
		return std::nullopt;
	}

	std::string DecryptedPass(reinterpret_cast<char*>(DecryptedPassVect.data()), DecryptedPassVect.size());

	return DecryptedPass;
}

std::vector<unsigned char> Crypto::DeriveKey(const std::string& MasterPass, const std::vector<unsigned char>& Salt)
{
	std::vector<unsigned char> Key(KEY_LEN);


	if (crypto_pwhash(Key.data(), sizeof Key, MasterPass.c_str(), MasterPass.size(), Salt.data(), crypto_pwhash_OPSLIMIT_INTERACTIVE, crypto_pwhash_MEMLIMIT_INTERACTIVE, crypto_pwhash_ALG_DEFAULT) != 0)
	{
		throw std::runtime_error("ERROR: Could not derive key. (Argon2 error)");
	}
	return Key;
}

std::vector<unsigned char> Crypto::GenerateNonce()
{
	std::vector<unsigned char> Nonce(crypto_secretbox_NONCEBYTES);

	randombytes_buf(Nonce.data(), Nonce.size());	

	return Nonce;
}

std::vector<unsigned char> Crypto::GenerateSalt()
{
	std::vector<unsigned char> Salt(crypto_pwhash_SALTBYTES);

	randombytes_buf(Salt.data(), Salt.size());

	return Salt;

}

std::optional<std::vector<unsigned char>> Crypto::GenerateSentinel(const std::vector<unsigned char>& Key, const std::vector<unsigned char>& SentinelNonce)
{
	std::string Pass = "verified";

	auto tEncrypted = EncryptPassword(Pass, Key, SentinelNonce);

	if (tEncrypted.has_value())
	{
		std::vector<unsigned char> EncryptedSent = tEncrypted.value();

		return EncryptedSent;
	}
	return std::nullopt;
}

bool Crypto::VerifyMaster(const std::vector<unsigned char>& Sentinel, const std::vector<unsigned char>& Key, const std::vector<unsigned char>& Nonce)
{
	std::string Verifier = "verified";

	auto tDecrypted = DecryptPassword(Sentinel, Key, Nonce);

	if (tDecrypted.has_value())
	{
		if (tDecrypted.value() == Verifier)
		{
			return true;
		}
		return false;
	}

	throw std::runtime_error("Failed to decrypt sentinel.");
}


