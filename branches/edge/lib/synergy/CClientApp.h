/*
* synergy -- mouse and keyboard sharing utility
* Copyright (C) 2002 Chris Schoeneman
* 
* This package is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* found in the file COPYING that should have accompanied this file.
* 
* This package is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/

#pragma once

#include "CApp.h"
#include "CString.h"
#include "CNetworkAddress.h"

class CClientApp : virtual public CApp {
public:
	CClientApp();
	virtual ~CClientApp();

	class CArgs {
	public:
		CArgs();
		~CArgs();

	public:
		static CArgs*		s_instance;
		const char* 		m_pname;
		bool				m_backend;
		bool				m_restartable;
		bool				m_daemon;
		int 				m_yscroll;
		const char* 		m_logFilter;
		const char*			m_display;
		CString 			m_name;
		CNetworkAddress* 	m_serverAddress;
		const char*			m_logFile;
	};
};
