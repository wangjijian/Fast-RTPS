#ifndef RTPS_SOCKET_INFO_
#define RTPS_SOCKET_INFO_

#include <asio.hpp>
#include <memory>
#include <fastrtps/rtps/messages/MessageReceiver.h>

namespace eprosima{
namespace fastrtps{
namespace rtps{

#if defined(ASIO_HAS_MOVE)
    // Typedefs
	typedef asio::ip::udp::socket eProsimaUDPSocket;
	typedef asio::ip::tcp::socket eProsimaTCPSocket;
    // UDP
	inline eProsimaUDPSocket* getSocketPtr(eProsimaUDPSocket &socket)
    {
        return &socket;
    }
    inline eProsimaUDPSocket moveSocket(eProsimaUDPSocket &socket)
    {
        return std::move(socket);
    }
    inline eProsimaUDPSocket createUDPSocket(asio::io_service& io_service)
    {
        return std::move(asio::ip::udp::socket(io_service));
    }
    inline eProsimaUDPSocket& getRefFromPtr(eProsimaUDPSocket* socket)
    {
        return *socket;
    }
    // TCP
	inline eProsimaTCPSocket* getSocketPtr(eProsimaTCPSocket &socket)
    {
        return &socket;
    }
    inline eProsimaTCPSocket moveSocket(eProsimaTCPSocket &socket)
    {
        return std::move(socket);
    }
    inline eProsimaTCPSocket createTCPSocket(asio::io_service& io_service)
    {
        return std::move(asio::ip::tcp::socket(io_service));
    }
    inline eProsimaTCPSocket& getRefFromPtr(eProsimaTCPSocket* socket)
    {
        return *socket;
    }
#else
    // Typedefs
	typedef std::shared_ptr<asio::ip::udp::socket> eProsimaUDPSocket;
	typedef std::shared_ptr<asio::ip::tcp::socket> eProsimaTCPSocket;
    // UDP
    inline eProsimaUDPSocket getSocketPtr(eProsimaUDPSocket &socket)
    {
        return socket;
    }
    inline eProsimaUDPSocket& moveSocket(eProsimaUDPSocket &socket)
    {
        return socket;
    }
    inline eProsimaUDPSocket createUDPSocket(asio::io_service& io_service)
    {
        return std::make_shared<asio::ip::udp::socket>(io_service);
    }
    inline eProsimaUDPSocket& getRefFromPtr(eProsimaUDPSocket &socket)
    {
        return socket;
    }
    // TCP
    inline eProsimaTCPSocket getSocketPtr(eProsimaTCPSocket &socket)
    {
        return socket;
    }
    inline eProsimaTCPSocket& moveSocket(eProsimaTCPSocket &socket)
    {
        return socket;
    }
    inline eProsimaTCPSocket createTCPSocket(asio::io_service& io_service)
    {
        return std::make_shared<asio::ip::tcp::socket>(io_service);
    }
    inline eProsimaTCPSocket& getRefFromPtr(eProsimaTCPSocket &socket)
    {
        return socket;
    }
#endif

class UDPSocketInfo
    {
        public:
            UDPSocketInfo(eProsimaUDPSocket& socket)
                : mp_receiver(nullptr)
                , m_bAlive(true)
                , m_thread(nullptr)
                , socket_(moveSocket(socket))
            {
            }

            UDPSocketInfo(UDPSocketInfo&& socketInfo)
                : mp_receiver(nullptr)
                , m_bAlive(true)
                , m_thread(nullptr)
                , socket_(moveSocket(socketInfo.socket_))
            {
            }

            ~UDPSocketInfo()
            {
                m_bAlive = false;
                if (m_thread != nullptr)
                {
                    m_thread->join();
                    delete m_thread;
                }
                mp_receiver = nullptr;
            }

            UDPSocketInfo& operator=(UDPSocketInfo&& socketInfo)
            {
                socket_ = moveSocket(socketInfo.socket_);
                return *this;
            }

            void only_multicast_purpose(const bool value)
            {
                only_multicast_purpose_ = value;
            };

