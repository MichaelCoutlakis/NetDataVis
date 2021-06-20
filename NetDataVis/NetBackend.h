/***************************************************************************//**
* \brief		
* \copyright	Michael Coutlakis 2021
* \license		MIT License, see the LICENSE file.
*******************************************************************************/
#pragma once

#include "../../CppDataVisLib/CppDataVisLib/Backend.h"

#include "Schemas/NetBackendSchema_generated.h"
#include "FlatbufferUtils.h"

#include <boost/asio/io_context.hpp>
#include <boost/asio.hpp>
#include <thread>

#include <string>
#include <iostream>

namespace dvis
{
class NetBackend : public Backend
{
public:
	NetBackend(uint16_t port_num);
	virtual ~NetBackend() 
	{
		if (m_IO_thread.joinable())
			m_IO_thread.join();
	}
	void RenderFigure(Figure* fig) override;
	void RenderXY_Plot(Figure* fig, XY_Plot* xy_plot) override;


	void SayHi()
	{
		std::lock_guard<std::mutex> lock(m_mx);
		if (m_socket)
		{
			if (m_socket->is_open())
			{
				//boost::asio::async_write(*m_socket.get(),
				//	boost::asio::buffer(std::string("Hi!")),
				//	[](boost::system::error_code ec, std::size_t)
				//	{
				//		if (!ec)
				//		{

				//		}
				//		else
				//		{
				//			std::cout << "Write Failed" << std::endl;
				//		}
				//	});
				try
				{
					boost::asio::write
					(
						*(m_socket.get()),
						boost::asio::buffer(std::string("Hi!\n"))
					);
				}
				catch (...)
				{
					std::cout << "Caught " << std::endl;
					m_socket->close();
				}
				std::cout << "Writing Hi!" << std::endl;
			}
			else
				std::cout << "Socket not open" << std::endl;
		}
		else
			std::cout << "Socket not connected" << std::endl;
	}
	void SendPacket()
	{
		std::lock_guard<std::mutex> lock(m_mx);
		if (m_socket)
		{
			if (m_socket->is_open())
			{
				try
				{
					pkt::NetPacketT packet;
					packet.m_figure = std::make_unique<pkt::FigureT>();
					packet.m_figure->m_title = "Figure Title";
					flatbuffers::FlatBufferBuilder builder = ToBuffer(packet);
					std::cout << "Made flatbuffer with size " << builder.GetSize() << std::endl;

					size_t num_bytes_written = boost::asio::write
					(
						*m_socket.get(),
						boost::asio::const_buffer(builder.GetBufferPointer(), builder.GetSize())
					);
					std::cout << "Wrote " << num_bytes_written << " bytes " << std::endl;
				}
				catch (...)
				{
					m_socket->close();
				}
			}
		}
		else
			std::cout << "Socket not connected" << std::endl;
	}
private:
	boost::asio::io_context m_IO_context;

	std::shared_ptr<boost::asio::ip::tcp::socket> m_socket;
	boost::asio::ip::tcp::acceptor m_acceptor;

	bool IsConnected() { return m_socket && m_socket->is_open(); }
	void DoAccept();

	mutable std::mutex m_mx;
	std::thread m_IO_thread;
	//boost::asio
};
}
