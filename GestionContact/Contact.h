#ifndef CONTACT_H
#define CONTACT_H

#include <string>

class Contact {
public:
    Contact(const std::string& nom, const std::string& telephone, const std::string& email, const std::string& adresse);

    std::string GetNom() const;
    std::string GetTelephone() const;
    std::string GetEmail() const;
    std::string GetAdresse() const;

    std::string ToString() const;

private:
    std::string nom_;
    std::string telephone_;
    std::string email_;
    std::string adresse_;
};

#endif