            bool& only_multicast_purpose()
            {
                return only_multicast_purpose_;
            }

            bool only_multicast_purpose() const
            {
                return only_multicast_purpose_;
            }

#if defined(ASIO_HAS_MOVE)
            inline eProsimaUDPSocket* getSocket()
#else
            inline eProsimaUDPSocket getSocket()
#endif
            {
                return getSocketPtr(socket_);
            }

            inline void SetThread(std::thread* pThread)
            {
                m_thread = pThread;
            }

            inline bool IsAlive() const
            {
                return m_bAlive;
            }

            inline void Disable()
            {
                m_bAlive = false;
            }

            inline void SetMessageReceiver(std::shared_ptr<MessageReceiver> receiver)
            {
                mp_receiver = receiver;
            }

            inline std::shared_ptr<MessageReceiver> GetMessageReceiver()
            {
                return mp_receiver;
            }

        private:

            std::shared_ptr<MessageReceiver> mp_receiver; //Associated Readers/Writers inside of MessageReceiver
            bool m_bAlive;
            std::thread* m_thread;
            eProsimaUDPSocket socket_;
            bool only_multicast_purpose_;
            UDPSocketInfo(const UDPSocketInfo&) = delete;
            UDPSocketInfo& operator=(const UDPSocketInfo&) = delete;
    };

class TCPSocketInfo
{
    public:
        TCPSocketInfo(eProsimaTCPSocket& socket)
            : mp_receiver(nullptr)
            , m_bAlive(true)
            , m_thread(nullptr)
            , socket_(moveSocket(socket))
        {
            mMutex = std::make_shared<std::recursive_mutex>();
        }

        TCPSocketInfo(TCPSocketInfo&& socketInfo)
            : mp_receiver(nullptr)
            , m_bAlive(true)
            , m_thread(nullptr)
            , socket_(moveSocket(socketInfo.socket_))
        {
            mMutex = std::make_shared<std::recursive_mutex>();
        }

        ~TCPSocketInfo()
        {
            m_bAlive = false;
            if (m_thread != nullptr)
            {
                m_thread->join();
                delete m_thread;
            }
            mp_receiver = nullptr;
        }

        TCPSocketInfo& operator=(TCPSocketInfo&& socketInfo)
        {
            socket_ = moveSocket(socketInfo.socket_);
            return *this;
        }

        void only_multicast_purpose(const bool value)
        {
            only_multicast_purpose_ = value;
        };

        bool& only_multicast_purpose()
        {
            return only_multicast_purpose_;
        }

        bool only_multicast_purpose() const
        {
            return only_multicast_purpose_;
        }

#if defined(ASIO_HAS_MOVE)
        inline eProsimaTCPSocket* getSocket()
#else
        inline eProsimaTCPSocket getSocket()
#endif
        {
            return getSocketPtr(socket_);
        }

        std::recursive_mutex& GetMutex() const
        {
            return *mMutex;
        }

        inline void SetThread(std::thread* pThread)
        {
            m_thread = pThread;
        }

        inline bool IsAlive() const
        {
            return m_bAlive;
        }

        inline void Disable()
        {
            m_bAlive = false;
        }

        inline void SetMessageReceiver(std::shared_ptr<MessageReceiver> receiver)
        {
            mp_receiver = receiver;
        }

        inline std::shared_ptr<MessageReceiver> GetMessageReceiver()
        {
            return mp_receiver;
        }

    private:

        std::shared_ptr<MessageReceiver> mp_receiver; //Associated Readers/Writers inside of MessageReceiver
        bool m_bAlive;
        std::thread* m_thread;
        std::shared_ptr<std::recursive_mutex> mMutex;
        eProsimaTCPSocket socket_;
        bool only_multicast_purpose_;
        TCPSocketInfo(const TCPSocketInfo&) = delete;
        TCPSocketInfo& operator=(const TCPSocketInfo&) = delete;
};


} // namespace rtps
} // namespace fastrtps
} // namespace eprosima

#endif // RTPS_TCP_PORT_MANAGER_