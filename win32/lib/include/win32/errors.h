#pragma once

#include <wchar.h>

#if defined (__cplusplus)

#include <string>
#include <exception>
#include <cstdint>

namespace windows {

    class win32_error : public std::exception {
        private:
            uint32_t m_error_code;

            std::wstring m_message;
        public:
            win32_error(uint32_t error_code) {
                m_error_code = error_code;
            }

            win32_error(uint32_t error_code, const std::wstring& msg) {
                m_message = msg;
                m_error_code = error_code;
            }

            win32_error(uint32_t error_code, const wchar_t *msg) {
                m_message = std::wstring{msg};
                m_error_code = error_code;
            }
    };

    class message_format_error : public win32_error {
        public:
            using win32_error::win32_error;
    };

    std::wstring get_error_message(std::uint32_t error);
}

#endif
