#include <wx/wx.h>
#include "Mainframe.h"

class MyApp : public wxApp {
public:
    virtual bool OnInit() {
        Mainframe* frame = new Mainframe("Gestion de Contacts");
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
