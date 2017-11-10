#ifndef AUTH_BRIDGE_H
#define AUTH_BRIDGE_H
#include <Wt/WApplication>
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
#include <string>

#include "User.h"

using namespace Wt;
namespace dbo = Wt::Dbo;

class User;

class Bridge: public WResource{
	public:
	    Bridge();
	    virtual ~Bridge();
		bool emptyObj; 
		Json::Object bridgeData;

	    dbo::ptr<User> user;

	    template<class Action>
	    void persist(Action& a)
	    {
	        dbo::belongsTo(a, user, "user");
	    }


	private: 
		void handleHttpResponse(boost::system::error_code err, const Http::Message& response);

};


#endif //AUTH_BRIDGE_H

#endif //AUTH_BRIDGE_H
