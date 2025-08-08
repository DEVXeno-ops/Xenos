#pragma once

#include "stdafx.h"
#include <BlackBone/src/BlackBone/Misc/Thunk.hpp>
#include <string>
#include <string_view>
#include <map>

#define MSG_HANDLER(n) virtual INT_PTR n(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)

class Window
{
public:
    using fnWndProc = INT_PTR(Window::*)(HWND, UINT, WPARAM, LPARAM);
    using mapWndProc = std::map<UINT, std::pair<Window*, fnWndProc>>;

public:
    explicit Window(HWND hwnd = nullptr)
        : _hwnd(hwnd)
        , _subThunk(&Window::SubProc, this) 
    { }

    virtual ~Window()
    {
        if (_oldProc && _hwnd)
        {
            SetWindowLongPtrW(_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(_oldProc));
        }
    }

    virtual inline void Attach(HWND hwnd) 
    { 
        _hwnd = hwnd; 
    }

    virtual inline void Attach(HWND hDlg, UINT id) 
    { 
        _id = id; 
        _hwnd = GetDlgItem(hDlg, id); 
    }

    [[nodiscard]] virtual inline HWND hwnd() const { return _hwnd; }
    [[nodiscard]] virtual inline UINT id() const { return _id; }

    virtual inline void enable()  const 
    { 
        if (_hwnd) EnableWindow(_hwnd, TRUE); 
    }

    virtual inline void disable() const 
    { 
        if (_hwnd) EnableWindow(_hwnd, FALSE); 
    }

    [[nodiscard]] virtual std::wstring text() const
    {
        if (!_hwnd) return {};
        
        wchar_t buf[512] = { 0 };
        GetWindowTextW(_hwnd, buf, static_cast<int>(std::size(buf)));
        return buf;
    }

    virtual inline BOOL text(std::wstring_view newText)
    {
        return _hwnd ? SetWindowTextW(_hwnd, newText.data()) : FALSE;
    }

    [[nodiscard]] virtual inline WNDPROC oldProc() const { return _oldProc; }

    virtual void Subclass(UINT message, fnWndProc handler, Window* instance = nullptr)
    {
        if (!handler)
        {
            // Remove old handler
            if (_subMessages.erase(message) > 0 && _subMessages.empty() && _oldProc && _hwnd)
            {
                SetWindowLongPtrW(_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(_oldProc));
                _oldProc = nullptr;
            }
        }
        else
        {
            _subMessages[message] = { instance ? instance : this, handler };
            if (!_oldProc && _hwnd)
            {
                _oldProc = reinterpret_cast<WNDPROC>(
                    SetWindowLongPtrW(_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(_subThunk.GetThunk()))
                );
            }
        }
    }

private:
    LRESULT SubProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        auto it = _subMessages.find(message);
        if (it != _subMessages.end())
        {
            return (it->second.first->*it->second.second)(hwnd, message, wParam, lParam);
        }
        return CallWindowProcW(_oldProc, hwnd, message, wParam, lParam);
    }

protected:
    HWND _hwnd = nullptr;
    UINT _id = 0;
    WNDPROC _oldProc = nullptr;
    mapWndProc _subMessages;
    Win32Thunk<WNDPROC, Window> _subThunk;
};
