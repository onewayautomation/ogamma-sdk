#pragma once

#include <string>
#include <future>
#include <memory>
#include <mutex>
#include <functional>
#include <stdint.h>
#include <vector>
#include "opcua/OperationResult.h"
#include "opcua/DateTime.h"
#include "opcua/EnpointUrl.h"
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/deadline_timer.hpp>

namespace OWA {
  namespace OpcUa {
    struct ResolveResultType
    {
      ResolveResultType();

      // Start initialization by settings remote (or, "official") endpoint URL. For initial connections, it is what the user enters as endpoint URL. 
      // Might have IP address or host name
      // This method does not do any logic, just sets single member variable.
      // For the endpoint used to call GetEndpoints, it is set to the  value returned by the FindServers response.
      // For the endpoint used to call CreateSession, it is set to the value returned by the GetEndpoints response.
      ResolveResultType& setRemoteEndpointUrl(const std::string& url);
      
      ResolveResultType& setUrlMap(const std::map<std::string, std::string>&);

      ResolveResultType& setDnsMap(const std::map<std::string, std::string>&);

      // Used to initialize from other instance. 
      // For example, the instance which is going to be used to call GetEndpoints, can be initialized from instance used to call FindServer.
      // After this, if the other instance was already resolved, this might be resolved too.
      ResolveResultType& InitFromOther(const ResolveResultType&);

      // Called to update fields with another instance, which has resolved IP addresses.
      ResolveResultType& CopyResolveResult(const ResolveResultType&);

      // Sets endpoint variable, no logic.
      ResolveResultType& setLocalEndpointUrl(const std::string& url);

      // Implements logic to set the local URL, based on remote URL, and existing mappings.
      ResolveResultType& setLocalUrlFromRemote();

      // Reset before starting next resolve attempt.
      ResolveResultType& onStartResolve();

      // This method by default tries to resolve using existing maps.
      bool resolved(bool trueToResolve = true);

      std::string getNextIpv4Address();
      uint32_t getNumericPortNumber();
      std::string getRemoteEndpointUrl();
      ResolveResultType& clearCache();

      ResolveResultType& addIpAddress(const std::string&);
      
      // Name is used for debugging/troubleshooting. Usually set to Connection or Transport name.
      std::string name;

      // Endpoint URL used to open TCP connection
      EndpointUrl endpointUrl;

      // Endpoint URL as it was returned by FindServers or GetEndpoints service, 
      // This one should be used to set in Hello Message, CreateSession service, and to validate server certificates.
      // If the server runs in Docker, this might be Docker service name. 
      EndpointUrl remoteEndpointUrl;
      
      uint32_t timeout;

      DateTime startTime;

      OperationResult result;
      uint32_t executionTimeInMs;
      std::string hostName;
      uint32_t port;
      std::vector<std::string> ip4Addresses;
      std::vector<std::string> ip6Addresses;

      int   index;
      bool  isResolved;

      // Maps used to resolve host name to IP address. Copied from ClientConfiguraiton.
      /* EndpointURL map. Used to substitute whole endpoint URLm including protocol and port number.*/
      // Before starting name resolution, remoteEndpointUrl is mapped using this map.
      std::map<std::string, std::string> urlMap;

      /* DNS Resolver Map, used to substitute host name returned by FindServers or GetEndpoints response. */
      // Can have IP address or host name in the "value" part.
      std::map<std::string, std::string> dnsMap;

      // The cache, used for quick resolving of host names to IP addresses. Cleared after many failing connection attempts. 
      // Initially has copy from dnsMap entries having IP address in the value part.
      std::map<std::string, std::string> cache;
    };

    typedef std::function<void(const ResolveResultType&)> ResolverCallbackFunctionType;

    class IpAddressResolver: public std::enable_shared_from_this<IpAddressResolver> {
    public:
      IpAddressResolver();
      // Method to resolve host name to vector of IP Addresses.
      std::shared_future<ResolveResultType> resolve(
        const std::string name,
        const std::string endpointUrl,
        uint32_t timeout,
        ResolverCallbackFunctionType cb );

      static OperationResult getBoostEndpoint(const std::string ip, uint32_t port, boost::asio::ip::tcp::endpoint& result);
      
      std::string getCurrentIpV4Address();
      static bool isIpAddress(const std::string&);
      static bool isIpv4address(const std::string&);
      static bool isIpv6address(const std::string&);

      virtual ~IpAddressResolver();

    protected:

      std::promise<ResolveResultType> promise;
      std::vector<ResolverCallbackFunctionType> callbacks;
      std::shared_future<ResolveResultType> future;

      ResolveResultType result;
      std::recursive_mutex stateMutex;
      std::shared_ptr<boost::asio::io_service> ioService;

      std::shared_ptr<boost::asio::ip::tcp::resolver> resolver_;
      std::shared_ptr<boost::asio::deadline_timer> timer_;
      bool isValueSet;
    };
  }
}