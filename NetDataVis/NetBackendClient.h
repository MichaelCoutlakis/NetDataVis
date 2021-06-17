#pragma once

#include <iostream>
#include <boost/asio/io_context.hpp>
#include <boost/asio.hpp>

#include <thread>

namespace dvis
{
	class cNetBackendClient
	{
	public:
		cNetBackendClient(std::string strIP, unsigned uPort) :
			m_Resolver(m_IO_Context),
			m_Socket(m_IO_Context)
		{
			auto Endpoints = m_Resolver.resolve(strIP, std::to_string(uPort));
			DoConnect(Endpoints);

			m_Thread = std::thread([this]() {m_IO_Context.run(); });
		}
		~cNetBackendClient()
		{
			if (m_Thread.joinable())
			{
				m_Thread.join();
			}
		}
	private:
		boost::asio::io_context m_IO_Context;
		boost::asio::ip::tcp::socket m_Socket;
		boost::asio::ip::tcp::resolver m_Resolver;
		std::thread m_Thread;

		std::string m_strRead{ ' ', 100 };

		void DoConnect(const boost::asio::ip::tcp::resolver::results_type& Endpoints)
		{
			boost::asio::async_connect(m_Socket, Endpoints,
				[this](boost::system::error_code ec, boost::asio::ip::tcp::endpoint)
				{
					if (!ec)
					{
						std::cout << "Connected!" << std::endl;
						DoRead();
					}
					else
					{
						std::cout << "DoConnect: Error Code " << ec << std::endl;
					}
				});
		}
		void DoRead()
		{

			boost::asio::async_read(m_Socket,
				boost::asio::buffer(m_strRead),
				[this](boost::system::error_code ec, std::size_t)
				{
					if (!ec)
					{
						std::cout << m_strRead << std::endl;
						DoRead();
					}
					else
					{
						m_Socket.close();
					}
				});
		}
	};
}