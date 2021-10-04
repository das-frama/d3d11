#pragma once
#include "pch.h"

/**
 * @brief Helper class for DirectX exceptions
 */
class DXException : public std::exception
{
public:
    DXException(int line, const char *file, HRESULT hr) noexcept
        : line(line), file(file), hr(hr) {}

    virtual const char *what() const noexcept override
    {
        std::ostringstream oss;
        oss << GetType() << std::endl
            << "[Error Code] " << GetErrorCode() << std::endl
            << "[Description] " << GetErrorString() << std::endl
            << GetOriginString();
        whatBuffer = oss.str();
        return whatBuffer.c_str();
    }

    virtual const char *GetType() const noexcept
    {
        return "DirectX Exception";
    }

    int GetLine() const noexcept
    {
        return line;
    }

    const std::string &GetFile() const noexcept
    {
        return file;
    }

    std::string GetOriginString() const noexcept
    {
        std::ostringstream oss;
        oss << "[File] " << file << std::endl
            << "[Line] " << line;
        return oss.str();
    }

    static std::string TranslateErrorCode(HRESULT hr) noexcept
    {
        char *pMsgBuf = nullptr;
        DWORD nMsgLen = FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, hr,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            reinterpret_cast<LPWSTR>(&pMsgBuf), 0, nullptr);

        if (nMsgLen == 0)
        {
            return "Unidentified error code";
        }

        std::string errorString = pMsgBuf;
        LocalFree(pMsgBuf);
        return errorString;
    }

    HRESULT GetErrorCode() const noexcept
    {
        return hr;
    }

    std::string GetErrorString() const noexcept
    {
        return TranslateErrorCode(hr);
    }

private:
    int line;
    std::string file;
    HRESULT hr;

protected:
    mutable std::string whatBuffer;
};


inline void ThrowIfFailed(int line, const char *file, HRESULT hr)
{
    if (FAILED(hr))
    {
        // Set a breakpoint on this line to catch DirectX API errors
        throw DXException(line, file, hr);
    }
}

#define DX_LAST_ERROR_EXCEPTION DXException(__LINE__, __FILE__, GetLastError())
#define CALL_INFO __LINE__, __FILE__