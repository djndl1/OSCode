#ifndef REPORTERROR_H_
#define REPORTERROR_H_

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

VOID ReportError(LPCTSTR userMessage, DWORD exitCode, BOOL printErrorMessage);

void ReportException(LPCTSTR userMessage, DWORD exceptionCode);

#ifdef __cplusplus
}
#endif

#endif // REPORTERROR_H_
