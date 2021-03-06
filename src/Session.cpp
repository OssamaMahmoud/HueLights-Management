
#include "Wt/Auth/AuthService"
#include "Wt/Auth/HashFunction"
#include "Wt/Auth/PasswordService"
#include "Wt/Auth/PasswordStrengthValidator"
#include "Wt/Auth/PasswordVerifier"
#include "Wt/Auth/GoogleService"
#include "Wt/Auth/FacebookService"
#include "Wt/Auth/Dbo/AuthInfo"
#include "Wt/Auth/Dbo/UserDatabase"

#include "Session.h"

namespace {

    class MyOAuth : public std::vector<const Wt::Auth::OAuthService *>
    {
    public:
        ~MyOAuth()
        {
            for (unsigned i = 0; i < size(); ++i)
                delete (*this)[i];
        }
    };

    Wt::Auth::AuthService myAuthService;
    Wt::Auth::PasswordService myPasswordService(myAuthService);
    MyOAuth myOAuthServices;

}

void Session::configureAuth()
{
    myAuthService.setAuthTokensEnabled(true, "logincookie");
    myAuthService.setEmailVerificationEnabled(false);
    myAuthService.setEmailVerificationRequired(false);

    Wt::Auth::PasswordVerifier *verifier = new Wt::Auth::PasswordVerifier();
    verifier->addHashFunction(new Wt::Auth::BCryptHashFunction(7));
    myPasswordService.setVerifier(verifier);
    myPasswordService.setAttemptThrottlingEnabled(true);
    myPasswordService.setStrengthValidator(new Wt::Auth::PasswordStrengthValidator());

    if (Wt::Auth::GoogleService::configured())
        myOAuthServices.push_back(new Wt::Auth::GoogleService(myAuthService));

    if (Wt::Auth::FacebookService::configured())
        myOAuthServices.push_back(new Wt::Auth::FacebookService(myAuthService));

    for (unsigned i = 0; i < myOAuthServices.size(); ++i)
        myOAuthServices[i]->generateRedirectEndpoint();
}

Session::Session(const std::string& sqliteDb)
        : connection_(sqliteDb)
{
    connection_.setProperty("show-queries", "true");

    setConnection(connection_);

    mapClass<User>("user");
    mapClass<AuthInfo>("auth_info");
    mapClass<AuthInfo::AuthIdentityType>("auth_identity");
    mapClass<AuthInfo::AuthTokenType>("auth_token");
    //mapClass<Bridge>("bridges");

    try {
        createTables();
        std::cerr << "Created database." << std::endl;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << "Using existing database";
    }

    users_ = new UserDatabase(*this);
}

Session::~Session()
{
    delete users_;
}

Wt::Auth::AbstractUserDatabase& Session::users()
{
    return *users_;
}


//use this to get a ptr to the user where ever you are in the application
dbo::ptr<User> Session::user() const
{
    if (login_.loggedIn()) {
        dbo::ptr<AuthInfo> authInfo = users_->find(login_.user());
        return authInfo->user();
    } else
        return dbo::ptr<User>();
}

dbo::ptr<User> Session::user(const Wt::Auth::User& authUser)
{
    dbo::ptr<AuthInfo> authInfo = users_->find(authUser);

    dbo::ptr<User> user = authInfo->user();

    if (!user) {
        user = add(new User());
        authInfo.modify()->setUser(user);
    }

    return user;
}

const Wt::Auth::AuthService& Session::auth()
{
    myAuthService.setEmailVerificationRequired(false);
    myAuthService.setIdentityPolicy(Wt::Auth::IdentityPolicy::EmailAddressIdentity);
    return myAuthService;
}

const Wt::Auth::PasswordService& Session::passwordAuth()
{
    return myPasswordService;
}

const std::vector<const Wt::Auth::OAuthService *>& Session::oAuth()
{
    return myOAuthServices;
}