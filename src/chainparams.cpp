// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Copyright (c) 2017-2018 The Phore developers
// Copyright (c) 2018-2019 The Monetaryunit developers
// Copyright (c) 2020 The Bare developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "random.h"
#include "bignum.h"
#include "util.h"
#include "utilstrencodings.h"
#include "streams.h"
#include "clientversion.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */
static bool regenerate = false;

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (      0, uint256("30ef80527cafd9e8685412f85e30efc2ffbfa15398c2852fe5ba0ace7f6cb741"))
	(  46028, uint256("d7b8aa83b193e4dfa7ba0b663ee0188a5f29b5a06bda34901c698aa7c7b5c245"))
	(  95015, uint256("5ef62dd9404ab16a2b810011cd2eead361280a43557945df1d44cf51b8edfc3b"))
	( 151978, uint256("a80a559478d02adc7130d5859dcd9847561c1ab84dd7750ba1a604b817a77c62"))
	( 218512, uint256("6f4064590ac7d130a56a32bcec43d578c2b1ae51eb440636d4b019874dbeceef"))
	( 313388, uint256("787f8254c99998b65dc7fc66065f8134be33b36fe331147cd0747295dd8b6325"))
	( 468790, uint256("79e10eb4729fe00b97197c13ec23ba20d9d86c47c5e005cb8f9adf3f849f56cb"))
	( 500175, uint256("68d5f1216a1fbf44c29f9d1c24afe22abc70cbc769989b1da3666cc818b606be"))
	( 964891, uint256("52913ee603c8e7f220fd4beb0591272bda859b8e6438c3f1b0abaefab243289a"));

static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1642437487, // * UNIX timestamp of last checkpoint block
    2053000,     // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    1600        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("554ec25d2f508143b7b137727a0f04052c405b01a1e3a2a42eebf943c316266d"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1581171337,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("6a7fd108b4ef0aab9ce43734d66a04dc83ac4c49a5b3a3e23387dbfcdcaca765"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1581171338,
    0,
    100};

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x25;
        pchMessageStart[1] = 0x65;
        pchMessageStart[2] = 0x72;
        pchMessageStart[3] = 0x34;
        vAlertPubKey = ParseHex("044bcdbe70298b5521ca91a85b97f95212f123b3b68afc8b1903d24e98537f0a824db22b41345b29e81d266c24437382c19cc27191049c2473c9f500f9addd6a41");
        nDefaultPort = 32201;
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 24 * 60 * 60; // BARE: 1 day
        nTargetSpacing = 1 * 60;  // BARE: 60 seconds
        nMaturity = 50;
        nMasternodeCountDrift = 20;
        nMaxMoneyOut = 1410000 * COIN;

        /** Height or Time Based Activations **/
        nLastPOWBlock = 500;
        nModifierUpdateBlock = 1;

        /**
         * Build the genesis block. Note that the output of the genesis coinbase cannot
         * be spent as it did not originally exist in the database.
         *
         * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
         *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
         *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
         *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
         *   vMerkleTree: e0028e
         */
        const char* pszTimestamp = "BARE v2, The Adult Coin - Bitcoin Block 621074 - 000000000000000000020f62ea032afcca69a64f1e012f63a1a1aa9a486d1e66";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].SetEmpty();
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1584227334; // Saturday, March 14, 2020 11:08:54 PM GMT
        genesis.nBits = 0x207fffff;;
        genesis.nNonce = 0;

        hashGenesisBlock = genesis.GetHash();
        if (regenerate) {
            hashGenesisBlock = uint256S("");
            genesis.nNonce = 0;
            if (true && (genesis.GetHash() != hashGenesisBlock)) {
                uint256 hashTarget = CBigNum().SetCompact(genesis.nBits).getuint256();
                while (genesis.GetHash() > hashTarget)
                {
                    ++genesis.nNonce;
                    if (genesis.nNonce == 0)
                    {
                        ++genesis.nTime;
                    }
                }
                std::cout << "// Mainnet ---";
                std::cout << " nonce: " << genesis.nNonce;
                std::cout << " time: " << genesis.nTime;
                std::cout << " hash: 0x" << genesis.GetHash().ToString().c_str();
                std::cout << " merklehash: 0x"  << genesis.hashMerkleRoot.ToString().c_str() <<  "\n";
            }
        } else {
            LogPrintf("Mainnet ---\n");
            LogPrintf(" nonce: %u\n", genesis.nNonce);
            LogPrintf(" time: %u\n", genesis.nTime);
            LogPrintf(" hash: 0x%s\n", genesis.GetHash().ToString().c_str());
            LogPrintf(" merklehash: 0x%s\n", genesis.hashMerkleRoot.ToString().c_str());
            assert(hashGenesisBlock == uint256("0x30ef80527cafd9e8685412f85e30efc2ffbfa15398c2852fe5ba0ace7f6cb741"));
            assert(genesis.hashMerkleRoot == uint256("0xbfe8624eaa27b3eb3f6258bca5f866147660de77582088229d865399c9d4c3eb"));
        }

