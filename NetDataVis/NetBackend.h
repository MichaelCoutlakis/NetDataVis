#pragma once

#include "../../CppDataVisLib/CppDataVisLib/Backend.h"

#include <boost/asio/io_context.hpp>
#include <boost/asio.hpp>
#include <thread>

#include <string>
#include <iostream>

namespace dvis
{
	class cNetBackend : public cBackend
	{
	public:
		cNetBackend(uint16_t uPortNum);
		virtual ~cNetBackend() 
		{
			if (m_Thread.joinable())
				m_Thread.join();
		}
		void RenderFigure(cFigure* pFig) override;
		void RenderXY_Plot(cXY_Plot* pXY_Plot) override;


		void SayHi()
		{
			std::lock_guard<std::mutex> lock(m_mx);
			if (m_pSocket)
			{
				if (m_pSocket->is_open())
				{
					boost::asio::write
					(
						*(m_pSocket.get()),
						boost::asio::buffer(std::string("Hi!\n"))
					);
					std::cout << "Writing Hi!" << std::endl;
				}
				else
					std::cout << "Socket not open" << std::endl;
			}
			else
				std::cout << "Socket not connected" << std::endl;
		}
	private:
		boost::asio::io_context m_IO_Context;

		std::shared_ptr<boost::asio::ip::tcp::socket> m_pSocket;
		boost::asio::ip::tcp::acceptor m_Acceptor;


		void DoAccept();

		mutable std::mutex m_mx;
		std::thread m_Thread;
		//boost::asio
	};
}
