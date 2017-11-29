#include "UserDetailsModel.h"
#include "User.h"
#include "Session.h"

const Wt::WFormModel::Field
        UserDetailsModel::fNameField = "fName";


const Wt::WFormModel::Field
        UserDetailsModel::lNameField = "lName";

UserDetailsModel::UserDetailsModel(Session& session, Wt::WObject *parent)
        : Wt::WFormModel(parent),
          _session(session)
{
    this->addField(fNameField, "Enter first name");
    this->addField(lNameField, "Enter Last name");
}

void UserDetailsModel::save(const Wt::Auth::User& authUser)
{
    Wt::Dbo::ptr<User> user = _session.user(authUser);
    user.modify()->setFName(valueText(fNameField).toUTF8());
    user.modify()->setLName(valueText(lNameField).toUTF8());
    user.modify()->setBridgeIp("");
    user.modify()->setBridgePort("");
}