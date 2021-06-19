#include <iostream>

#include "NetBackend.h"

using boost::asio::ip::tcp;

dvis::NetBackend::NetBackend(uint16_t uPortNum) :
	m_acceptor(m_IO_context, tcp::endpoint(tcp::v4(), uPortNum))
	//m_Socket(m_IO_context)
{
	// Waiting for connection... .
	DoAccept();
	m_IO_thread = std::thread([this]() {m_IO_context.run(); });
}

void dvis::NetBackend::RenderFigure(Figure* pFig)
{
}

void dvis::NetBackend::RenderXY_Plot(XY_Plot* pXY_Plot)
{
}

void dvis::NetBackend::DoAccept()
{
	m_acceptor.async_accept(
		[this](boost::system::error_code ec, tcp::socket Socket)
		{
			if (!ec)
			{
				std::lock_guard<std::mutex> lock(m_mx);
				std::cout << "Accepted connection!" << std::endl;
				m_socket = std::make_shared<tcp::socket>(std::move(Socket));
			}
			// Failed, try another connection
			DoAccept();
		});
}
