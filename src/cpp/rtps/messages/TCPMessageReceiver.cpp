// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @file TCPMessageReceiver.cpp
 *
 */

#include <fastrtps/transport/tcp/RTCPHeader.h>
#include <fastrtps/rtps/messages/TCPMessageReceiver.h>
#include <fastrtps/transport/SocketInfo.h>
#include <fastrtps/log/Log.h>

#define IDSTRING "(ID:" << std::this_thread::get_id() <<") "<<

using namespace eprosima::fastrtps;

namespace eprosima {
namespace fastrtps{
namespace rtps {

TCPMessageReceiver::TCPMessageReceiver()
{
}

TCPMessageReceiver::~TCPMessageReceiver()
{
}

bool TCPMessageReceiver::sendData(std::shared_ptr<TCPSocketInfo> &pSocketInfo, 
        const TCPHeader &header, const TCPControlMsgHeader &ctrlHeader,
        const octet *data, const uint32_t size)
{
    CDRMessage_t msg;
    CDRMessage::initCDRMsg(&msg);
    RTPSMessageCreator::addCustomContent(&msg, header.getAddress(), TCPHeader::GetSize());
    RTPSMessageCreator::addCustomContent(&msg, (octet*)(&ctrlHeader), sizeof(TCPControlMsgHeader));
    RTPSMessageCreator::addCustomContent(&msg, data, size);

    return pSocketInfo->getSocket()->write_some(asio::buffer(msg.buffer, msg.length)) > 0;
}

bool TCPMessageReceiver::sendData(std::shared_ptr<TCPSocketInfo> &pSocketInfo, 
        const TCPHeader &header, const TCPControlMsgHeader &ctrlHeader,
        const octet *data, const uint32_t size, const ResponseCode respCode)
{
    CDRMessage_t msg;
    CDRMessage::initCDRMsg(&msg);
    RTPSMessageCreator::addCustomContent(&msg, header.getAddress(), TCPHeader::GetSize());
    RTPSMessageCreator::addCustomContent(&msg, (octet*)(&ctrlHeader), sizeof(TCPControlMsgHeader));
    RTPSMessageCreator::addCustomContent(&msg, (octet*)(&respCode), 4); // uint32_t
    RTPSMessageCreator::addCustomContent(&msg, data, size);

    return pSocketInfo->getSocket()->write_some(asio::buffer(msg.buffer, msg.length)) > 0;
}

void TCPMessageReceiver::sendConnectionRequest(std::shared_ptr<TCPSocketInfo> &pSocketInfo, const Locator_t &transportLocator)
{
    TCPHeader header;
    TCPControlMsgHeader ctrlHeader;
    ConnectionRequest_t request;

    header.logicalPort = 0; // This is a control message
    ctrlHeader.length = sizeof(TCPControlMsgHeader) + sizeof(ControlProtocolRequestData);
    ctrlHeader.kind = BIND_CONNECTION_REQUEST;
    ctrlHeader.setFlags(false, true, true);
    ctrlHeader.setEndianess(DEFAULT_ENDIAN);
    header.length = ctrlHeader.length + TCPHeader::GetSize();
    request.transportLocator(transportLocator);

    sendData(pSocketInfo, header, ctrlHeader, (octet*)&request, request.GetSize());

    pSocketInfo->ChangeStatus(TCPSocketInfo::eConnectionStatus::eWaitingForBindResponse);
}

void TCPMessageReceiver::sendOpenLogicalPortRequest(std::shared_ptr<TCPSocketInfo> &pSocketInfo, OpenLogicalPortRequest_t &request)
{

}

void TCPMessageReceiver::sendCheckLogicalPortsRequest(std::shared_ptr<TCPSocketInfo> &pSocketInfo, CheckLogicalPortsRequest_t &request)
{

}

void TCPMessageReceiver::sendKeepAliveRequest(std::shared_ptr<TCPSocketInfo> &pSocketInfo, KeepAliveRequest_t &request)
{

}

void TCPMessageReceiver::sendLogicalPortIsClosedRequest(std::shared_ptr<TCPSocketInfo> &pSocketInfo, 
        LogicalPortIsClosedRequest_t &request)
{

}

void TCPMessageReceiver::processConnectionRequest(std::shared_ptr<TCPSocketInfo> &pSocketInfo, const ConnectionRequest_t &request,   
        Locator_t &localLocator)
{
    TCPHeader header;
    TCPControlMsgHeader ctrlHeader;
    BindConnectionResponse_t response;

    header.logicalPort = 0; // This is a control message
    ctrlHeader.length = sizeof(TCPControlMsgHeader) + sizeof(ControlProtocolResponseData);
    ctrlHeader.kind = BIND_CONNECTION_RESPONSE;
    ctrlHeader.setFlags(false, true, false);
    ctrlHeader.setEndianess(DEFAULT_ENDIAN);
    header.length = ctrlHeader.length + TCPHeader::GetSize();
    response.locator(localLocator);
 
    // TODO More options!
    if (pSocketInfo->mConnectionStatus == TCPSocketInfo::eConnectionStatus::eWaitingForBind)
    {

        sendData(pSocketInfo, header, ctrlHeader, (octet*)&response, response.GetSize(), RETCODE_OK);
        pSocketInfo->ChangeStatus(TCPSocketInfo::eConnectionStatus::eEstablished);
    }
    else
    {
        if (pSocketInfo->mConnectionStatus == TCPSocketInfo::eConnectionStatus::eEstablished)
        {
            sendData(pSocketInfo, header, ctrlHeader, (octet*)&response, response.GetSize(), RETCODE_EXISTING_CONNECTION);
        }
        else
        {
            sendData(pSocketInfo, header, ctrlHeader, (octet*)&response, response.GetSize(), RETCODE_SERVER_ERROR);
        }
    }
}

void TCPMessageReceiver::processOpenLogicalPortRequest(std::shared_ptr<TCPSocketInfo> &pSocketInfo, 
        const OpenLogicalPortRequest_t &request)
{

}

void TCPMessageReceiver::processCheckLogicalPortsRequest(std::shared_ptr<TCPSocketInfo> &pSocketInfo, 
        const CheckLogicalPortsRequest_t &request)
{

}

void TCPMessageReceiver::processKeepAliveRequest(std::shared_ptr<TCPSocketInfo> &pSocketInfo, const KeepAliveRequest_t &request)
{

}

void TCPMessageReceiver::processLogicalPortIsClosedRequest(std::shared_ptr<TCPSocketInfo> &pSocketInfo, 
        const LogicalPortIsClosedRequest_t &request)
{

}

void TCPMessageReceiver::processBindConnectionResponse(std::shared_ptr<TCPSocketInfo> &pSocketInfo, 
        const BindConnectionResponse_t &response)
{

}

void TCPMessageReceiver::processCheckLogicalPortsResponse(std::shared_ptr<TCPSocketInfo> &pSocketInfo, 
        const CheckLogicalPortsResponse_t &response)
{

}

void TCPMessageReceiver::processResponse(std::shared_ptr<TCPSocketInfo> &pSocketInfo, const ControlProtocolResponseData &response)
{

}


} /* namespace rtps */
} /* namespace fastrtps */
} /* namespace eprosima */