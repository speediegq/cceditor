/* cceditor
 * Geometry Dash save editor
 * Licensed under the GNU General Public License version 3.0.
 *
 * See included LICENSE file for copyright and license details.
 */

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <boost/regex.hpp>
#include <wx/wx.h>
#include "CCData.hpp"

class CCEditor : public wxApp {
    public:
        bool OnInit() override;
};

wxIMPLEMENT_APP(CCEditor);

class Window : public wxFrame {
    public:
        Window();
        std::string currentFile{};
        std::vector<uint8_t> fileContents{};
    private:
        std::string theUser{};
        std::string statJumps{};
        std::string statAttempts{};
        std::string statOfficialLevels{};
        std::string statOnlineLevels{};
        std::string statDemons{};
        std::string statStars{};
        std::string statMapPacks{};
        std::string statSecretCoins{};
        std::string statDestroyed{};
        std::string statLiked{};
        std::string statRated{};
        std::string statUserCoins{};
        std::string statDiamonds{};
        std::string statOrbs{};
        std::string statDaily{};
        std::string statFireShard{};
        std::string statIceShard{};
        std::string statPoisonShard{};
        std::string statShadowShard{};
        std::string statLavaShard{};
        std::string statDemonKeys{};
        std::string statTotalOrbs{};

        wxTextCtrl* jumpText;
        wxTextCtrl* attemptText;
        wxTextCtrl* officialLevelText;
        wxTextCtrl* onlineLevelText;
        wxTextCtrl* demonsText;
        wxTextCtrl* starsText;
        wxTextCtrl* mapPacksText;
        wxTextCtrl* secretCoinsText;
        wxTextCtrl* destroyedText;
        wxTextCtrl* likedText;
        wxTextCtrl* ratedText;
        wxTextCtrl* userCoinsText;
        wxTextCtrl* diamondsText;
        wxTextCtrl* orbsText;
        wxTextCtrl* dailyText;
        wxTextCtrl* fireShardText;
        wxTextCtrl* iceShardText;
        wxTextCtrl* poisonShardText;
        wxTextCtrl* shadowShardText;
        wxTextCtrl* lavaShardText;
        wxTextCtrl* demonKeysText;
        wxTextCtrl* totalOrbsText;

        wxStaticText* jumpLabel;
        wxStaticText* attemptLabel;
        wxStaticText* officialLevelLabel;
        wxStaticText* onlineLevelLabel;
        wxStaticText* demonsLabel;
        wxStaticText* starsLabel;
        wxStaticText* mapPacksLabel;
        wxStaticText* secretCoinsLabel;
        wxStaticText* destroyedLabel;
        wxStaticText* likedLabel;
        wxStaticText* ratedLabel;
        wxStaticText* userCoinsLabel;
        wxStaticText* diamondsLabel;
        wxStaticText* orbsLabel;
        wxStaticText* dailyLabel;
        wxStaticText* fireShardLabel;
        wxStaticText* iceShardLabel;
        wxStaticText* poisonShardLabel;
        wxStaticText* shadowShardLabel;
        wxStaticText* lavaShardLabel;
        wxStaticText* demonKeysLabel;
        wxStaticText* totalOrbsLabel;

        void onOpen(wxCommandEvent& event);
        void onSave(wxCommandEvent& event);
        void onSaveAs(wxCommandEvent& event);
        void onChange(wxCommandEvent& event);
        void onEncode(wxCommandEvent& event);
        void onDecode(wxCommandEvent& event);
        void onExit(wxCommandEvent& event);
        void onAbout(wxCommandEvent& event);

        wxDECLARE_EVENT_TABLE();
};

enum {
    ID_Encode = 3,
    ID_Decode = 2,
    ID_Change = 4,
};

bool CCEditor::OnInit() {
    Window *frame = new Window();
    frame->Show(true);
    return true;
}

