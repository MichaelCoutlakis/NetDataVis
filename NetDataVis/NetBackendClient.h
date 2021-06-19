/***************************************************************************//**
* \brief		Client to receive data from the NetBackend.
* \copyright	Michael Coutlakis 2021
* \license		MIT License, see the LICENSE file.
*******************************************************************************/
#pragma once


#include <thread>
#include <iostream>

#include <boost/asio/io_context.hpp>
#include <boost/asio.hpp>

#include "Schemas/NetBackendSchema_generated.h"
#include "FlatbufferUtils.h"

namespace dvis
{
class NetBackendClient
{
public:
	NetBackendClient(std::string IP, unsigned port) :
		m_resolver(m_IO_context),
		m_socket(m_IO_context),
		m_buf(4096)
	{
		auto endpoints = m_resolver.resolve(IP, std::to_string(port));
		DoConnect(endpoints);

		m_thread = std::thread([this]() {m_IO_context.run(); });
	}
	~NetBackendClient()
	{
		if (m_thread.joinable())
		{
			m_thread.join();
		}
	}
private:
	boost::asio::io_context m_IO_context;
	boost::asio::ip::tcp::socket m_socket;
	boost::asio::ip::tcp::resolver m_resolver;
	std::thread m_thread;

	std::string m_strRead{ ' ', 100 };

	std::vector<uint8_t> m_buf;

	void DoConnect(const boost::asio::ip::tcp::resolver::results_type& endpoints)
	{
		boost::asio::async_connect(m_socket, endpoints,
			[this](boost::system::error_code ec, boost::asio::ip::tcp::endpoint)
			{
				if (!ec)
				{
					std::cout << "Connected!" << std::endl;
					DoReadPacketSize();
				}
				else
				{
					std::cout << "DoConnect: Error Code " << ec << std::endl;
				}
			});
	}
	/// @brief Read in just the first 4 bytes of the packet, which will be the size due to flatbuffers size prefix.
	void DoReadPacketSize()
	{
		boost::asio::async_read(m_socket,
			boost::asio::buffer(m_buf.data(), 4U),
			[this](boost::system::error_code ec, std::size_t num_bytes)
			{
				if (!ec)
				{
					std::cout << "Got " << num_bytes << " bytes !!" << std::endl;
					//// Deserialize the buffer:
					//fbNet::NetPacketT NetPacket;
					//flatbuffers::GetSizePrefixedRoot<fbNet::NetPacket>(m_vBuf.data())->UnPackTo(&NetPacket);
					//if (NetPacket.m_Figure)
					//	std::cout << "The figure title is " << NetPacket.m_Figure->m_title << std::endl;
					//else
					//	std::cout << "No figure recieved" << std::endl;

					uint32_t uPacketSize = *(uint32_t*)(m_buf.data());
					std::cout << "Packet size is " << uPacketSize << std::endl;
					DoReadPacket(uPacketSize);
				}
				else
				{
					m_socket.close();
				}
			});
	}
	void DoReadPacket(size_t packet_size)
	{
		if (packet_size + 4U >= m_buf.size())
		{
			std::cout << "Warning: Packet was too big for allocated buffer, resizing... ." << std::endl;
			m_buf.resize(4U + packet_size);
		}
		boost::asio::async_read(m_socket,
			boost::asio::buffer(m_buf.data() + 4U, packet_size),
			[this, packet_size](boost::system::error_code ec, std::size_t n_rx_bytes)
			{
				if (!ec)
				{
					if (n_rx_bytes == packet_size)
					{
						std::cout << "Got " << n_rx_bytes << " bytes !!" << std::endl;
						// Deserialize the buffer:
						pkt::NetPacketT packet;
						flatbuffers::GetSizePrefixedRoot<pkt::NetPacket>(m_buf.data())->UnPackTo(&packet);
						if (packet.m_figure)
							std::cout << "The figure title is " << packet.m_figure->m_title << std::endl;
						else
							std::cout << "No figure recieved" << std::endl;

						// Read the next packet size
						DoReadPacketSize();
					}
					else
					{
						std::cout << "Incorrect size of read: Expected: " << packet_size << " got " << n_rx_bytes << std::endl;
					}
				}
				else
				{
					m_socket.close();
				}
			});

		//boost::asio::async_read(m_Socket,
		//	boost::asio::buffer(m_strRead),
		//	[this](boost::system::error_code ec, std::size_t)
		//	{
		//		if (!ec)
		//		{
		//			std::cout << m_strRead << std::endl;
		//			DoRead();
		//		}
		//		else
		//		{
		//			m_Socket.close();
		//		}
		//	});
	}
};
}