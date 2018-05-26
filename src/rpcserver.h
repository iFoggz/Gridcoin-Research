// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef _BITCOINRPC_H_
#define _BITCOINRPC_H_ 1

#include <string>
#include <list>
#include <map>

class CBlockIndex;

#include "json/json_spirit_reader_template.h"
#include "json/json_spirit_writer_template.h"
#include "json/json_spirit_utils.h"

#include "global_objects_noui.hpp"
#include "util.h"
#include "checkpoints.h"

json_spirit::Object JSONRPCError(int code, const std::string& message);

void StopRPCThreads();
void ThreadRPCServer(void* parg);
int CommandLineRPC(int argc, char *argv[]);

/*
  Type-check arguments; throws JSONRPCError if wrong type given. Does not check that
  the right number of arguments are passed, just that any passed are the correct type.
  Use like:  RPCTypeCheck(params, boost::assign::list_of(str_type)(int_type)(obj_type));
*/
void RPCTypeCheck(const json_spirit::Array& params,
                  const std::list<json_spirit::Value_type>& typesExpected, bool fAllowNull=false);
/*
  Check for expected keys/value types in an Object.
  Use like: RPCTypeCheck(object, boost::assign::map_list_of("name", str_type)("value", int_type));
*/
void RPCTypeCheck(const json_spirit::Object& o,
                  const std::map<std::string, json_spirit::Value_type>& typesExpected, bool fAllowNull=false);

typedef json_spirit::Value(*rpcfn_type)(const json_spirit::Array& params, bool fHelp);

enum rpccategory
{
    cat_null,
    cat_wallet,
    cat_mining,
    cat_developer,
    cat_network
};

class CRPCCommand
{
public:
    std::string name;
    rpcfn_type actor;
    bool okSafeMode;
    rpccategory category;
};

/**
 * Bitcoin RPC command dispatcher.
 */
class CRPCTable
{
private:
    std::map<std::string, const CRPCCommand*> mapCommands;
public:
    CRPCTable();
    const CRPCCommand* operator[](std::string name) const;
    std::string help(std::string name, rpccategory category) const;

    /**
     * Execute a method.
     * @param method   Method to execute
     * @param params   Array of arguments (JSON objects)
     * @returns Result of the call.
     * @throws an exception (json_spirit::Value) when an error happens.
     */
    json_spirit::Value execute(const std::string &method, const json_spirit::Array &params) const;
};

extern const CRPCTable tableRPC;

extern int64_t nWalletUnlockTime;
extern int64_t AmountFromValue(const json_spirit::Value& value);
extern json_spirit::Value ValueFromAmount(int64_t amount);
extern double GetDifficulty(const CBlockIndex* blockindex = NULL);

extern std::string HelpRequiringPassphrase();
extern void EnsureWalletIsUnlocked();

//
// Utilities: convert hex-encoded Values
// (throws error if not hex).
//
extern uint256 ParseHashV(const json_spirit::Value& v, std::string strName);
extern uint256 ParseHashO(const json_spirit::Object& o, std::string strKey);
extern std::vector<unsigned char> ParseHexV(const json_spirit::Value& v, std::string strName);

// Rpc reordered by category

