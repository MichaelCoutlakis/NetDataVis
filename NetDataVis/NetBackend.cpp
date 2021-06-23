/***************************************************************************//**
 * \brief		Implementation of a network backend (renders the data
 *				visualization over the network)
 * \copyright	Michael Coutlakis (2021)
 * \license		MIT License, see the LICENSE file.
 *******************************************************************************/
#include <iostream>

#include "NetBackend.h"
#include "../../CppDataVisLib/CppDataVisLib/Figure.h"
#include "../../CppDataVisLib/CppDataVisLib/XY_Plot.h"

using boost::asio::ip::tcp;

dvis::NetBackend::NetBackend(uint16_t uPortNum) :
	m_acceptor(m_IO_context, tcp::endpoint(tcp::v4(), uPortNum))
	//m_Socket(m_IO_context)
{
	// Waiting for connection... .
	DoAccept();
	m_IO_thread = std::thread([this]() {m_IO_context.run(); });
}

void dvis::NetBackend::RenderFigure(Figure* fig)
{
	m_packet.m_figure = std::make_unique<pkt::FigureT>();
	m_packet.m_figure->m_title = fig->Title();
	m_packet.m_figure->m_ID = fig->ID();

	//std::lock_guard<std::mutex> lock(m_mx);
	//if(IsConnected())
	//{
	//	try
	//	{
	//		pkt::NetPacketT packet;
	//		packet.m_figure = std::make_unique<pkt::FigureT>();
	//		packet.m_figure->m_title = fig->Title();
	//		packet.m_figure->m_ID = fig->ID();

	//		flatbuffers::FlatBufferBuilder builder = ToBuffer(packet);
	//		std::cout << "Made flatbuffer with size " << builder.GetSize() << std::endl;

	//		size_t num_bytes_written = boost::asio::write
	//		(
	//			*m_socket.get(),
	//			boost::asio::const_buffer(builder.GetBufferPointer(), builder.GetSize())
	//		);
	//		std::cout << "Wrote " << num_bytes_written << " bytes " << std::endl;
	//	}
	//	catch(...)
	//	{
	//		m_socket->close();
	//	}
	//}
	//else
	//	std::cout << "Socket not connected" << std::endl;
}

void dvis::NetBackend::RenderXY_Plot(XY_Plot* xy_plot)
{
	std::lock_guard<std::mutex> lock(m_mx);
	if(IsConnected())
	{
		try
		{
			auto& plot = m_packet.m_figure->m_xy_plot;
			plot = std::make_unique<pkt::XY_PlotT>();

			if(xy_plot->m_x_axis_bottom)
				plot->m_x_label = xy_plot->m_x_axis_bottom->m_label;
			if(xy_plot->m_y_axis_left)
				plot->m_y_label = xy_plot->m_y_axis_left->m_label;

			plot->m_series.push_back(std::make_unique<pkt::XY_SeriesT>());
			auto& series0 = plot->m_series.back();
			series0->m_x = xy_plot->m_x;
			series0->m_y = xy_plot->m_y;

			flatbuffers::FlatBufferBuilder builder = ToBuffer(m_packet);
			std::cout << "Made flatbuffer with size " << builder.GetSize() << std::endl;

			size_t num_bytes_written = boost::asio::write
			(
				*m_socket.get(),
				boost::asio::const_buffer(builder.GetBufferPointer(), builder.GetSize())
			);
			std::cout << "Wrote " << num_bytes_written << " bytes " << std::endl;
		}
		catch(...)
		{
			m_socket->close();
		}
	}
	else
		std::cout << "Socket not connected" << std::endl;
}




void dvis::NetBackend::DoAccept()
{
	m_acceptor.async_accept
	(
		[this](boost::system::error_code ec, tcp::socket Socket)
		{
			if(!ec)
			{
				std::lock_guard<std::mutex> lock(m_mx);
				std::cout << "Accepted connection!" << std::endl;
				m_socket = std::make_shared<tcp::socket>(std::move(Socket));
			}
			// Failed, try another connection
			DoAccept();
		}
	);
}