// Mainnet --- nonce: 0 time: 1584227334 hash: 0x30ef80527cafd9e8685412f85e30efc2ffbfa15398c2852fe5ba0ace7f6cb741 merklehash: 0xbfe8624eaa27b3eb3f6258bca5f866147660de77582088229d865399c9d4c3eb

        vSeeds.push_back(CDNSSeedData("dns1", "dns01.bare.network"));
        vSeeds.push_back(CDNSSeedData("dns2", "dns02.bare.network"));
        vSeeds.push_back(CDNSSeedData("dns3", "dns03.bare.network"));
        vSeeds.push_back(CDNSSeedData("dns4", "dns04.bare.network"));
		vSeeds.push_back(CDNSSeedData("onion", "zxsow7urhijswnvp.onion"));
		
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 25);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 16);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 110);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
        // 	BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md //used: 475   | 0x800001db | BARE   | [BARE Network](https://bare.network) 
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x01)(0xDB).convert_to_container<std::vector<unsigned char> >();

        bech32_hrp = "bare";

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "048a6bf259eac7886037b9daad4d43856eeab0c1408671436f1f24a067d8dadf79ef721f0eb053b5444e01932387e2c6c03466bf0dbbdeba84302434fd3e28b077";
        strSporkKeyTemp = "048a6bf259eac7886037b9daad4d43856eeab0c1408671436f1f24a067d8dadf79ef721f0eb053b5444e01932387e2c6c03466bf0dbbdeba84302434fd3e28b077";
        strObfuscationPoolDummyAddress = "BPTA3JSwXuzHWs56xU7v1ezAWoBFeYXmKV";
        nBudgetFeeConfirmations = 6; // Number of confirmations for the finalization fee
		
		strBootstrapUrl = "https://bootstrap.bare.network/v2/mainnet"; // location of bare bootstrap file
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0xce;
        pchMessageStart[1] = 0xff;
        pchMessageStart[2] = 0xca;
        pchMessageStart[3] = 0x44;
        vAlertPubKey = ParseHex("046dce17c38e0f92e6aaa7b744aaaa8be89a007a2e66d62b3944c659239d87e3721bd429a7f1f7cdf02763f3aaa7e8d6f4ac9541f6b9aa78b21b9b1e12fab307b8");
        nDefaultPort = 32203;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 6 * 60 * 60; // BARE: 6 hours
        nTargetSpacing = 1 * 30;  // BARE: 30 seconds
        nMaturity = 15;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 51197; //approx Mon, 17 Apr 2017 04:00:00 GMT
        nMaxMoneyOut = 1500000 * COIN;
        nLastPOWBlock = 250;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1581171337; // Saturday, February 8, 2020 3:15:37 PM GMT+01:00
        genesis.nNonce = 0;

        hashGenesisBlock = genesis.GetHash();
        if (regenerate) {
            hashGenesisBlock = uint256S("");
            genesis.nNonce = 0;
            if (true && (genesis.GetHash() != hashGenesisBlock)) {
                uint256 hashTarget = CBigNum().SetCompact(genesis.nBits).getuint256();
                while (genesis.GetHash() > hashTarget)
                {
                    ++genesis.nNonce;
                    if (genesis.nNonce == 0)
                    {
                        ++genesis.nTime;
                    }
                }
                std::cout << "// Testnet ---";
                std::cout << " nonce: " << genesis.nNonce;
                std::cout << " time: " << genesis.nTime;
                std::cout << " hash: 0x" << genesis.GetHash().ToString().c_str();
                std::cout << " merklehash: 0x"  << genesis.hashMerkleRoot.ToString().c_str() <<  "\n";

            }
        } else {
            LogPrintf("Testnet ---\n");
            LogPrintf(" nonce: %u\n", genesis.nNonce);
            LogPrintf(" time: %u\n", genesis.nTime);
            LogPrintf(" hash: 0x%s\n", genesis.GetHash().ToString().c_str());
            LogPrintf(" merklehash: 0x%s\n", genesis.hashMerkleRoot.ToString().c_str());
            assert(hashGenesisBlock == uint256("0x554ec25d2f508143b7b137727a0f04052c405b01a1e3a2a42eebf943c316266d"));
            assert(genesis.hashMerkleRoot == uint256("0xbfe8624eaa27b3eb3f6258bca5f866147660de77582088229d865399c9d4c3eb"));
        }

