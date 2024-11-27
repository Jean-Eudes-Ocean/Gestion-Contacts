#include "Contact.h"

Contact::Contact(const std::string& nom, const std::string& telephone, const std::string& email, const std::string& adresse)
    : nom_(nom), telephone_(telephone), email_(email), adresse_(adresse) {
}

std::string Contact::GetNom() const { return nom_; }
std::string Contact::GetTelephone() const { return telephone_; }
std::string Contact::GetEmail() const { return email_; }
std::string Contact::GetAdresse() const { return adresse_; }

std::string Contact::ToString() const {
    return nom_ + " | " + telephone_ + " | " + email_ + " | " + adresse_;
}
