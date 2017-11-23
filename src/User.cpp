//
// Created by ubuntu on 11/1/17.
//

#include "User.h"

#include <Wt/Dbo/Impl>
#include <Wt/Auth/Dbo/AuthInfo>

DBO_INSTANTIATE_TEMPLATES(User);

void User::setFName(const std::string &fName) {
    User::fName = fName;
}

void User::setLName(const std::string &lName) {
    User::lName = lName;
}


const Wt::Dbo::weak_ptr<AuthInfo> &User::getAuthInfo() const {
    return authInfo;
}

void User::setAuthInfo(const Wt::Dbo::weak_ptr<AuthInfo> &authInfo) {
    User::authInfo = authInfo;
}

const std::string &User::getFName() const {
    return fName;
}

const std::string &User::getLName() const {
    return lName;
}
