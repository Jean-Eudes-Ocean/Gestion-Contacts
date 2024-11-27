#include "Mainframe.h"
#include <fstream>
#include <sstream>
#include <algorithm>

// Table des événements
wxBEGIN_EVENT_TABLE(Mainframe, wxFrame)
EVT_BUTTON(ID_AJOUT_CONTACT, Mainframe::AjoutContact)
EVT_BUTTON(ID_SAUVEGARDER_CONTACTS, Mainframe::SauvegarderContacts)
EVT_BUTTON(ID_CHARGER_CONTACTS, Mainframe::ChargerContacts)
EVT_BUTTON(ID_SUPPRIMER_CONTACT, Mainframe::SupprimerContact)
EVT_BUTTON(ID_MODIFIER_CONTACT, Mainframe::ModifierContact)
EVT_BUTTON(ID_TRIER_CONTACTS, Mainframe::TrierContacts)
wxEND_EVENT_TABLE()

Mainframe::Mainframe(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(700, 500)) {

    // Créer le panneau principal
    wxPanel* panel = new wxPanel(this);

    // Créer le sizer principal vertical
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Ajouter la liste des contacts
    contactList_ = new wxListBox(panel, wxID_ANY, wxDefaultPosition, wxSize(600, 250));
    mainSizer->Add(contactList_, 1, wxEXPAND | wxALL, 10);

    // Créer un sizer horizontal pour la recherche
    wxBoxSizer* searchSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* searchLabel = new wxStaticText(panel, wxID_ANY, "Rechercher :");
    searchBox_ = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(300, -1));
    searchBox_->Bind(wxEVT_TEXT, &Mainframe::RechercherContact, this);

    // Ajouter les éléments de recherche au sizer
    searchSizer->Add(searchLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
    searchSizer->Add(searchBox_, 1, wxEXPAND);

    mainSizer->Add(searchSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

    // Créer un sizer pour les boutons
    wxGridSizer* buttonSizer = new wxGridSizer(2, 3, 10, 10);

    wxButton* addButton = new wxButton(panel, ID_AJOUT_CONTACT, "Ajouter Contact");
    wxButton* saveButton = new wxButton(panel, ID_SAUVEGARDER_CONTACTS, "Sauvegarder Contacts");
    wxButton* loadButton = new wxButton(panel, ID_CHARGER_CONTACTS, "Charger Contacts");
    wxButton* deleteButton = new wxButton(panel, ID_SUPPRIMER_CONTACT, "Supprimer Contact");
    wxButton* editButton = new wxButton(panel, ID_MODIFIER_CONTACT, "Modifier Contact");
    wxButton* sortButton = new wxButton(panel, ID_TRIER_CONTACTS, "Trier Contacts");

    // Ajouter les boutons au sizer des boutons
    buttonSizer->Add(addButton, 1, wxEXPAND);
    buttonSizer->Add(saveButton, 1, wxEXPAND);
    buttonSizer->Add(loadButton, 1, wxEXPAND);
    buttonSizer->Add(deleteButton, 1, wxEXPAND);
    buttonSizer->Add(editButton, 1, wxEXPAND);
    buttonSizer->Add(sortButton, 1, wxEXPAND);

    mainSizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 10);

    // Appliquer le sizer principal au panneau
    panel->SetSizer(mainSizer);

    // Ajuster la taille de la fenêtre pour contenir tout
    mainSizer->SetSizeHints(this);
}


// Methode pour l'ajout d'un contact
void Mainframe::AjoutContact(wxCommandEvent& event) {
    wxString nom = wxGetTextFromUser("Entrez le nom :", "Ajouter un contact");
    wxString telephone = wxGetTextFromUser("Entrez le numéro de téléphone :", "Ajouter un contact");
    wxString email = wxGetTextFromUser("Entrez l'email :", "Ajouter un contact");
    wxString adresse = wxGetTextFromUser("Entrez l'adresse :", "Ajouter un contact");

    if (!nom.IsEmpty() && !telephone.IsEmpty()) {
        Contact contact(std::string(nom.mb_str()), std::string(telephone.mb_str()),
            std::string(email.mb_str()), std::string(adresse.mb_str()));
        contacts_.push_back(contact);
        contactList_->Append(wxString::FromUTF8(contact.ToString()));

        
        wxMessageBox("Le contact a été ajouté avec succès !", "Succès", wxOK | wxICON_INFORMATION);
    }
    else {
        wxMessageBox("Le nom et le numéro de téléphone sont obligatoires.", "Erreur", wxOK | wxICON_ERROR);
    }
}

