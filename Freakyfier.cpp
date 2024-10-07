#include <wx/wx.h>
#include <wx/textctrl.h>
#include <unordered_map>

// Lookup table for math bold characters
std::unordered_map<wchar_t, wchar_t> mathBoldMap = {
    {L'A', 0x1D400}, {L'B', 0x1D401}, {L'C', 0x1D402}, {L'D', 0x1D403}, {L'E', 0x1D404},
    {L'F', 0x1D405}, {L'G', 0x1D406}, {L'H', 0x1D407}, {L'I', 0x1D408}, {L'J', 0x1D409},
    {L'K', 0x1D40A}, {L'L', 0x1D40B}, {L'M', 0x1D40C}, {L'N', 0x1D40D}, {L'O', 0x1D40E},
    {L'P', 0x1D40F}, {L'Q', 0x1D410}, {L'R', 0x1D411}, {L'S', 0x1D412}, {L'T', 0x1D413},
    {L'U', 0x1D414}, {L'V', 0x1D415}, {L'W', 0x1D416}, {L'X', 0x1D417}, {L'Y', 0x1D418},
    {L'Z', 0x1D419},
    {L'a', 0x1D41A}, {L'b', 0x1D41B}, {L'c', 0x1D41C}, {L'd', 0x1D41D}, {L'e', 0x1D41E},
    {L'f', 0x1D41F}, {L'g', 0x1D420}, {L'h', 0x1D421}, {L'i', 0x1D422}, {L'j', 0x1D423},
    {L'k', 0x1D424}, {L'l', 0x1D425}, {L'm', 0x1D426}, {L'n', 0x1D427}, {L'o', 0x1D428},
    {L'p', 0x1D429}, {L'q', 0x1D42A}, {L'r', 0x1D42B}, {L's', 0x1D42C}, {L't', 0x1D42D},
    {L'u', 0x1D42E}, {L'v', 0x1D42F}, {L'w', 0x1D430}, {L'x', 0x1D431}, {L'y', 0x1D432},
    {L'z', 0x1D433},
    {L'0', 0x1D7E0}, {L'1', 0x1D7E1}, {L'2', 0x1D7E2}, {L'3', 0x1D7E3}, {L'4', 0x1D7E4},
    {L'5', 0x1D7E5}, {L'6', 0x1D7E6}, {L'7', 0x1D7E7}, {L'8', 0x1D7E8}, {L'9', 0x1D7E9}
};

class MyFrame : public wxFrame
{
public:
    MyFrame() : wxFrame(nullptr, wxID_ANY, "Math Bold Converter", wxDefaultPosition, wxSize(400, 300))
    {
        inputText = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
        outputText = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);

        // Bind the text input event
        inputText->Bind(wxEVT_TEXT, &MyFrame::OnTextInput, this);

        // Layout
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(inputText, 0, wxEXPAND | wxALL, 10);
        sizer->Add(outputText, 1, wxEXPAND | wxALL, 10);
        SetSizer(sizer);
    }

private:
    wxTextCtrl* inputText;
    wxTextCtrl* outputText;

    void OnTextInput(wxCommandEvent& event)
    {
        wxString input = inputText->GetValue();
        wxString output;

        // Convert each character
        for (size_t i = 0; i < input.Length(); ++i)
        {
            wchar_t c = input[i];
            auto it = mathBoldMap.find(c);
            if (it != mathBoldMap.end())
            {
                output += it->second; // Append the math bold equivalent
            }
            else
            {
                output += c; // Append unchanged if not found
            }
        }

        outputText->SetValue(output); // Update output text control
    }
};

class MyApp : public wxApp
{
public:
    virtual bool OnInit()
    {
        MyFrame* frame = new MyFrame();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
