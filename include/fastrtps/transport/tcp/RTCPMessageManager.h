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
 * @file RTCPMessageManager.h
 */



#ifndef RTCP_MESSAGEMANAGER_H_
#define RTCP_MESSAGEMANAGER_H_
#ifndef DOXYGEN_SHOULD_SKIP_THIS_PUBLIC
#include "../../qos/ParameterList.h"
#include <fastrtps/rtps/common/all_common.h>
#include <fastrtps/rtps/writer/StatelessWriter.h>
#include <fastrtps/rtps/writer/StatefulWriter.h>
#include <fastrtps/transport/tcp/TCPControlMessage.h>
#include <fastrtps/transport/tcp/RTCPHeader.h>
#include <fastrtps/transport/TCPv4Transport.h>

namespace eprosima {
namespace fastrtps{
namespace rtps {

class TCPSocketInfo;
class TCPv4Transport;

/**
 * Class RTCPMessageManager, process the received TCP messages.
 * @ingroup MANAGEMENT_MODULE
 */
class RTCPMessageManager
{
public:

    RTCPMessageManager(TCPv4Transport* tcpv4_transport) : transport(tcpv4_transport) {}
    virtual ~RTCPMessageManager();

    void sendConnectionRequest(std::shared_ptr<TCPSocketInfo> pSocketInfo, uint16_t localLogicalPort);
    void sendOpenLogicalPortRequest(std::shared_ptr<TCPSocketInfo> pSocketInfo, OpenLogicalPortRequest_t &request);
    void sendOpenLogicalPortRequest(std::shared_ptr<TCPSocketInfo> pSocketInfo, uint16_t port);
    void sendCheckLogicalPortsRequest(std::shared_ptr<TCPSocketInfo> pSocketInfo, CheckLogicalPortsRequest_t &request);
    void sendCheckLogicalPortsRequest(std::shared_ptr<TCPSocketInfo> pSocketInfo, std::vector<uint16_t> &ports);
    void sendKeepAliveRequest(std::shared_ptr<TCPSocketInfo> pSocketInfo, KeepAliveRequest_t &request);
    void sendKeepAliveRequest(std::shared_ptr<TCPSocketInfo> pSocketInfo);
    void sendLogicalPortIsClosedRequest(std::shared_ptr<TCPSocketInfo> pSocketInfo,
        LogicalPortIsClosedRequest_t &request);
    void sendLogicalPortIsClosedRequest(std::shared_ptr<TCPSocketInfo> pSocketInfo, uint16_t port);
    void sendUnbindConnectionRequest(std::shared_ptr<TCPSocketInfo> pSocketInfo);

    void processConnectionRequest(std::shared_ptr<TCPSocketInfo> pSocketInfo, const ConnectionRequest_t &request,
        const TCPTransactionId &transactionId, Locator_t &localLocator);
    void processOpenLogicalPortRequest(std::shared_ptr<TCPSocketInfo> pSocketInfo,
        const OpenLogicalPortRequest_t &request, const TCPTransactionId &transactionId);
    void processCheckLogicalPortsRequest(std::shared_ptr<TCPSocketInfo> pSocketInfo,
        const CheckLogicalPortsRequest_t &request, const TCPTransactionId &transactionId);
    void processKeepAliveRequest(std::shared_ptr<TCPSocketInfo> pSocketInfo,
        const KeepAliveRequest_t &request, const TCPTransactionId &transactionId);
    void processLogicalPortIsClosedRequest(std::shared_ptr<TCPSocketInfo> pSocketInfo,
        const LogicalPortIsClosedRequest_t &request, const TCPTransactionId &transactionId);

    bool processBindConnectionResponse(std::shared_ptr<TCPSocketInfo> pSocketInfo,
        const BindConnectionResponse_t &response, const TCPTransactionId &transactionId);
    bool processCheckLogicalPortsResponse(std::shared_ptr<TCPSocketInfo> pSocketInfo,
        const CheckLogicalPortsResponse_t &response, const TCPTransactionId &transactionId);
    bool processOpenLogicalPortResponse(std::shared_ptr<TCPSocketInfo> pSocketInfo,
        ResponseCode respCode, const TCPTransactionId &transactionId, Locator_t &remoteLocator);
    bool processKeepAliveResponse(std::shared_ptr<TCPSocketInfo> pSocketInfo,
        ResponseCode respCode, const TCPTransactionId &transactionId);

    void processRTCPMessage(std::shared_ptr<TCPSocketInfo> socketInfo, octet* receiveBuffer);
    // data must contain full RTCP message without the TCPHeader
    static bool CheckCRC(const TCPHeader &header, const octet *data, uint32_t size);
    static void CalculateCRC(TCPHeader &header, const octet *data, uint32_t size);

protected:
    TCPv4Transport* transport;
    std::set<TCPTransactionId> mUnconfirmedTransactions;

    void prepareAndSendCheckLogicalPortsRequest(std::shared_ptr<TCPSocketInfo> pSocketInfo);

private:
    TCPTransactionId myTransId;
    std::recursive_mutex mutex;

    TCPTransactionId getTransactionId()
    {
        std::unique_lock<std::recursive_mutex> scopedLock(mutex);
        return myTransId++;
    }

    size_t sendMessage(std::shared_ptr<TCPSocketInfo> pSocketInfo, const CDRMessage_t &msg) const;
    bool sendData(std::shared_ptr<TCPSocketInfo> pSocketInfo, TCPCPMKind kind,
        const TCPTransactionId &transactionId, const SerializedPayload_t *payload = nullptr, 
        const ResponseCode respCode = RETCODE_VOID);
    void fillHeaders(TCPCPMKind kind, const TCPTransactionId &transactionId,
        TCPControlMsgHeader &retCtrlHeader, TCPHeader &header, 
        const SerializedPayload_t *payload = nullptr,
        const ResponseCode *respCode = nullptr);
};
} /* namespace rtps */
} /* namespace fastrtps */
} /* namespace eprosima */
#endif
#endif /* RTCP_MESSAGEMANAGER_H_ */