/**
 * -----------------------------------------------------
 * File        trackers_panel.cpp
 * Authors     David O., Impact
 * License     GPLv3
 * Web         http://popoklopsi.de, http://gugyclan.eu
 * -----------------------------------------------------
 *
 * Copyright (C) 2013-2016 David O., Impact
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>
 */

// Include Project
#include "calladmin-client.h"
#include "tinyxml2/tinyxml2.h"
#include "curl_util.h"


// Update Button Id
enum {
	wxID_UpdateTrackers = wxID_HIGHEST + 10,
};


// Button Events for Log Panel
BEGIN_EVENT_TABLE(TrackerPanel, wxPanel)
EVT_BUTTON(wxID_UpdateTrackers, TrackerPanel::OnUpdate)
END_EVENT_TABLE()


// Create Tracker Panel
TrackerPanel::TrackerPanel() : wxPanel(caGetNotebook(), wxID_ANY) {
	// Border and Center
	wxSizerFlags flags;

	// Border and Centre
	flags.Border(wxALL, 10);
	flags.Centre();

	// Create Box
	wxSizer* const sizerTop = new wxBoxSizer(wxVERTICAL);

	this->trackerBox = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_HSCROLL | wxLB_SINGLE);
	this->trackerBox->SetFont(wxFont(14, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	// Add Log Box
	sizerTop->Add(this->trackerBox, 1, wxEXPAND);

	wxSizer* const sizerBtns = new wxBoxSizer(wxHORIZONTAL);

	// Hide and Exit Button
	sizerBtns->Add(new wxButton(this, wxID_UpdateTrackers, "Update Trackers"), flags.Border(wxALL, 5));

	// Add Buttons to Box
	sizerTop->Add(sizerBtns, flags.Align(wxALIGN_CENTER_HORIZONTAL));

	// Auto Size
	SetSizerAndFit(sizerTop, true);
}


void TrackerPanel::AddTracker(wxString text) {
	this->trackerBox->Append(wxString::FromUTF8(text));
}


void TrackerPanel::DeleteTrackers() {
	this->trackerBox->Clear();
}


// Button Event -> Update List
void TrackerPanel::OnUpdate(wxCommandEvent& WXUNUSED(event)) {
	// Get the Trackers Page
	CurlThread::GetPage(TrackerPanel::RefreshTrackers, caGetConfig()->GetPage() + "/trackers.php?from=20&from_type=interval&key=" + caGetConfig()->GetKey());
}


// Refresh Trackers
void TrackerPanel::RefreshTrackers(char* errors, wxString result, int WXUNUSED(extra)) {
	// Log Action
	caLogAction("Got Trackers");

	wxString error = "";

	// Delete old ones
	caGetTrackersPanel()->DeleteTrackers();

	// Not empty?
	if (result != "") {
		// Everything good :)
		if (strcmp(errors, "") == 0) {
			// Proceed XML result!
			tinyxml2::XMLDocument doc;
			tinyxml2::XMLNode *node;
			tinyxml2::XMLError parseError;

			// Parse the xml data
			parseError = doc.Parse(result);

			// Parsing good :)?
			if (parseError == tinyxml2::XML_SUCCESS) {
				// Goto xml child
				node = doc.FirstChild();

				// Goto CallAdmin_Trackers
				if (node != NULL) {
					node = node->NextSibling();
				}

				// Found Trackers?
				if (node != NULL) {
					// found someone?
					bool found = false;

					// Tracker Loop
					for (tinyxml2::XMLNode *node2 = node->FirstChild(); node2; node2 = node2->NextSibling()) {
						// API Error?
						if ((wxString)node2->Value() == "error") {
							error = node2->FirstChild()->Value();

							break;
						}


						// Search admin steamids
						for (tinyxml2::XMLNode *node3 = node2->FirstChild(); node3; node3 = node3->NextSibling()) {
							// Found steamid
							if ((wxString)node3->Value() == "trackerID") {
								wxString steamidString = node3->FirstChild()->Value();

								// Build csteamid
								CSteamID steamidTracker = CallDialog::SteamIDtoCSteamID((char*)steamidString.mb_str().data());

								// Valid Tracker ID?
								if (steamidTracker.IsValid()) {
									// Create Name Timer
									// TODO Kills itself?
									new NameTimer(steamidTracker);

									found = true;
								}
							}
						}
					}

					// Have we found something?
					if (found) {
						// We are finished :)
						return;
					}
				}
			} else {
				// XML ERROR
				error = "XML ERROR: Couldn't parse the trackers API!";

				// Log Action
				caLogAction("XML Error in trackers API");
			}
		} else {
			// Curl error
			error = errors;

			// Log Action
			caLogAction("CURL Error " + error);
		}
	} else {
		// Curl error
		error = "Couldn't init. CURL connection";
	}


	// Seems we found no one
	if (error == "") {
		error = "Found no available trackers!";
	}

	caGetTaskBarIcon()->ShowMessage("Coulnd't retrieve trackers!", error, NULL);
}