// Testnet --- nonce: 0 time: 1581171337 hash: 0x554ec25d2f508143b7b137727a0f04052c405b01a1e3a2a42eebf943c316266d merklehash: 0xbfe8624eaa27b3eb3f6258bca5f866147660de77582088229d865399c9d4c3eb

        vFixedSeeds.clear();
        vSeeds.clear();

        vSeeds.push_back(CDNSSeedData("testnetdns", "testnetdns.bare.network"));


        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 139); // Testnet bare addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);  // Testnet bare script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet bare BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
        // Testnet bare BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
        // Testnet bare BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        bech32_hrp = "tbare";

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKeyTemp = "044555edf92189a03a509d89ab69e76148c7b4f59291e2195736b43e968694a80e11e927c8cd1dd605bc3c6809a1cadb330d230fb7a3d4a98833c5940641895abd";
        strSporkKey = "044555edf92189a03a509d89ab69e76148c7b4f59291e2195736b43e968694a80e11e927c8cd1dd605bc3c6809a1cadb330d230fb7a3d4a98833c5940641895abd";
        strObfuscationPoolDummyAddress = "7vRzZ63yCrXCf8C8sXnCuLbLf4L2kemrLkmF4MJp22JVG93VHdi";
        nBudgetFeeConfirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                     // here because we only have a 8 block finalization window on testnet
		
		strBootstrapUrl = "https://bootstrap.bare.network/v2/testnet"; // location of bare bootstrap file
		
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        pchMessageStart[0] = 0x54;
        pchMessageStart[1] = 0x14;
        pchMessageStart[2] = 0x64;
        pchMessageStart[3] = 0x95;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // BARE: 1 day
        nTargetSpacing = 1 * 60;        // BARE: 1 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1581171338; // Saturday, February 8, 2020 3:15:38 PM GMT+01:00
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 1;
        nMaturity = 0;
        nLastPOWBlock = 999999999; // PoS complicates Regtest because of timing issues
        nDefaultPort = 32205;

        hashGenesisBlock = genesis.GetHash();
        if (regenerate) {
            hashGenesisBlock = uint256S("");
            if (true && (genesis.GetHash() != hashGenesisBlock)) {
                uint256 hashTarget = CBigNum().SetCompact(genesis.nBits).getuint256();
                while (genesis.GetHash() > hashTarget)
                {
                    ++genesis.nNonce;
                    if (genesis.nNonce == 0)
                    {
                        ++genesis.nTime;
                    }
                }
                std::cout << "// Regtestnet ---";
                std::cout << " nonce: " << genesis.nNonce;
                std::cout << " time: " << genesis.nTime;
                std::cout << " hash: 0x" << genesis.GetHash().ToString().c_str();
                std::cout << " merklehash: 0x"  << genesis.hashMerkleRoot.ToString().c_str() <<  "\n";

            }
        } else {
            LogPrintf("Regtestnet ---\n");
            LogPrintf(" nonce: %u\n", genesis.nNonce);
            LogPrintf(" time: %u\n", genesis.nTime);
            LogPrintf(" hash: 0x%s\n", genesis.GetHash().ToString().c_str());
            LogPrintf(" merklehash: 0x%s\n", genesis.hashMerkleRoot.ToString().c_str());
            assert(hashGenesisBlock == uint256("0x6a7fd108b4ef0aab9ce43734d66a04dc83ac4c49a5b3a3e23387dbfcdcaca765"));
            assert(genesis.hashMerkleRoot == uint256("0xbfe8624eaa27b3eb3f6258bca5f866147660de77582088229d865399c9d4c3eb"));
        }