Window::Window() : wxFrame(nullptr, wxID_ANY, "cceditor") {
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(wxID_OPEN, "&Open\tCtrl-O", "Open a file to edit.");
    menuFile->Append(wxID_SAVE, "&Save\tCtrl-S", "Save.");
    menuFile->Append(wxID_SAVEAS, "&Save As\tCtrl-Shift-S", "Save as.");
    menuFile->Append(ID_Encode, "&Encode\tCtrl-E", "Open a file to encode.");
    menuFile->Append(ID_Decode, "&Decode\tCtrl-D", "Open a file to decode.");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);

    wxMenuItem *saveMenuItem = menuFile->FindItem(wxID_SAVE);
    wxMenuItem *saveAsMenuItem = menuFile->FindItem(wxID_SAVEAS);

    if (!currentFile.compare("")) {
        saveMenuItem->Enable(false);
        saveAsMenuItem->Enable(false);
    }

    CreateStatusBar();

    Bind(wxEVT_MENU, &Window::onOpen, this, wxID_OPEN);
    Bind(wxEVT_MENU, &Window::onSave, this, wxID_SAVE);
    Bind(wxEVT_MENU, &Window::onSaveAs, this, wxID_SAVEAS);
    Bind(wxEVT_MENU, &Window::onEncode, this, ID_Encode);
    Bind(wxEVT_MENU, &Window::onDecode, this, ID_Decode);
    Bind(wxEVT_MENU, &Window::onAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &Window::onExit, this, wxID_EXIT);
}

void Window::onExit(wxCommandEvent& event) {
    Close(true);
}

void Window::onAbout(wxCommandEvent& event) {
    wxMessageBox("cceditor\n\nLicensed under the GNU General Public License version 3.\nCopyright (c) 2024 speedie\nhttps://git.speedie.site/speedie/cceditor", "About cceditor", wxOK | wxICON_INFORMATION);
}

