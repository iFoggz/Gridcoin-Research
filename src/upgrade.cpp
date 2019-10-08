#include "upgrade.h"
#include "util.h"
#include "ui_interface.h"

#include <algorithm>
#include <univalue.h>
#include <vector>
#include <boost/thread.hpp>

Upgrade::Upgrade()
{

}

void Upgrade::CheckForLatestUpdate()
{
    // If testnet skip this
    if (fTestNet)
        return;

    Http VersionPull;

    std::string GithubResponse = "";

    // We receive the response and it's in a json reply
    UniValue Response(UniValue::VOBJ);
    std::string VersionResponse = VersionPull.GetLatestVersionResponse("https://api.github.com/repos/gridcoin-community/Gridcoin-Research/releases/latest");

    if (VersionResponse.empty())
    {
        LogPrintf("Update Checker: No Response from github");

        return;
    }

    Response.read(VersionResponse);

    // Get the information we need:
    // 'body' for information about changes
    // 'tag_name' for version
    // 'name' for checking if its a mandatory or leisure
    std::string GithubReleaseData = find_value(Response, "tag_name").get_str();
    std::string GithubReleaseTypeData = find_value(Response, "name").get_str();
    std::string GithubReleaseBody = find_value(Response, "body").get_str();
    std::string GithubReleaseType = "";

    if (GithubReleaseTypeData.find("leisure"))
        GithubReleaseType = _("leisure");

    else if (GithubReleaseTypeData.find("mandatory"))
        GithubReleaseType = _("mandatory");

    else
        GithubReleaseType = _("unknown");

    // Parse version data
    std::vector<std::string> GithubVersion;
    std::vector<int> LocalVersion;

    ParseString(GithubReleaseData, '.', GithubVersion);

    LocalVersion.push_back(CLIENT_VERSION_MAJOR);
    LocalVersion.push_back(CLIENT_VERSION_MINOR);
    LocalVersion.push_back(CLIENT_VERSION_REVISION);

    if (GithubVersion.size() != 4)
    {
        LogPrintf("Update Check: Got malformed version (%s)", GithubReleaseData);

        return;
    }

    bool NewVersion = false;

    try {
        // Left to right version numbers.
        // 3 numbers to check for production.
        for (unsigned int x = 0; x < 3; x++)
        {
            if (NewVersion)
                break;

            if (std::stoi(GithubVersion[x]) > LocalVersion[x])
                NewVersion = true;
        }
    }
    catch (std::exception& ex)
    {
        LogPrintf("Update Check: Exception occured checking client version against github version (%s)", ToString(ex.what()));

        return;
    }

    if (!NewVersion)
        return;

    // New version was found
    std::string ClientMessage = _("Local version: ") + strprintf("%d.%d.%d.%d", CLIENT_VERSION_MAJOR, CLIENT_VERSION_MINOR, CLIENT_VERSION_REVISION, CLIENT_VERSION_BUILD) + "\r\n";
    ClientMessage.append(_("Github version: ") + GithubReleaseData + "\r\n");
    ClientMessage.append(_("This update is ") + GithubReleaseType + "\r\n\r\n");
    ClientMessage.append(GithubReleaseBody);
    uiInterface.UpdateMessageBox(ClientMessage);

    return;
}

bool Upgrade::SnapshotRequested()
{
     // Create a thread for the Snapshot download
    boost::thread SnapshotThread(std::bind(&Upgrade::DownloadSnapshot, this)); // thread runs free

    while (!Status.SnapshotDownloadComplete)
    {
        if (Status.SnapshotDownloadFailed)
        {
            LogPrintf("Snapshot Downloader: Failed to download snapshot!");

            return false;
        }

        LogPrintf("Snapshot Downloader: File size %" PRId64, Status.SnapshotDownloadSize);
        if (Status.SnapshotDownloadSpeed < 1000000)
            LogPrintf("Snapshot Downloader: Speed %.0f KB/s", Status.SnapshotDownloadSpeed / (double)1000);

        else
            LogPrintf("Snapshot Downloader: Speed %.0f MB/s", Status.SnapshotDownloadSpeed / (double)1000000);


        LogPrintf("Snapshot Downloader: Progress %.2f", Status.SnapshotDownloadProgress);

        MilliSleep(3000);
    }

    return true;
}

void Upgrade::DownloadSnapshot()
{
     // Download the snapshot.zip
    Http HTTPHandler;

    HTTPHandler.DownloadSnapshot("https://download.gridcoin.us/download/downloadstake/signed/snapshot.zip", GetDataDir().string() + "/snapshot.zip");

    return;
}
