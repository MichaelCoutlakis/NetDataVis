/***************************************************************************//**
* \brief
* \copyright	Michael Coutlakis 2021
* \license		MIT License, see the LICENSE file.
*******************************************************************************/
#pragma once
#include <flatbuffers/flatbuffers.h>

#include <boost/asio.hpp>


//template<typename FlatBufferT>
//boost::asio::const_buffer ToBuffer(const FlatBufferT& FlatBuf)
//{
//	// Serialize to buffer:
//	flatbuffers::FlatBufferBuilder builder;
//	builder.FinishSizePrefixed(std::remove_reference<decltype(FlatBuf))
//	size_t uSize = 10;
//	void* pData = nullptr;
//	return boost::asio::const_buffer(pData, uSize);
//}

namespace dvis
{

	inline flatbuffers::FlatBufferBuilder ToBuffer(const pkt::NetPacketT& packet)
	{
		// Serialize to buffer:
		flatbuffers::FlatBufferBuilder builder;
		builder.FinishSizePrefixed(pkt::NetPacket::Pack(builder, &packet));

		return builder;
	}
}