#ifndef USER_H_
#define USER_H_

#include <Wt/Dbo/Types>
#include <Wt/WGlobal>
#include "Bridge.h"

namespace dbo = Wt::Dbo;

class User;
class Bridge;
typedef Wt::Auth::Dbo::AuthInfo<User> AuthInfo;



class User {
private:
    std::string fName;
    std::string lName;
    dbo::collection < dbo::ptr<Bridge> > bridgeList;
    dbo::weak_ptr<AuthInfo> authInfo;
public:
    void setFName(const std::string &fName);

    void setLName(const std::string &lName);

    const Wt::Dbo::collection<Wt::Dbo::ptr<Bridge>> &getBridgeList() const;

    void setBridgeList(const Wt::Dbo::collection<Wt::Dbo::ptr<Bridge>> &bridgeList);

    const Wt::Dbo::weak_ptr<AuthInfo> &getAuthInfo() const;

    void setAuthInfo(const Wt::Dbo::weak_ptr<AuthInfo> &authInfo);

    const std::string &getFName() const;

    const std::string &getLName() const;

    //add bridges list here


    /* You probably want to add other user information here */

    template<class Action>
    void persist(Action& a) {
        dbo::field(a, fName, "fName");
        dbo::field(a, lName, "lName");
        dbo::hasOne(a, authInfo, "user");
        dbo::hasMany(a, bridgeList, dbo::ManyToOne, "user");
    }


};


DBO_EXTERN_TEMPLATES(User);

#endif // USER_H_