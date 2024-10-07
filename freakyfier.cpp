#include <wx/wx.h>
#include <wx/tglbtn.h>
#include <cstdint>
#include <vector>

HHOOK hHook = NULL;

void sendSurrogatePair(char32_t target) {
    wchar_t high_surrogate = 0xD800 + ((target - 0x10000) >> 10);
    wchar_t low_surrogate = 0xDC00 + ((target - 0x10000) & 0x3FF);

    std::vector<INPUT> in(4);
    ZeroMemory(&in[0], in.size() * sizeof(INPUT));

    in[0].type = INPUT_KEYBOARD;
    in[0].ki.wVk = 0;
    in[0].ki.wScan = high_surrogate;
    in[0].ki.dwFlags = KEYEVENTF_UNICODE;

    in[1].type = INPUT_KEYBOARD;
    in[1].ki.wVk = 0;
    in[1].ki.wScan = low_surrogate;
    in[1].ki.dwFlags = KEYEVENTF_UNICODE;

    in[2] = in[0];
    in[2].ki.dwFlags |= KEYEVENTF_KEYUP;

    in[3] = in[1];
    in[3].ki.dwFlags |= KEYEVENTF_KEYUP;

    SendInput(in.size(), &in[0], sizeof(INPUT));
}

char32_t to_math_bold(wchar_t c) {
    if (c >= L'A' && c <= L'Z') {
        return c + 0x1D4D0 - L'A';
    }
    else if (c >= L'a' && c <= L'z') {
        return c + 0x1D4EA - L'a';
    }
    else if (c >= L'0' && c <= L'9') {
        return c + 0x1D7CE - L'0';
    }
    return c;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
        KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;
        DWORD vkCode = p->vkCode;

        WCHAR buffer[4] = { 0 };
        BYTE keyState[256];
        GetKeyboardState(keyState);

        if ((GetKeyState(VK_SHIFT) & 0x8000)) {
            keyState[VK_SHIFT] |= 0x80;
        }

        if ((GetKeyState(VK_CAPITAL) & 0x0001)) {
            keyState[VK_CAPITAL] |= 0x01;
        }

        int result = ToUnicode(vkCode, p->scanCode, keyState, buffer, sizeof(buffer) / sizeof(WCHAR), 0);

        if (result > 0) {
            wchar_t typedChar = buffer[0];
            char32_t boldChar = to_math_bold(typedChar);

            if (boldChar != typedChar) {
                if (boldChar >= 0x10000) {
                    sendSurrogatePair(boldChar);
                }
                else {
                    INPUT input;
                    input.type = INPUT_KEYBOARD;
                    input.ki.wVk = 0;
                    input.ki.wScan = (WORD)boldChar;
                    input.ki.dwFlags = KEYEVENTF_UNICODE;
                    input.ki.time = 0;
                    input.ki.dwExtraInfo = 0;

                    SendInput(1, &input, sizeof(INPUT));
                    input.ki.dwFlags |= KEYEVENTF_KEYUP;
                    SendInput(1, &input, sizeof(INPUT));
                }

                return 1;
            }
        }
    }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

// Toggle hook
void ToggleHook(bool enable) {
    if (enable && !hHook) {
        hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    }
    else if (!enable && hHook) {
        UnhookWindowsHookEx(hHook);
        hHook = NULL;
    }
}

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
    MyFrame();

private:
    void OnToggleHook(wxCommandEvent& event);
    wxToggleButton* m_toggleBtn;
    bool isHookEnabled;

    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_TOGGLEBUTTON(1001, MyFrame::OnToggleHook)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame* frame = new MyFrame();
    frame->Show(true);
    return true;
}

MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, "Freakyfier", wxDefaultPosition, wxSize(300, 200)),
    isHookEnabled(false)
{
    m_toggleBtn = new wxToggleButton(this, 1001, "Disabled", wxPoint(50, 50), wxSize(200, 50));
}

void MyFrame::OnToggleHook(wxCommandEvent& event)
{
    isHookEnabled = event.IsChecked();
    m_toggleBtn->SetLabel(isHookEnabled ? "Enabled" : "Disabled");
    ToggleHook(isHookEnabled);
}