void Window::onOpen(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, _("Open CCGameManager.dat"), "", "", "CCGameManager.dat (*.dat)|*.dat|All files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }

    currentFile = openFileDialog.GetPath().ToStdString();

    wxMenuItem *saveMenuItem = GetMenuBar()->FindItem(wxID_SAVE);
    wxMenuItem *saveAsMenuItem = GetMenuBar()->FindItem(wxID_SAVEAS);

    if (saveMenuItem) {
        saveMenuItem->Enable(true);
    }

    if (saveAsMenuItem) {
        saveAsMenuItem->Enable(true);
    }

    try {
        CCData::CCData data;
        std::ifstream file(currentFile, std::ios::binary);
        std::vector<uint8_t> theFileContents(std::istreambuf_iterator<char>(file), {});

        fileContents = data.Decrypt(theFileContents);
    } catch (const std::exception& e) {
        wxMessageBox("Failed to decode the file!", "Error", wxICON_ERROR | wxOK, this);
    }

    std::string file(fileContents.begin(), fileContents.end());

    // get the username to be displayed in the status
    boost::smatch umatches;
    if (boost::regex_match(file, umatches, boost::regex(".*<k>playerName<\\/k><s>(.*?)<\\/s>.*"))) {
        if (umatches.size() > 1) {
            theUser = umatches[1].str();
        }
    }

    // this is such a hack, but i hate all xml libs so it will do
    boost::smatch matches;
    if (boost::regex_match(file, matches, boost::regex(".*<k>GS_value<\\/k><d><k>1<\\/k><s>(.*?)<\\/s><k>2<\\/k><s>(.*?)<\\/s><k>3<\\/k><s>(.*?)<\\/s><k>4<\\/k><s>(.*?)<\\/s><k>5<\\/k><s>(.*?)<\\/s><k>6<\\/k><s>(.*?)<\\/s><k>7<\\/k><s>(.*?)<\\/s><k>8<\\/k><s>(.*?)<\\/s><k>9<\\/k><s>(.*?)<\\/s><k>10<\\/k><s>(.*?)<\\/s><k>11<\\/k><s>(.*?)<\\/s><k>12<\\/k><s>(.*?)<\\/s><k>13<\\/k><s>(.*?)<\\/s><k>14<\\/k><s>(.*?)<\\/s><k>15<\\/k><s>(.*?)<\\/s><k>16<\\/k><s>(.*?)<\\/s><k>17<\\/k><s>(.*?)<\\/s><k>18<\\/k><s>(.*?)<\\/s><k>19<\\/k><s>(.*?)<\\/s><k>20<\\/k><s>(.*?)<\\/s><k>21<\\/k><s>(.*?)<\\/s><k>22<\\/k><s>(.*?)<\\/s>.*"))) {
        if (matches.size() > 1) {
            statJumps = matches[1].str();
            statAttempts = matches[2].str();
            statOfficialLevels = matches[3].str();
            statOnlineLevels = matches[4].str();
            statDemons = matches[5].str();
            statStars = matches[6].str();
            statMapPacks = matches[7].str();
            statSecretCoins = matches[8].str();
            statDestroyed = matches[9].str();
            statLiked = matches[10].str();
            statRated = matches[11].str();
            statUserCoins = matches[12].str();
            statDiamonds = matches[13].str();
            statOrbs = matches[14].str();
            statDaily = matches[15].str();
            statFireShard = matches[16].str();
            statIceShard = matches[17].str();
            statPoisonShard = matches[18].str();
            statShadowShard = matches[19].str();
            statLavaShard = matches[20].str();
            statDemonKeys = matches[21].str();
            statTotalOrbs = matches[22].str();
        }
    }

    SetStatusText("Editing '" + currentFile + "' for user '" + theUser + "'");

    jumpText = new wxTextCtrl(this, wxID_ANY, statJumps, wxDefaultPosition, wxDefaultSize, 0);
    attemptText = new wxTextCtrl(this, wxID_ANY, statAttempts, wxDefaultPosition, wxDefaultSize, 0);
    officialLevelText = new wxTextCtrl(this, wxID_ANY, statOfficialLevels, wxDefaultPosition, wxDefaultSize, 0);
    onlineLevelText = new wxTextCtrl(this, wxID_ANY, statOnlineLevels, wxDefaultPosition, wxDefaultSize, 0);
    demonsText = new wxTextCtrl(this, wxID_ANY, statDemons, wxDefaultPosition, wxDefaultSize, 0);
    starsText = new wxTextCtrl(this, wxID_ANY, statStars, wxDefaultPosition, wxDefaultSize, 0);
    mapPacksText = new wxTextCtrl(this, wxID_ANY, statMapPacks, wxDefaultPosition, wxDefaultSize, 0);
    secretCoinsText = new wxTextCtrl(this, wxID_ANY, statSecretCoins, wxDefaultPosition, wxDefaultSize, 0);
    destroyedText = new wxTextCtrl(this, wxID_ANY, statDestroyed, wxDefaultPosition, wxDefaultSize, 0);
    likedText = new wxTextCtrl(this, wxID_ANY, statLiked, wxDefaultPosition, wxDefaultSize, 0);
    ratedText = new wxTextCtrl(this, wxID_ANY, statRated, wxDefaultPosition, wxDefaultSize, 0);
    userCoinsText = new wxTextCtrl(this, wxID_ANY, statUserCoins, wxDefaultPosition, wxDefaultSize, 0);
    diamondsText = new wxTextCtrl(this, wxID_ANY, statDiamonds, wxDefaultPosition, wxDefaultSize, 0);
    orbsText = new wxTextCtrl(this, wxID_ANY, statOrbs, wxDefaultPosition, wxDefaultSize, 0);
    dailyText = new wxTextCtrl(this, wxID_ANY, statDaily, wxDefaultPosition, wxDefaultSize, 0);
    fireShardText = new wxTextCtrl(this, wxID_ANY, statFireShard, wxDefaultPosition, wxDefaultSize, 0);
    iceShardText = new wxTextCtrl(this, wxID_ANY, statIceShard, wxDefaultPosition, wxDefaultSize, 0);
    poisonShardText = new wxTextCtrl(this, wxID_ANY, statPoisonShard, wxDefaultPosition, wxDefaultSize, 0);
    shadowShardText = new wxTextCtrl(this, wxID_ANY, statShadowShard, wxDefaultPosition, wxDefaultSize, 0);
    lavaShardText = new wxTextCtrl(this, wxID_ANY, statLavaShard, wxDefaultPosition, wxDefaultSize, 0);
    demonKeysText = new wxTextCtrl(this, wxID_ANY, statDemonKeys, wxDefaultPosition, wxDefaultSize, 0);
    totalOrbsText = new wxTextCtrl(this, wxID_ANY, statTotalOrbs, wxDefaultPosition, wxDefaultSize, 0);

    jumpLabel = new wxStaticText(this, wxID_ANY, "Jumps:");
    attemptLabel = new wxStaticText(this, wxID_ANY, "Attempts:");
    officialLevelLabel = new wxStaticText(this, wxID_ANY, "Completed Official Levels:");
    onlineLevelLabel = new wxStaticText(this, wxID_ANY, "Completed Online Levels:");
    demonsLabel = new wxStaticText(this, wxID_ANY, "Demons:");
    starsLabel = new wxStaticText(this, wxID_ANY, "Stars:");
    mapPacksLabel = new wxStaticText(this, wxID_ANY, "Completed Map Packs:");
    secretCoinsLabel = new wxStaticText(this, wxID_ANY, "Secret Coins:");
    destroyedLabel = new wxStaticText(this, wxID_ANY, "Destroyed Players:");
    likedLabel = new wxStaticText(this, wxID_ANY, "Liked Levels:");
    ratedLabel = new wxStaticText(this, wxID_ANY, "Rated Levels:");
    userCoinsLabel = new wxStaticText(this, wxID_ANY, "User Coins:");
    diamondsLabel = new wxStaticText(this, wxID_ANY, "Diamonds:");
    orbsLabel = new wxStaticText(this, wxID_ANY, "Orbs:");
    dailyLabel = new wxStaticText(this, wxID_ANY, "Daily Levels:");
    fireShardLabel = new wxStaticText(this, wxID_ANY, "Fire Shards:");
    iceShardLabel = new wxStaticText(this, wxID_ANY, "Ice Shards:");
    poisonShardLabel = new wxStaticText(this, wxID_ANY, "Poison Shards:");
    shadowShardLabel = new wxStaticText(this, wxID_ANY, "Shadow Shards:");
    lavaShardLabel = new wxStaticText(this, wxID_ANY, "Lava Shards:");
    demonKeysLabel = new wxStaticText(this, wxID_ANY, "Demon Keys:");
    totalOrbsLabel = new wxStaticText(this, wxID_ANY, "Total Orbs:");

    wxButton* changeButton = new wxButton(this, ID_Change, "Change", wxDefaultPosition, wxDefaultSize, 0);

    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* hbox2 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* hbox3 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* hbox4 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* hbox5 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* hbox6 = new wxBoxSizer(wxHORIZONTAL);

    hbox1->Add(jumpLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox1->Add(jumpText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox1->Add(attemptLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox1->Add(attemptText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox1->Add(officialLevelLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox1->Add(officialLevelText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox1->Add(onlineLevelLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox1->Add(onlineLevelText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox2->Add(demonsLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox2->Add(demonsText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox2->Add(starsLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox2->Add(starsText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox2->Add(mapPacksLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox2->Add(mapPacksText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox2->Add(secretCoinsLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox2->Add(secretCoinsText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox3->Add(destroyedLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox3->Add(destroyedText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox3->Add(likedLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox3->Add(likedText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox3->Add(ratedLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox3->Add(ratedText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox3->Add(userCoinsLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox3->Add(userCoinsText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox4->Add(diamondsLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox4->Add(diamondsText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox4->Add(orbsLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox4->Add(orbsText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox4->Add(dailyLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox4->Add(dailyText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox4->Add(fireShardLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox4->Add(fireShardText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox5->Add(iceShardLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox5->Add(iceShardText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox5->Add(poisonShardLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox5->Add(poisonShardText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox5->Add(shadowShardLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox5->Add(shadowShardText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox5->Add(lavaShardLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox5->Add(lavaShardText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox6->Add(demonKeysLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox6->Add(demonKeysText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox6->Add(totalOrbsLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    hbox6->Add(totalOrbsText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    vbox->Add(hbox1, 0, wxEXPAND);
    vbox->Add(hbox2, 0, wxEXPAND);
    vbox->Add(hbox3, 0, wxEXPAND);
    vbox->Add(hbox4, 0, wxEXPAND);
    vbox->Add(hbox5, 0, wxEXPAND);
    vbox->Add(hbox6, 0, wxEXPAND);
    vbox->Add(changeButton, 0, wxALIGN_CENTER | wxALL, 5);

    SetSizerAndFit(vbox);
}

void Window::onChange(wxCommandEvent& event) {
    std::string file(fileContents.begin(), fileContents.end());

    file = boost::regex_replace(file, boost::regex("<k>1<\\/k><s>" + statJumps + "<\\/s>"), "<k>1</k><s>" + jumpText->GetValue().ToStdString() + "</s>");
    file = boost::regex_replace(file, boost::regex("<k>2<\\/k><s>" + statAttempts + "<\\/s>"), "<k>2</k><s>" + attemptText->GetValue().ToStdString() + "</s>");
    file = boost::regex_replace(file, boost::regex("<k>3<\\/k><s>" + statOfficialLevels + "<\\/s>"), "<k>3</k><s>" + officialLevelText->GetValue().ToStdString() + "</s>");
    file = boost::regex_replace(file, boost::regex("<k>4<\\/k><s>" + statOnlineLevels + "<\\/s>"), "<k>4</k><s>" + officialLevelText->GetValue().ToStdString() + "</s>");
    file = boost::regex_replace(file, boost::regex("<k>5<\\/k><s>" + statDemons + "<\\/s>"), "<k>5</k><s>" + demonsText->GetValue().ToStdString() + "</s>");
    file = boost::regex_replace(file, boost::regex("<k>6<\\/k><s>" + statStars + "<\\/s>"), "<k>6</k><s>" + starsText->GetValue().ToStdString() + "</s>");
    file = boost::regex_replace(file, boost::regex("<k>7<\\/k><s>" + statMapPacks + "<\\/s>"), "<k>7</k><s>" + mapPacksText->GetValue().ToStdString() + "</s>");
    file = boost::regex_replace(file, boost::regex("<k>8<\\/k><s>" + statSecretCoins + "<\\/s>"), "<k>8</k><s>" + secretCoinsText->GetValue().ToStdString() + "</s>");
    file = boost::regex_replace(file, boost::regex("<k>9<\\/k><s>" + statDestroyed + "<\\/s>"), "<k>9</k><s>" + destroyedText->GetValue().ToStdString() + "</s>");
    file = boost::regex_replace(file, boost::regex("<k>10<\\/k><s>" + statLiked + "<\\/s>"), "<k>10</k><s>" + likedText->GetValue().ToStdString() + "</s>");
    file = boost::regex_replace(file, boost::regex("<k>11<\\/k><s>" + statRated + "<\\/s>"), "<k>11</k><s>" + ratedText->GetValue().ToStdString() + "</s>");
    file = boost::regex_replace(file, boost::regex("<k>12<\\/k><s>" + statUserCoins + "<\\/s>"), "<k>12</k><s>" + secretCoinsText->GetValue().ToStdString() + "</s>");
    file = boost::regex_replace(file, boost::regex("<k>13<\\/k><s>" + statDiamonds + "<\\/s>"), "<k>13</k><s>" + diamondsText->GetValue().ToStdString() + "</s>");
    file = boost::regex_replace(file, boost::regex("<k>14<\\/k><s>" + statOrbs + "<\\/s>"), "<k>14</k><s>" + orbsText->GetValue().ToStdString() + "</s>");
    file = boost::regex_replace(file, boost::regex("<k>15<\\/k><s>" + statDaily + "<\\/s>"), "<k>15</k><s>" + dailyText->GetValue().ToStdString() + "</s>");
    file = boost::regex_replace(file, boost::regex("<k>16<\\/k><s>" + statFireShard + "<\\/s>"), "<k>16</k><s>" + fireShardText->GetValue().ToStdString() + "</s>");
    file = boost::regex_replace(file, boost::regex("<k>17<\\/k><s>" + statIceShard + "<\\/s>"), "<k>17</k><s>" + iceShardText->GetValue().ToStdString() + "</s>");
    file = boost::regex_replace(file, boost::regex("<k>18<\\/k><s>" + statPoisonShard + "<\\/s>"), "<k>18</k><s>" + poisonShardText->GetValue().ToStdString() + "</s>");
    file = boost::regex_replace(file, boost::regex("<k>19<\\/k><s>" + statShadowShard + "<\\/s>"), "<k>19</k><s>" + shadowShardText->GetValue().ToStdString() + "</s>");
    file = boost::regex_replace(file, boost::regex("<k>20<\\/k><s>" + statLavaShard + "<\\/s>"), "<k>20</k><s>" + lavaShardText->GetValue().ToStdString() + "</s>");
    file = boost::regex_replace(file, boost::regex("<k>21<\\/k><s>" + statDemonKeys + "<\\/s>"), "<k>21</k><s>" + demonKeysText->GetValue().ToStdString() + "</s>");
    file = boost::regex_replace(file, boost::regex("<k>22<\\/k><s>" + statTotalOrbs + "<\\/s>"), "<k>22</k><s>" + totalOrbsText->GetValue().ToStdString() + "</s>");

    // we may need to get values again idk

    fileContents = std::vector<uint8_t>(file.begin(), file.end());

    wxMessageBox("Changes have been made.", "Changed", wxOK, this);
}

void Window::onSave(wxCommandEvent& event) {
    if (!currentFile.compare("")) {
        wxMessageBox("Failed to save!", "Error", wxICON_ERROR | wxOK, this);
    }

    try {
        CCData::CCData data;
        std::vector<uint8_t> Data = data.Encrypt(fileContents);

        std::ofstream output;

        output.open(currentFile);
        output.write(reinterpret_cast<const char*>(Data.data()), Data.size());
        output.close();
    } catch (const std::exception& e) {
        wxMessageBox("Failed to encode the file!", "Error", wxICON_ERROR | wxOK, this);
    }

    wxMessageBox("Changes have been saved to the file.", "Saved", wxOK, this);
}

void Window::onSaveAs(wxCommandEvent& event) {
    wxFileDialog newFileDialog(this, "Create a new CCGameManager.dat file", "", "", "CCGameManager.dat (*.dat)|*.dat", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (newFileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }

    currentFile = newFileDialog.GetPath().ToStdString();

    try {
        CCData::CCData data;
        std::vector<uint8_t> Data = data.Encrypt(fileContents);

        std::ofstream output;

        output.open(currentFile);
        output.write(reinterpret_cast<const char*>(Data.data()), Data.size());
        output.close();
    } catch (const std::exception& e) {
        wxMessageBox("Failed to encode the file!", "Error", wxICON_ERROR | wxOK, this);
    }
}

void Window::onEncode(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, "Open CCGameManager.xml", "", "", "CCGameManager.xml (*.xml)|*.xml|All files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }

    wxFileDialog newFileDialog(this, "Create a new CCGameManager.dat file", "", "", "CCGameManager.dat (*.dat)|*.dat", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (newFileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }

    std::string inputFile = openFileDialog.GetPath().ToStdString();
    std::string outputFile = newFileDialog.GetPath().ToStdString();

    try {
        CCData::CCData data;
        std::ifstream file(inputFile, std::ios::binary);

        std::vector<uint8_t> fileContents(std::istreambuf_iterator<char>(file), {});
        std::vector<uint8_t> Data{};

        Data = data.Encrypt(fileContents);

        std::ofstream output;

        output.open(outputFile);
        output.write(reinterpret_cast<const char*>(Data.data()), Data.size());
        output.close();
    } catch (const std::exception& e) {
        wxMessageBox("Failed to encode the file!", "Error", wxICON_ERROR | wxOK, this);
    }
}
void Window::onDecode(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, _("Open CCGameManager.dat"), "", "", "CCGameManager.dat (*.dat)|*.dat|All files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }

    wxFileDialog newFileDialog(this, "Create a new CCGameManager.xml file", "", "", "CCGameManager.xml (*.xml)|*.xml", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (newFileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }

    std::string inputFile = openFileDialog.GetPath().ToStdString();
    std::string outputFile = newFileDialog.GetPath().ToStdString();

    try {
        CCData::CCData data;
        std::ifstream file(inputFile, std::ios::binary);

        std::vector<uint8_t> fileContents(std::istreambuf_iterator<char>(file), {});
        std::vector<uint8_t> Data{};

        Data = data.Decrypt(fileContents);

        std::ofstream output;

        output.open(outputFile);
        output.write(reinterpret_cast<const char*>(Data.data()), Data.size());
        output.close();
    } catch (const std::exception& e) {
        wxMessageBox("Failed to decode the file!", "Error", wxICON_ERROR | wxOK, this);
    }
}

wxBEGIN_EVENT_TABLE(Window, wxFrame)
    EVT_BUTTON(ID_Change, Window::onChange)
wxEND_EVENT_TABLE()
