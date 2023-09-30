#include <date/date.h>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/datectrl.h>
#include <wx/dateevt.h>

class MainFrame final : public wxFrame
{
public:
    MainFrame()
        : wxFrame(nullptr, wxID_ANY, "wxDatesAndDateDotH")
        , pDateCtrl(nullptr)
        , pDateLabel(nullptr)
        , pDateDotHFloorLabel(nullptr)
        , pDateDotHCeilLabel(nullptr)
        , pDateCmpLabel(nullptr) {
        CreateControls();
        FillControls();

        Bind(
            wxEVT_DATE_CHANGED,
            &MainFrame::OnDateSelection,
            this,
            wxdIDC_DATESLCTR
        );
    }

    virtual ~MainFrame() = default;

    void OnDateSelection(wxDateEvent& event) {
        pDateLabel->SetLabel(wxString::Format("Date Selected:    %s", event.GetDate().FormatISOCombined()));

        auto eventDate = event.GetDate();
        auto dateUtc = eventDate.MakeFromTimezone(wxDateTime::UTC);
        auto dateUtcTicks = dateUtc.GetTicks();

        auto dateFloor = date::floor<date::days>(std::chrono::system_clock::from_time_t(dateUtcTicks));
        auto dateCeil = date::ceil<date::days>(std::chrono::system_clock::from_time_t(dateUtcTicks));

        pDateDotHFloorLabel->SetLabel(wxString::Format("date::floor =       %s", date::format("%F", dateFloor)));
        pDateDotHCeilLabel->SetLabel(wxString::Format("date::ceil =         %s", date::format("%F", dateCeil)));

        pDateCmpLabel->SetLabel(
            wxString::Format("Event Date: %s | UTC: %s",
                event.GetDate().Format(wxDefaultDateTimeFormat, wxDateTime::UTC),
                dateUtc.Format(wxDefaultDateTimeFormat, wxDateTime::UTC))
        );
    }

private:
    void CreateControls() {
        auto mainPanel = new wxPanel(this, wxID_ANY);

        auto mainSizer = new wxBoxSizer(wxVERTICAL);
        mainPanel->SetSizer(mainSizer);

        auto starterLabel = new wxStaticText(mainPanel, wxID_ANY, "wxDates");
        mainSizer->Add(starterLabel, wxSizerFlags().Center());

        auto dateSizer = new wxBoxSizer(wxHORIZONTAL);
        mainSizer->Add(dateSizer);

        auto dateLabel = new wxStaticText(mainPanel, wxID_ANY, "Date: ");
        dateSizer->Add(dateLabel, wxSizerFlags().Border(wxALL, 5).CenterVertical());

        auto currentDate = wxDateTime::Now();
        auto utcDate = currentDate.MakeUTC();
        pDateCtrl = new wxDatePickerCtrl(mainPanel, wxdIDC_DATESLCTR, utcDate);
        dateSizer->Add(pDateCtrl, wxSizerFlags().Border(wxALL, 5));

        pDateLabel = new wxStaticText(mainPanel, wxID_ANY, "Date Selected: --");
        mainSizer->Add(pDateLabel, wxSizerFlags().Border(wxALL, 5));

        pDateDotHFloorLabel = new wxStaticText(mainPanel, wxID_ANY, "date::floor = --");
        mainSizer->Add(pDateDotHFloorLabel, wxSizerFlags().Border(wxALL, 5));

        pDateDotHCeilLabel = new wxStaticText(mainPanel, wxID_ANY, "date::ceil = --");
        mainSizer->Add(pDateDotHCeilLabel, wxSizerFlags().Border(wxALL, 5));

        pDateCmpLabel = new wxStaticText(mainPanel, wxID_ANY, "Date Cmp: --");
        mainSizer->Add(pDateCmpLabel, wxSizerFlags().Border(wxALL, 5));
    }

    void FillControls() {
        auto dateTimeNow = wxDateTime::Now();
        auto dateTimeNowTicks = dateTimeNow.GetTicks();

        pDateLabel->SetLabel(wxString::Format("Date Selected:    %s", dateTimeNow.FormatISODate()));

        auto dateFloor = date::floor<date::days>(std::chrono::system_clock::from_time_t(dateTimeNowTicks));
        auto dateCeil = date::ceil<date::days>(std::chrono::system_clock::from_time_t(dateTimeNowTicks));

        pDateDotHFloorLabel->SetLabel(wxString::Format("date::floor =       %s", date::format("%F", dateFloor)));
        pDateDotHCeilLabel->SetLabel(wxString::Format("date::ceil =         %s", date::format("%F", dateCeil)));
    }

    wxDatePickerCtrl* pDateCtrl;
    wxStaticText* pDateLabel;
    wxStaticText* pDateDotHFloorLabel;
    wxStaticText* pDateDotHCeilLabel;
    wxStaticText* pDateCmpLabel;

    enum {
        wxdIDC_DATESLCTR = wxID_HIGHEST + 1001
    };
};

class Application : public wxApp
{
public:
    Application() = default;
    virtual ~Application() = default;

    bool OnInit() override {
        auto frame = new MainFrame();
        frame->Show(true);
        SetTopWindow(frame);

        return true;
    }
};

wxIMPLEMENT_APP(Application);
