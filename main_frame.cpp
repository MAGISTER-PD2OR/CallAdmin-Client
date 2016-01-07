/**
 * -----------------------------------------------------
 * File        main_frame.cpp
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
#include "main_frame.h"
#include "calladmin-client.h"
#include "curl_util.h"

#include <wx/xrc/xmlres.h>


// Events for Main Frame
BEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_CLOSE(MainFrame::OnCloseWindow)
EVT_ICONIZE(MainFrame::OnMinimizeWindow)
END_EVENT_TABLE()


// Init Main Frame (Window)
bool MainFrame::InitFrame(bool createInTaskbar) {
	if (!wxXmlResource::Get()->LoadFrame(this, NULL, "mainFrame")) {
		wxMessageBox("Error: Couldn't find XRCID mainFrame", "Error on creating CallAdmin", wxOK | wxCENTRE | wxICON_ERROR);

		return false;
	}

	// Load the notebook
	wxNotebook *notebook;
	FIND_OR_FAIL(notebook, XRCCTRL(*this, "notebook", wxNotebook), "notebook");

	this->notebook = new Notebook(notebook);
	if (!this->notebook->CreatePages()) {
		return false;
	}

	caLogAction("Created Notebook and loaded pages successfully!", LogLevel::LEVEL_DEBUG);

	// Start in taskbar?
	Show(!createInTaskbar || !caGetTaskBarIcon()->IsAvailable());

	// Fit Notebook
	this->notebook->GetWindow()->Fit();

	// Fit Main
	Fit();

	// Centre to Screen
	Centre();

	return true;
}


// Window Event -> Exit programm
void MainFrame::OnCloseWindow(wxCloseEvent &WXUNUSED(event)) {
	delete this->notebook;

	caGetApp().ExitProgramm();
}


// Window Event -> Hide Window
void MainFrame::OnMinimizeWindow(wxIconizeEvent &WXUNUSED(event)) {
	caLogAction("Hided main frame", LogLevel::LEVEL_DEBUG);

	if (caGetConfig()->GetHideOnMinimize()) {
		if (caGetTaskBarIcon()->IsAvailable()) {
			// Log Action
			caLogAction("Hided main frame to taskbar", LogLevel::LEVEL_DEBUG);

			Show(false);
		} else {
			caLogAction("Hided main frame (Taskbar not available)", LogLevel::LEVEL_DEBUG);

			Iconize(true);
		}
	}
}