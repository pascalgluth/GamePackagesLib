#include "GamePackages/Encryption.h"

#include "cryptopp/osrng.h"
#include "cryptopp/modes.h"
#include "cryptopp/base64.h"

namespace gpkg
{

    std::vector<uint8_t> Encryption::EncryptBytes(const std::vector<uint8_t> &data, const std::vector<uint8_t> &key,
                                                  const std::vector<uint8_t> &iv)
    {
        auto aes = CryptoPP::AES::Encryption(key.data(), key.size());
        auto aesCbc = CryptoPP::CBC_Mode_ExternalCipher::Encryption(aes, iv.data());

        std::vector<uint8_t> encryptedData;

        CryptoPP::VectorSource vs(data,
                                  true,
                                  new CryptoPP::StreamTransformationFilter(
                                          aesCbc,
                                          new CryptoPP::Base64Encoder(new CryptoPP::VectorSink(encryptedData))));

        return std::move(encryptedData);
    }

    std::vector<uint8_t> Encryption::DecryptBytes(const std::vector<uint8_t> &data, const std::vector<uint8_t> &key,
                                                  const std::vector<uint8_t> &iv)
    {
        auto aes = CryptoPP::AES::Decryption(key.data(), key.size());
        auto aesCbc = CryptoPP::CBC_Mode_ExternalCipher::Decryption(aes, iv.data());

        std::vector<uint8_t> decryptedData;

        CryptoPP::VectorSource vs(data, true,
                                  new CryptoPP::Base64Decoder(
                                          new CryptoPP::StreamTransformationFilter(
                                                  aesCbc,
                                                  new CryptoPP::VectorSink(decryptedData))
                                  ));

        return std::move(decryptedData);
    }

    void Encryption::GenerateRSA256(std::vector<uint8_t> *outKey, std::vector<uint8_t> *outIV)
    {
        std::vector<uint8_t> key(256 / 8);
        std::vector<uint8_t> iv(CryptoPP::AES::BLOCKSIZE);

#ifdef BLOCKING_RNG_AVAILABLE
        CryptoPP::BlockingRng rng;
#else
        CryptoPP::DefaultAutoSeededRNG rng;
#endif
        rng.GenerateBlock(key.data(), key.size());
        rng.GenerateBlock(iv.data(), iv.size());

        *outKey = key;
        *outIV = iv;
    }
}