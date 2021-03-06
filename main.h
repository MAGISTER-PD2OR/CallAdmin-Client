#ifndef MAIN_H
#define MAIN_H

/**
 * -----------------------------------------------------
 * File        main.h
 * Authors     David Ordnung, Impact
 * License     GPLv3
 * Web         http://dordnung.de, http://gugyclan.eu
 * -----------------------------------------------------
 *
 * Copyright (C) 2013-2017 David Ordnung, Impact
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

#pragma once


// Precomp Header
#include <wx/wxprec.h>

// We need WX
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/listbox.h>
#include <wx/notebook.h>
#include "call.h"

// Main Notebook
extern wxNotebook* notebook;




// Event ID's for Main Dialog
enum
{
	wxID_Hide = wxID_HIGHEST+100,
	wxID_Reconnect,
	wxID_BoxClick,
	wxID_CheckBox,
	wxID_SteamChanged,
	wxID_ThreadHandled,
};




// Main Dialog Class
class MainDialog: public wxDialog
{
private:
	wxCheckBox* store;
	wxCheckBox* available;
	wxCheckBox* sound;

	wxButton* reconnectButton;

	wxPanel *panel;

	wxListBox* callBox;
	wxSizer* sizerBody;

	wxStaticText* eventText;
	wxStaticText* steamText;

public:
	MainDialog(const wxString& title) : wxDialog(NULL, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxMINIMIZE_BOX) 
	{
		// Initialize vars
		store = NULL;
		available = NULL;
		sound = NULL;
		reconnectButton = NULL;
		panel = NULL;
		callBox = NULL;
		sizerBody = NULL;
		eventText = NULL;
		steamText = NULL;
	}

	// Create the window
	void createWindow(bool taskbar = false);

	// Check checkboxes
	bool isAvailable() {return (available != NULL && available->IsChecked());}
	bool wantStore() {return (store != NULL && !store->IsChecked());}
	bool wantSound() {return (sound != NULL && sound->IsChecked());}

	// Update Window
	void setEventText(wxString text) {eventText->SetLabelText(text); sizerBody->Layout(); eventText->Refresh(); panel->SetSizerAndFit(sizerBody, false); notebook->Fit(); Fit();}
	void setReconnectButton(bool enable=false) {reconnectButton->Enable(enable);}

	void setSteamStatus(wxString text, wxColor color) {steamText->SetLabelText(text); steamText->SetForegroundColour(color); sizerBody->Layout(); panel->SetSizerAndFit(sizerBody, false); notebook->Fit(); Fit();}

	// Update Call list
	void updateCall();
	void resetCalls() {callBox->Clear();}
	void setHandled(int item)
	{
		callBox->SetString(item, "F - " + wxString::FromUTF8(call_dialogs[item]->getBoxText()));
		call_dialogs[item]->setFinish();
		call_dialogs[item]->takeover->Enable(false);
	}


protected:
	// Button Events
	void OnHide(wxCommandEvent& event);
	void OnReconnect(wxCommandEvent& event);

	void OnCloseWindow(wxCloseEvent& event);
	void OnMinimizeWindow(wxIconizeEvent& event);
	void OnBoxClick(wxCommandEvent& event);

	void OnCheckBox(wxCommandEvent& event);
	void OnSteamChange(wxCommandEvent& event);

	// Thread Event
	void OnThread(wxCommandEvent& event);

	DECLARE_EVENT_TABLE()
};


// Main Dialog
extern MainDialog *main_dialog;

#endif
