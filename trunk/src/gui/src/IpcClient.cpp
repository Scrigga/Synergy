/*
 * synergy -- mouse and keyboard sharing utility
 * Copyright (C) 2012 Nick Bolton
 *
 * This package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * found in the file COPYING that should have accompanied this file.
 *
 * This package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "IpcClient.h"
#include <QTcpSocket>
#include <QHostAddress>
#include <iostream>
#include <QTimer>
#include "IpcReader.h"
#include "Ipc.h"

IpcClient::IpcClient()
{
	m_Socket = new QTcpSocket(this);
	m_Reader = new IpcReader(m_Socket);
	connect(m_Socket, SIGNAL(connected()), this, SLOT(connected()));
	connect(m_Socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
	connect(m_Reader, SIGNAL(readLogLine(const QString&)), this, SLOT(handleReadLogLine(const QString&)));
}

IpcClient::~IpcClient()
{
}

void IpcClient::connected()
{
	char typeBuf[1];
	typeBuf[0] = kIpcClientGui;
	write(kIpcHello, 1, typeBuf);

	infoMessage("connection established");

	m_Reader->start();
}

void IpcClient::connectToHost()
{
	infoMessage("connecting to service...");
	m_Socket->connectToHost(QHostAddress(QHostAddress::LocalHost), IPC_PORT);
}

void IpcClient::error(QAbstractSocket::SocketError error)
{
	QString text;
	switch (error) {
		case 0: text = "connection refused"; break;
		case 1: text = "remote host closed"; break;
		default: text = QString("code=%1").arg(error); break;
	}

	errorMessage(QString("ipc connection error, %1").arg(text));

	QTimer::singleShot(1000, this, SLOT(connectToHost()));
}

void IpcClient::write(int code, int length, const char* data)
{
	QDataStream stream(m_Socket);

	char codeBuf[1];
	codeBuf[0] = code;
	stream.writeRawData(codeBuf, 1);

	switch (code) {
	case kIpcHello:
		stream.writeRawData(data, 1);
		break;

	case kIpcCommand: {
		char lenBuf[2];
		intToBytes(length, lenBuf, 2);
		stream.writeRawData(lenBuf, 2);
		stream.writeRawData(data, length);
		break;
	}

	default:
		std::cerr << "message type not supported: " << code << std::endl;
		break;
	}
}

void IpcClient::handleReadLogLine(const QString& text)
{
	readLogLine(text);
}

// TODO: qt must have a built in way of converting int to bytes.
void IpcClient::intToBytes(int value, char *buffer, int size)
{
	if (size == 2) {
		buffer[0] = (value >> 8) & 0xff;
		buffer[1] = value & 0xff;
	}
	else {
		// TODO: other sizes, if needed.
	}
}
