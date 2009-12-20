
#ifndef MIC_EVENT_HANDLER_H
#define MIC_EVENT_HANDLER_H

// Workaround for _T defined in engine/unicode.h
#if defined(_T)
#undef _T
#endif

// MIC interface
#include <atlbase.h>
#include <atlcom.h>
#include <micaut.h>


static const int MATHINPUTCONTROL_SINK_ID = 1;


template <class T>
class ATL_NO_VTABLE MICEventHandler :
    public IDispEventSimpleImpl<MATHINPUTCONTROL_SINK_ID, MICEventHandler<T>, &__uuidof(_IMathInputControlEvents)>
{
private:
    IUnknown * mUnknown;        ///< Pointer to MIC

public:
    static const _ATL_FUNC_INFO mOnMICInsertInfo; // = {CC_STDCALL, VT_I4, 1, {VT_BSTR}};
    static const _ATL_FUNC_INFO mOnMICCloseInfo;  // = {CC_STDCALL, VT_I4, 0, {VT_EMPTY}};

    // Event handling map
    BEGIN_SINK_MAP(MICEventHandler)
        SINK_ENTRY_INFO(MATHINPUTCONTROL_SINK_ID, __uuidof(_IMathInputControlEvents), DISPID_MICInsert, OnMICInsert, const_cast<_ATL_FUNC_INFO*>(&mOnMICInsertInfo))
        SINK_ENTRY_INFO(MATHINPUTCONTROL_SINK_ID, __uuidof(_IMathInputControlEvents), DISPID_MICClose, OnMICClose, const_cast<_ATL_FUNC_INFO*>(&mOnMICCloseInfo))
    END_SINK_MAP()


    /// Initializes MICEventHandler with pointer to MIC
    void Initialize(IUnknown * unknown)
    {
        mUnknown = unknown;
    }

    /// Insert button handler
    HRESULT __stdcall OnMICInsert(BSTR recognitionResult)
    {
        CComQIPtr<IMathInputControl> mic(mUnknown);
        if (mic) {
            onMathInput(recognitionResult);
            return S_OK;
        }
        return E_FAIL;
    }

    /// Close button handler
    HRESULT __stdcall OnMICClose()
    {
        CComPtr<IMathInputControl> mic;
        HRESULT hr = mUnknown->QueryInterface<IMathInputControl>(&mic);
        if (SUCCEEDED(hr)) {
            hr = mic->Hide();
        }
        return hr;
    }

    /// Math input handler which is called on MathML input from MIC
    virtual void onMathInput(BSTR mathml) = 0;
};

#endif // MIC_EVENT_HANDLER_H