// Deprecated
extern json_spirit::Value listitem(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value execute(const json_spirit::Array& params, bool fHelp);

// Wallet
extern json_spirit::Value addmultisigaddress(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value addredeemscript(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value backupprivatekeys(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value backupwallet(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value burn(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value burn2(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value checkwallet(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value createrawtransaction(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value decoderawtransaction(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value decodescript(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value dumpprivkey(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value dumpwallet(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value encrypt(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value encryptwallet(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getaccount(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getaccountaddress(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getaddressesbyaccount(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getbalance(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getnewaddress(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getnewpubkey(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getrawtransaction(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getreceivedbyaccount(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getreceivedbyaddress(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value gettransaction(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getwalletinfo(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value importprivkey(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value importwallet(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value keypoolrefill(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value listaccounts(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value listaddressgroupings(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value listreceivedbyaccount(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value listreceivedbyaddress(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value listsinceblock(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value listtransactions(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value listunspent(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value makekeypair(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value movecmd(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value newburnaddress(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value rain(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value repairwallet(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value resendtx(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value reservebalance(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value sendfrom(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value sendmany(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value sendrawtransaction(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value sendtoaddress(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value setaccount(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value settxfee(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value signmessage(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value signrawtransaction(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value unspentreport(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value validateaddress(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value validatepubkey(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value verifymessage(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value walletlock(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value walletpassphrase(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value walletpassphrasechange(const json_spirit::Array& params, bool fHelp);

//Mining
extern json_spirit::Value advertisebeacon(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value beaconreport(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value beaconstatus(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value cpids(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value currentneuralhash(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value currentneuralreport(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value explainmagnitude(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getmininginfo(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value lifetime(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value magnitude(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value mymagnitude(const json_spirit::Array& params, bool fHelp);
#ifdef WIN32
extern json_spirit::Value myneuralhash(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value neuralhash(const json_spirit::Array& params, bool fHelp);
#endif
extern json_spirit::Value neuralreport(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value proveownership(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value resetcpids(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value rsa(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value rsaweight(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value staketime(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value superblockage(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value superblocks(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value syncdpor2(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value upgradedbeaconreport(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value validcpids(const json_spirit::Array& params, bool fHelp);

// Developers
extern json_spirit::Value addkey(const json_spirit::Array& params, bool fHelp);
#ifdef WIN32
extern json_spirit::Value currentcontractaverage(const json_spirit::Array& params, bool fHelp);
#endif
extern json_spirit::Value debug(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value debug10(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value debug2(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value debug3(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value debug4(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value debugnet(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value dportally(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value forcequorom(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value gatherneuralhashes(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value genboinckey(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value rpc_getblockstats(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getlistof(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getnextproject(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value listdata(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value memorizekeys(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value network(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value neuralrequest(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value projects(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value readconfig(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value readdata(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value refhash(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value rpc_reorganize(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value seefile(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value sendalert(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value sendalert2(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value sendblock(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value sendrawcontract(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value superblockaverage(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value tally(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value tallyneural(const json_spirit::Array& params, bool fHelp);
#ifdef WIN32
extern json_spirit::Value testnewcontract(const json_spirit::Array& params, bool fHelp);
#endif
extern json_spirit::Value updatequoromdata(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value versionreport(const json_spirit::Array& params, bool fhelp);
extern json_spirit::Value writedata(const json_spirit::Array& params, bool fHelp);

// Network
extern json_spirit::Value addnode(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value addpoll(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value askforoutstandingblocks(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value currenttime(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value decryptphrase(const json_spirit::Array& params, bool fHelp);
//extern json_spirit::Value downloadblocks(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value encryptphrase(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getaddednodeinfo(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getbestblockhash(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getblock(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getblockbynumber(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getblockchaininfo(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getblockcount(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getblockhash(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getcheckpoint(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getconnectioncount(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getdifficulty(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getinfo(const json_spirit::Array& params, bool fHelp); // To Be Deprecated --> getblockchaininfo getnetworkinfo getwalletinfo
extern json_spirit::Value getnettotals(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getnetworkinfo(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getpeerinfo(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getrawmempool(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value listallpolls(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value listallpolldetails(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value listpolldetails(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value listpollresults(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value listpolls(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value memorypool(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value networktime(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value ping(const json_spirit::Array& params, bool fHelp);
#ifdef WIN32
extern json_spirit::Value reindex(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value restart(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value restorepoint(const json_spirit::Array& params, bool fHelp);
#endif
extern json_spirit::Value showblock(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value vote(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value votedetails(const json_spirit::Array& params, bool fHelp);

extern json_spirit::Value rpc_getsupervotes(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value rpc_exportstats(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value rpc_getrecentblocks(const json_spirit::Array& params, bool fHelp);

#endif

unsigned short GetDefaultRPCPort();
