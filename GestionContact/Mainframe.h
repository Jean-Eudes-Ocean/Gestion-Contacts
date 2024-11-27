#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wx.h>
#include <vector>
#include "Contact.h"

class Mainframe : public wxFrame {
public:
    Mainframe(const wxString& title);

private:
    void AjoutContact(wxCommandEvent& event);
    void SauvegarderContacts(wxCommandEvent& event);
    void ChargerContacts(wxCommandEvent& event);
    void SupprimerContact(wxCommandEvent& event); 
    void ModifierContact(wxCommandEvent& event);
    void RechercherContact(wxCommandEvent& event);
    void TrierContacts(wxCommandEvent& event);

    wxTextCtrl* searchBox_;
    wxListBox* contactList_;
    std::vector<Contact> contacts_;

    wxDECLARE_EVENT_TABLE();
};

enum {
    ID_AJOUT_CONTACT = 1,
    ID_SAUVEGARDER_CONTACTS,
    ID_CHARGER_CONTACTS,
    ID_SUPPRIMER_CONTACT,
    ID_MODIFIER_CONTACT,
    ID_RECHERCHER_CONTACT,  
    ID_TRIER_CONTACTS
};

#endif