// Regtestnet --- nonce: 1 time: 1581171338 hash: 0x6a7fd108b4ef0aab9ce43734d66a04dc83ac4c49a5b3a3e23387dbfcdcaca765 merklehash: 0xbfe8624eaa27b3eb3f6258bca5f866147660de77582088229d865399c9d4c3eb

        if (regenerate)
            exit(0);

        bech32_hrp = "bart";

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        // {
        //     "PrivateKey": "0411cbde71670d7fa0b9e29a29aac09bf5b0dc6566b4b30fb74785e4729e2156",
        //     "PublicKey": "041d6a87defe52522360ce74e5215df97fd3e167b71509b883c6c43f65666b15b5e7897cab47331d48bf50352b8d0d6d51769c12a422126ce5087e4b605de83cb9"
        // }
        strSporkKey = "041d6a87defe52522360ce74e5215df97fd3e167b71509b883c6c43f65666b15b5e7897cab47331d48bf50352b8d0d6d51769c12a422126ce5087e4b605de83cb9";
        strSporkKeyTemp = "041d6a87defe52522360ce74e5215df97fd3e167b71509b883c6c43f65666b15b5e7897cab47331d48bf50352b8d0d6d51769c12a422126ce5087e4b605de83cb9";
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 32207;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

bool ParamsSelected()
{
    return pCurrentParams != nullptr;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}

uint64_t GetBlockChainSize()
{
    const uint64_t GB_BYTES = 1000000000LL;
    return 1LL * GB_BYTES;
}

bool VerifyGenesisBlock(const std::string& datadir, const uint256& genesisHash, std::string& err)
{
    const std::string path = strprintf("%s/blocks/blk00000.dat", datadir);
    FILE *fptr = fopen(path.c_str(), "rb");
    if (!fptr) {
        err = strprintf("Failed to open file: %s", path);
        return false;
    }

    CAutoFile filein(fptr, SER_DISK, CLIENT_VERSION);
    if (filein.IsNull()) {
        err = strprintf("Open block file failed: %s", path);
        return false;
    }

    char buf[MESSAGE_START_SIZE] = {0};
    filein.read(buf, MESSAGE_START_SIZE);
    if (memcmp(buf, Params().MessageStart(), MESSAGE_START_SIZE)) {
        err = strprintf("Invalid magic numer %s in the file: %s", HexStr(buf, buf + MESSAGE_START_SIZE), path);
        return false;
    }

    unsigned int nSize = 0;
    filein >> nSize;
    if (nSize < 80 || nSize > 2000000) {
        err = strprintf("Invalid block size %u in the file: %s", nSize, path);
        return false;
    }

    CBlock block;
    try {

        /** Read block */
        filein >> block;
    } catch (std::exception& e) {
        err = strprintf("Deserialize or I/O error: %s", e.what());
        return false;
    }

    /** Check block hash */
    if (block.GetHash() != genesisHash) {
        err = strprintf("Block hash %s does not match genesis block hash %s", block.GetHash().ToString(), genesisHash.ToString());
        return false;
    } else {
        return true;
    }
}