// Methode pour la sauvegarde des contacts
void Mainframe::SauvegarderContacts(wxCommandEvent& event) {
    wxFileDialog saveFileDialog(this, "Sauvegarder les contacts", "", "",
        "Fichiers texte (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;

    std::ofstream file(std::string(saveFileDialog.GetPath().mb_str()));
    if (file.is_open()) {
        for (const auto& contact : contacts_) {
            file << contact.ToString() << std::endl;
        }
        file.close();
        wxMessageBox("Contacts sauvegardés avec succès !", "Succès", wxOK | wxICON_INFORMATION);
    }
    else {
        wxMessageBox("Erreur lors de la sauvegarde.", "Erreur", wxOK | wxICON_ERROR);
    }
}

// Methode pour le chargement des contacts
void Mainframe::ChargerContacts(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, "Charger les contacts", "", "",
        "Fichiers texte (*.txt)|*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    std::ifstream file(std::string(openFileDialog.GetPath().mb_str()));
    if (file.is_open()) {
        contacts_.clear();
        contactList_->Clear();

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream stream(line);
            std::string nom, telephone, email, adresse;

            std::getline(stream, nom, '|');
            std::getline(stream, telephone, '|');
            std::getline(stream, email, '|');
            std::getline(stream, adresse, '|');

            
            auto trim = [](std::string& str) {
                str.erase(0, str.find_first_not_of(" \t"));
                str.erase(str.find_last_not_of(" \t") + 1);
                };
            trim(nom);
            trim(telephone);
            trim(email);
            trim(adresse);

            Contact contact(nom, telephone, email, adresse);
            contacts_.push_back(contact);
            contactList_->Append(wxString::FromUTF8(contact.ToString()));
        }

        file.close();
        wxMessageBox("Contacts chargés avec succès !", "Succès", wxOK | wxICON_INFORMATION);
    }
    else {
        wxMessageBox("Erreur lors du chargement des contacts.", "Erreur", wxOK | wxICON_ERROR);
    }
}

// Methode pour la suppression d'un contact
void Mainframe::SupprimerContact(wxCommandEvent& event) {
    int selection = contactList_->GetSelection();
    if (selection == wxNOT_FOUND) {
        wxMessageBox("Veuillez sélectionner un contact à supprimer.", "Erreur", wxOK | wxICON_ERROR);
        return;
    }

    int response = wxMessageBox(
        "Êtes-vous sûr de vouloir supprimer ce contact ?",
        "Confirmation",
        wxYES_NO | wxICON_QUESTION
    );

    if (response == wxYES) {
        contacts_.erase(contacts_.begin() + selection); 
        contactList_->Delete(selection); 
        wxMessageBox("Contact supprimé avec succès !", "Succès", wxOK | wxICON_INFORMATION);
    }
    else {
        wxMessageBox("Suppression annulée.", "Information", wxOK | wxICON_INFORMATION);
    }
}



// Methode pour la modification d'un contact
void Mainframe::ModifierContact(wxCommandEvent& event) {
    int selection = contactList_->GetSelection();
    if (selection == wxNOT_FOUND) {
        wxMessageBox("Veuillez sélectionner un contact à modifier.", "Erreur", wxOK | wxICON_ERROR);
        return;
    }

    Contact& contact = contacts_[selection];

    wxString nom = wxGetTextFromUser("Modifier le nom :", "Modifier un contact", wxString::FromUTF8(contact.GetNom()));
    wxString telephone = wxGetTextFromUser("Modifier le numéro de téléphone :", "Modifier un contact", wxString::FromUTF8(contact.GetTelephone()));
    wxString email = wxGetTextFromUser("Modifier l'email :", "Modifier un contact", wxString::FromUTF8(contact.GetEmail()));
    wxString adresse = wxGetTextFromUser("Modifier l'adresse :", "Modifier un contact", wxString::FromUTF8(contact.GetAdresse()));

    if (!nom.IsEmpty() && !telephone.IsEmpty()) {
        contact = Contact(std::string(nom.mb_str()), std::string(telephone.mb_str()),
            std::string(email.mb_str()), std::string(adresse.mb_str()));

        
        contactList_->SetString(selection, wxString::FromUTF8(contact.ToString()));
        wxMessageBox("Contact modifié avec succès !", "Succès", wxOK | wxICON_INFORMATION);
    }
    else {
        wxMessageBox("Le nom et le numéro de téléphone sont obligatoires.", "Erreur", wxOK | wxICON_ERROR);
    }
}


// Methode pour la recherche d'un contact
void Mainframe::RechercherContact(wxCommandEvent& event) {
    wxString searchQuery = searchBox_->GetValue().Lower();  
    contactList_->Clear(); 

    for (const auto& contact : contacts_) {
        wxString contactName = wxString::FromUTF8(contact.GetNom()).Lower();  
        if (contactName.Contains(searchQuery)) {
            
            contactList_->Append(wxString::FromUTF8(contact.ToString()));
        }
    }

    if (contactList_->GetCount() == 0) {
        wxMessageBox("Aucun contact correspondant trouvé.", "Résultat", wxOK | wxICON_INFORMATION);
    }
}



// Methode pour le trie des contacts par ordre alphabetique
void Mainframe::TrierContacts(wxCommandEvent& event) {
    std::sort(contacts_.begin(), contacts_.end(), [](const Contact& a, const Contact& b) {
        // Convertir les noms en minuscules pour un tri insensible à la casse
        std::string lowerA = wxString::FromUTF8(a.GetNom()).Lower().ToStdString();
        std::string lowerB = wxString::FromUTF8(b.GetNom()).Lower().ToStdString();
        return lowerA < lowerB;
        });

    contactList_->Clear();
    for (const auto& contact : contacts_) {
        contactList_->Append(wxString::FromUTF8(contact.ToString()));
    }

    wxMessageBox("Contacts triés par ordre alphabétique avec succès !", "Succès", wxOK | wxICON_INFORMATION);
}

