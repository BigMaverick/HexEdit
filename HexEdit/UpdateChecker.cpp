// UpdateChecker.cpp - implements UpdateChecker class for checking for new versions
//
// Copyright (c) 2010 by Andrew W. Phillips.
//
// No restrictions are placed on the noncommercial use of this code,
// as long as this text (from the above copyright notice to the
// disclaimer below) is preserved.
//
// This code may be redistributed as long as it remains unmodified
// and is not sold for profit without the author's written consent.
//
// This code, or any part of it, may not be used in any software that
// is sold for profit, without the author's written consent.
//
// DISCLAIMER: This file is provided "as is" with no expressed or
// implied warranty. The author accepts no liability for any damage
// or loss of business that this product may cause.
//

#include "stdafx.h"
#include <Wininet.h>
#include "UpdateChecker.h"

// Returns true if the version is able to be checked - ie, the 
// Internet is active and the URL is available.
bool UpdateChecker::Online()
{
	return ::InternetCheckConnection(strUrl, FLAG_ICC_FORCE_CONNECTION, 0) != 0;
}

// Checks the current version (curr) against the latest version number stored
// in a file on the Internet.  All numbers are simple integers greater than 100.
// For example, version 4.10 is stored as the 3 digits "410".
// Returns false if a newer version is not available or there is an error, else true.
bool UpdateChecker::UpdateAvailable(int curr)
{
	bool retval = false;

	HINTERNET hint, hurl;
	hint = ::InternetOpen(_T("Update"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hint == (HINTERNET)0)
		return false;

	hurl = InternetOpenUrl(hint, strUrl, NULL, -1,
	                       INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE |
	                           INTERNET_FLAG_PRAGMA_NOCACHE | WININET_API_FLAG_ASYNC,
	                       NULL);

	char buf[8];            // stores bytes read from current version file on the Internet
	DWORD nread = 0;        // number of bytes read into buf
	if (hurl != (HINTERNET)0 &&	::InternetReadFile(hurl, buf, sizeof(buf)-1, &nread))
	{
		// File (or at least start of file) read OK
		buf[nread] = '\0';    // ensure string is null terminated
		int ver = atoi(buf);  // get version (eg "350" means version 3.50

		if (ver > curr)
			retval = true;

		::InternetCloseHandle(hurl);
	}
	::InternetCloseHandle(hint);

	return retval;
}
