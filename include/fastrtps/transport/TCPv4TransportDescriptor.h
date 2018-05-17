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

#ifndef TCPV4_TRANSPORT_DESCRIPTOR
#define TCPV4_TRANSPORT_DESCRIPTOR

#include "TransportInterface.h"

namespace eprosima{
namespace fastrtps{
namespace rtps{

/**
 * Transport configuration
 *
 * - bufferSize:    length of the buffers used for transmission. Passing
 *                  a buffer of different size will cause transmission to
 *                  fail.
 *
 * - interfaceWhiteList: Lists the allowed interfaces.
 * @ingroup TRANSPORT_MODULE
 */
typedef struct TCPv4TransportDescriptor: public TransportDescriptorInterface {
    virtual ~TCPv4TransportDescriptor(){}

    virtual TransportInterface* create_transport() const override;

    std::vector<uint16_t> listening_ports;
    octet wan_addr[4];

    void set_WAN_address(octet o1,octet o2,octet o3,octet o4){
        wan_addr[0] = o1;
        wan_addr[1] = o2;
        wan_addr[2] = o3;
        wan_addr[3] = o4;
    }

    void set_WAN_address(const std::string& in_address)
    {
        std::stringstream ss(in_address);
        int a,b,c,d; //to store the 4 ints
        char ch; //to temporarily store the '.'
        ss >> a >> ch >> b >> ch >> c >> ch >> d;
        wan_addr[12] = (octet)a;
        wan_addr[13] = (octet)b;
        wan_addr[14] = (octet)c;
        wan_addr[15] = (octet)d;
    }

    RTPS_DllAPI TCPv4TransportDescriptor();

    RTPS_DllAPI TCPv4TransportDescriptor(const TCPv4TransportDescriptor& t);
} TCPv4TransportDescriptor;

} // namespace rtps
} // namespace fastrtps
} // namespace eprosima

#endif