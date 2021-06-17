#include <iostream>

#include "NetBackend.h"

using boost::asio::ip::tcp;

dvis::cNetBackend::cNetBackend(uint16_t uPortNum) :
	m_Acceptor(m_IO_Context, tcp::endpoint(tcp::v4(), uPortNum))
	//m_Socket(m_IO_Context)
{
	// Waiting for connection... .
	DoAccept();
	m_Thread = std::thread([this]() {m_IO_Context.run(); });
}

void dvis::cNetBackend::RenderFigure(cFigure* pFig)
{
}

void dvis::cNetBackend::RenderXY_Plot(cXY_Plot* pXY_Plot)
{
}

void dvis::cNetBackend::DoAccept()
{
	m_Acceptor.async_accept(
		[this](boost::system::error_code ec, tcp::socket Socket)
		{
			if (!ec)
			{
				std::lock_guard<std::mutex> lock(m_mx);
				std::cout << "Accepted connection!" << std::endl;
				m_pSocket = std::make_shared<tcp::socket>(std::move(Socket));
			}
			// Failed, try another connection
			DoAccept();
		});
}
