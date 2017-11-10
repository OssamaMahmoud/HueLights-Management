#ifndef AUTH_BRIDGE_H_
#define AUTH_BRIDGE_H_
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/Http/Client>
#include <Wt/Http/Request>
#include <Wt/Json/Object>
#include <Wt/Json/Parser>
#include <Wt/Dbo/Dbo>
#include <Wt/WResource>
#include <string>

#include "User.h"

using namespace Wt;
using namespace Wt::Http;
namespace dbo = Wt::Dbo;

class User;

class Bridge: public WResource {
	public:
	    Bridge();
	    virtual ~Bridge();
		Json::Object bridgeData;

	    dbo::ptr<User> user;

	    template<class Action>
	    void persist(Action& a)
	    {
	        dbo::belongsTo(a, user, "user");
	    }
 
	protected: 
		void handleRequest(const Request &request, Response &response);
		void handleHttpResponse(boost::system::error_code err, const Http::Message& response);
};

#endif //AUTH_BRIDGE_H
