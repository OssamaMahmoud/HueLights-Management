#ifndef USER_H_
#define USER_H_

#include <Wt/Dbo/Types>
#include <Wt/WGlobal>
#include "Bridge.h"

namespace dbo = Wt::Dbo;

class User;


typedef Wt::Auth::Dbo::AuthInfo<User> AuthInfo;
using namespace std;


class User {
private:
    std::string fName;
    std::string lName;
    std::string bridgeIp, bridgePort;

    dbo::weak_ptr<AuthInfo> authInfo;

public:
    void setFName(const std::string &fName);

    void setLName(const std::string &lName);

    //const Wt::Dbo::collection<Wt::Dbo::ptr<Bridge>> &getBridgeList() const;

    //void setBridgeList(const Wt::Dbo::collection<Wt::Dbo::ptr<Bridge>> &bridgeList);

    const std::string &getFName() const;

    const std::string &getLName() const;



    const Wt::Dbo::weak_ptr<AuthInfo> &getAuthInfo() const;

    void setAuthInfo(const Wt::Dbo::weak_ptr<AuthInfo> &authInfo);



    //add bridges list here


    /* You probably want to add other user information here */

    template<class Action>
    void persist(Action& a) {
        dbo::field(a, fName, "fName");
        dbo::field(a, lName, "lName");
        dbo::field(a, bridgeIp, "bridgeIp");
        dbo::field(a, bridgeIp, "bridgePort");

        dbo::hasOne(a, authInfo, "user");
    }

    const string &getBridgeIp() const;

    void setBridgeIp(const string &bridgeIp);

    const string &getBridgePort() const;

    void setBridgePort(const string &bridgePort);


};


DBO_EXTERN_TEMPLATES(User);

#endif // USER_H_